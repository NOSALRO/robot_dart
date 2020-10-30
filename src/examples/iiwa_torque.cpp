#include <algorithm>
#include <cstdlib>
#include <iostream>

#include <robot_dart/control/pd_control.hpp>
#include <robot_dart/robot_dart_simu.hpp>

#include <robot_dart/sensor/torque.hpp>

#ifdef GRAPHIC
#include <robot_dart/gui/magnum/graphics.hpp>
#endif

int main()
{
    std::srand(std::time(NULL));

    std::vector<std::pair<std::string, std::string>> packages = {{"iiwa_description", "iiwa/iiwa_description"}};
    auto robot = std::make_shared<robot_dart::Robot>("iiwa/iiwa.urdf", packages);

    robot->fix_to_world();
    robot->set_position_enforced(false);
    robot->set_actuator_types("torque");

    Eigen::VectorXd ctrl(7);
    ctrl << 0., M_PI / 3., 0., -M_PI / 4., 0., 0., 0.;

    auto controller = std::make_shared<robot_dart::control::PDControl>(ctrl);
    robot->add_controller(controller);
    controller->set_pd(300., 50.);

    robot_dart::RobotDARTSimu simu(0.001);
#ifdef GRAPHIC
    auto graphics = std::make_shared<robot_dart::gui::magnum::Graphics>();
    simu.set_graphics(graphics);

    graphics->look_at({0., 3.5, 2.}, {0., 0., 0.25});
#endif
    simu.add_robot(robot);

    // Format Eigen to std::cout
    Eigen::IOFormat fmt(Eigen::StreamPrecision, Eigen::DontAlignCols, " ", "\n", "", "");
    std::cout.precision(5);

    robot->set_damping_coeffs(0.);
    robot->set_cfriction_coeffs(0.);

    for (const auto& jnt : robot->skeleton()->getJoints()) {
        for (size_t i = 0; i < jnt->getNumDofs(); ++i) {
            std::cout << "Stiffness: " << jnt->getSpringStiffness(i) << std::endl;
            std::cout << "Friction: " << jnt->getCoulombFriction(i) << std::endl;
            std::cout << "Damping: " << jnt->getDampingCoefficient(i) << std::endl;
        }
    }

    // Add a torque sensors to the robot
    int ct = 0;
    std::shared_ptr<robot_dart::sensor::Torque> tq_sensors[robot->num_dofs()];
    for (const auto& joint : robot->dof_names())
        tq_sensors[ct++] = simu.add_sensor<robot_dart::sensor::Torque>(robot, joint, 1000);

    auto start = std::chrono::steady_clock::now();
    Eigen::Vector3d external_force = Eigen::Vector3d::Zero();
    Eigen::MatrixXd M = robot->mass_matrix(); // aug_mass_matrix includes the damping, friction coeffs
    Eigen::VectorXd c = robot->coriolis_gravity_forces();

    while (simu.scheduler().next_time() < 20 && !simu.graphics()->done()) {
        simu.step();

        // Print torque sensor measurement
        if (simu.schedule(tq_sensors[0]->frequency())) {
            ct = 0;
            Eigen::VectorXd torques_measure(robot->num_dofs());
            for (const auto& tq_sens : tq_sensors)
                torques_measure.block<1, 1>(ct++, 0) = tq_sens->torques();

            // get joint torque due to external force using jacobian
            Eigen::MatrixXd jac = robot->jacobian("iiwa_link_4").bottomRows<3>();
            Eigen::VectorXd ext_tau = jac.transpose() * external_force;

            // actual torque on joints (the one that should be measured?)
            Eigen::VectorXd qdd = robot->accelerations();
            Eigen::VectorXd tau = M * qdd + c;

            std::cout << "  coriolis force: " << robot->coriolis_forces().transpose().format(fmt) << std::endl;
            std::cout << "   gravity force: " << robot->gravity_forces().transpose().format(fmt) << std::endl;
            std::cout << "   C and g force: " << robot->coriolis_gravity_forces().transpose().format(fmt) << std::endl;
            std::cout << "commanded torque: " << robot->commands().transpose().format(fmt) << std::endl;
            std::cout << "  motors' torque:   " << robot->forces().transpose().format(fmt) << std::endl;
            std::cout << " sensors' torque:  " << torques_measure.transpose().format(fmt) << std::endl;
            std::cout << " external torque:  " << ext_tau.transpose().format(fmt) << std::endl;
            std::cout << "inv. dyn. torque:  " << tau.transpose().format(fmt) << std::endl;
            std::cout << "=================================" << std::endl
                      << std::endl;
        }

        // add external force
        int time_in_seconds = static_cast<int>(simu.scheduler().next_time());
        if (time_in_seconds % 3 >= 1 && time_in_seconds % 3 < 2) {
            external_force = Eigen::Vector3d::Constant(30.0);
            //std::cout << "Applying force on iiwa_link_4" << std::endl;
        }
        else {
            external_force = Eigen::Vector3d::Zero();
        }

        robot->set_external_force("iiwa_link_4", external_force);

        M = robot->mass_matrix();
        c = robot->coriolis_gravity_forces();
    }
    auto end = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsed_seconds = end - start;
    std::cout << "benchmark time: " << elapsed_seconds.count() << "s\n";

    robot.reset();
    return 0;
}