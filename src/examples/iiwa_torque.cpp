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
    auto global_robot = std::make_shared<robot_dart::Robot>("iiwa/iiwa.urdf", packages);

    global_robot->fix_to_world();
    // global_robot->set_position_enforced(true);
    global_robot->set_actuator_types("torque");

    Eigen::VectorXd ctrl(7);
    ctrl << 0., M_PI / 3., 0., -M_PI / 4., 0., 0., 0.;

    global_robot->add_controller(std::make_shared<robot_dart::control::PDControl>(ctrl));
    std::static_pointer_cast<robot_dart::control::PDControl>(global_robot->controllers()[0])->set_pd(300., 50.);

    // Add a ghost robot; only visuals, no dynamics, no collision
    auto ghost = global_robot->clone_ghost();

    robot_dart::RobotDARTSimu simu(0.001);
    simu.set_gravity(-simu.gravity());
    //simu.set_gravity(Eigen::Vector3d::Zero());
    simu.set_collision_detector("fcl");
#ifdef GRAPHIC
    auto graphics = std::make_shared<robot_dart::gui::magnum::Graphics>(&simu);
    simu.set_graphics(graphics);

    graphics->look_at({0., 3.5, 2.}, {0., 0., 0.25});
#endif
    simu.add_checkerboard_floor();
    simu.add_robot(global_robot);
    simu.add_robot(ghost);

    // Format Eigen to std::cout
    Eigen::IOFormat fmt(Eigen::StreamPrecision, Eigen::DontAlignCols, " ", "\n", "", "");
    std::cout.precision(5);

    for(const auto& jnt : global_robot->skeleton()->getJoints())
    {
        for(int i=0; i<jnt->getNumDofs(); ++i)
        {
            jnt->setDampingCoefficient(i, 0.0);
            std::cout << "Stiffness: " << jnt->getSpringStiffness(i) << std::endl;
            std::cout << "Friction: " << jnt->getCoulombFriction(i) << std::endl;
            std::cout << "Damping: " << jnt->getDampingCoefficient(i) << std::endl;
        }
    }


    // Add a torque sensors to the robot
    int ct=0;
    std::shared_ptr<robot_dart::sensor::Torque> tq_sensors[global_robot->num_dofs()];
    for(const auto& joint : global_robot->dof_names())
        tq_sensors[ct++] = simu.add_sensor<robot_dart::sensor::Torque>(&simu, global_robot, joint, 500);

    auto start = std::chrono::steady_clock::now();
    Eigen::Vector3d external_force = Eigen::Vector3d::Zero();
    while (simu.scheduler().next_time() < 20 && !simu.graphics()->done()) {

        // Print torque sensor measurement
        if (simu.schedule(tq_sensors[0]->frequency())) {

            ct=0;
            Eigen::VectorXd torques_measure(global_robot->num_dofs());
            for(const auto& tq_sens : tq_sensors)
                torques_measure.block<1,1>(ct++, 0) = tq_sens->torques();

            // get joint torque due to external force using jacobian
            Eigen::MatrixXd jac = global_robot->jacobian("iiwa_link_4").bottomRows<3>();
            Eigen::VectorXd ext_tau = jac.transpose() * external_force;


            // actual torque on joints (the one that should be measured?)
            Eigen::MatrixXd M = global_robot->aug_mass_matrix();
            Eigen::VectorXd c = global_robot->coriolis_gravity_forces();
            Eigen::VectorXd qdd = global_robot->accelerations();
            Eigen::VectorXd tau = M * qdd + c;


            std::cout << "  coriolis force: " << global_robot->coriolis_forces().transpose().format(fmt) << std::endl;
            std::cout << "   gravity force: " << global_robot->gravity_forces().transpose().format(fmt) << std::endl;
            std::cout << "   C and g force: " << global_robot->coriolis_gravity_forces().transpose().format(fmt) << std::endl;
            std::cout << "commanded torque: " << global_robot->commands().transpose().format(fmt) << std::endl;
            std::cout << "  motors' torque:   " << global_robot->forces().transpose().format(fmt) << std::endl;
            std::cout << " sensors' torque:  " << torques_measure.transpose().format(fmt) << std::endl;
            std::cout << " external torque:  " << ext_tau.transpose().format(fmt) << std::endl;
            std::cout << "inv. dyn. torque:  " << tau.transpose().format(fmt) << std::endl;
            std::cout << "=================================" << std::endl << std::endl;
        }


        if (simu.schedule(simu.control_freq())) {
            Eigen::MatrixXd K = 10 * Eigen::MatrixXd::Identity(global_robot->num_dofs(), global_robot->num_dofs());

            Eigen::VectorXd velocities = global_robot->controllers()[0]->calculate(simu.scheduler().next_time());
            Eigen::VectorXd commands = global_robot->mass_matrix() * (K * velocities) + global_robot->coriolis_gravity_forces();

            global_robot->set_commands(commands);
            //global_robot->set_commands(Eigen::VectorXd::Zero(global_robot->num_dofs()));
        }

        simu.step_world();


        // add external force
        int time_in_seconds = static_cast<int>(simu.scheduler().next_time());
        if (time_in_seconds % 3 >= 1 && time_in_seconds % 3 < 2) {
            external_force = Eigen::Vector3d::Constant(30.0);
            //std::cout << "Applying force on iiwa_link_4" << std::endl;
        }
        else {
            external_force = Eigen::Vector3d::Zero();
        }

        global_robot->set_external_force("iiwa_link_4", external_force);
        
    }
    auto end = std::chrono::steady_clock::now();

    global_robot.reset();
    return 0;
}