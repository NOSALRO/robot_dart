#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <robot_dart/control/pd_control.hpp>
#include <robot_dart/robot_dart_simu.hpp>

#include <dart/collision/fcl/FCLCollisionDetector.hpp>
#include <dart/constraint/ConstraintSolver.hpp>

#ifdef GRAPHIC
#include <robot_dart/gui/magnum/graphics.hpp>
#endif

int main()
{
    std::srand(std::time(NULL));

    std::vector<std::pair<std::string, std::string>> packages = {{"icub_description", "icub/icub_description"}};
    auto robot = std::make_shared<robot_dart::Robot>("icub/icub.urdf", packages);

    robot->set_position_enforced(true);
    robot->skeleton()->setPosition(5, 0.5);
    robot->skeleton()->setPosition(2, 1.57);

    // Set actuator types to VELOCITY motors so that they stay in position without any controller
    robot->set_actuator_types("velocity");
  
    robot_dart::RobotDARTSimu simu(0.001);
    simu.set_collision_detector("fcl");
#ifdef GRAPHIC
    auto graphics = std::make_shared<robot_dart::gui::magnum::Graphics>(&simu);
    simu.set_graphics(graphics);
    graphics->look_at({0., 3.5, 2.}, {0., 0., 0.25});
//    graphics->record_video("icub.mp4");

#endif
    simu.add_checkerboard_floor();
    simu.add_robot(robot);


    simu.set_control_freq(100); // 100 Hz
    std::vector<std::string> dofs = {};

    auto start = std::chrono::steady_clock::now();
    while (simu.scheduler().next_time() < 20 && !simu.graphics()->done()) { 
        if (simu.schedule(simu.control_freq())) {
//            Eigen::VectorXd commands(5);
            // commands << sin(simu.scheduler().current_time() * 4),
            //             sin(simu.scheduler().current_time() * 4),
            //             sin(simu.scheduler().current_time() * 4),
            //             sin(simu.scheduler().current_time() * 4),
            //             sin(simu.scheduler().current_time() * 4);
  //          robot->set_commands(commands, dofs);
        }
        simu.step_world();
    }
    auto end = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsed_seconds = end-start;
    std::cout << "benchmark time: " << elapsed_seconds.count() << "s\n";

    robot.reset();
    return 0;
}
