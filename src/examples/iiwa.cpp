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
    global_robot->set_position_enforced(true);

    Eigen::VectorXd ctrl(7);
    ctrl << 0., M_PI / 3., 0., -M_PI / 4., 0., 0., 0.;

    global_robot->add_controller(std::make_shared<robot_dart::control::PDControl>(ctrl));
    std::static_pointer_cast<robot_dart::control::PDControl>(global_robot->controllers()[0])->set_pd(300., 50.);

    // Add a ghost robot; only visuals, no dynamics, no collision
    auto ghost = global_robot->clone_ghost();

    robot_dart::RobotDARTSimu simu(0.001);
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
    std::cout.precision(4); 


    // Add a torque sensors to the robot
    int ct=0;
    std::shared_ptr<robot_dart::sensor::Torque> tq_sensors[global_robot->num_dofs()];
    for(const auto& joint : global_robot->dof_names())
        tq_sensors[ct++] = simu.add_sensor<robot_dart::sensor::Torque>(&simu, global_robot, joint);

    auto start = std::chrono::steady_clock::now();
    while (simu.scheduler().next_time() < 20 && !simu.graphics()->done()) {

        if (simu.schedule(simu.control_freq())) {
            Eigen::VectorXd commands = global_robot->controllers()[0]->calculate(simu.scheduler().next_time());
            global_robot->set_commands(commands);
        }

        simu.step_world();

        // Print torque sensor measurement
        if (simu.schedule(tq_sensors[0]->frequency())) {
            
            ct=0;
            Eigen::VectorXd torques(global_robot->num_dofs());
            for(const auto& tq_sens : tq_sensors)
                torques.block<1,1>(ct++, 0) = tq_sens->torques();
            
            std::cout << "sensors' torque: " << torques.transpose().format(fmt) << std::endl;
            std::cout << "motors' torque: "  << global_robot->forces().transpose().format(fmt) <<std::endl;
            std::cout << "=================================" << std::endl;
        }
    }
    auto end = std::chrono::steady_clock::now();

    global_robot.reset();
    return 0;
}