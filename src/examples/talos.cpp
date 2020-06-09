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

    std::vector<std::pair<std::string, std::string>> packages = {{"talos_description", "talos/talos_description"}};
    auto robot = std::make_shared<robot_dart::Robot>("talos/talos.urdf", packages);
    std::cout<<"The model used is:["<<robot->model_filename()<<"]"<<std::endl;
   
    robot->set_position_enforced(true);
    robot->skeleton()->setPosition(5, 1.1);
    robot->skeleton()->setPosition(2, 1.57);

    // Set actuator types to VELOCITY motors so that they stay in position without any controller
    robot->set_actuator_types(dart::dynamics::Joint::VELOCITY);
    // First 6-DOFs should always be FORCE if robot is floating base
    for (size_t i = 0; i < 6; i++)
        robot->set_actuator_type(i, dart::dynamics::Joint::FORCE);

    robot_dart::RobotDARTSimu simu(0.001);
    simu.set_collision_detector("fcl");
#ifdef GRAPHIC
    auto graphics = std::make_shared<robot_dart::gui::magnum::Graphics>(&simu);
    simu.set_graphics(graphics);
    graphics->look_at({0., 3.5, 2.}, {0., 0., 0.25});
    graphics->record_video("talos_dancing.mp4");

#endif
    simu.add_checkerboard_floor();
    simu.add_robot(robot);


    simu.set_control_freq(100); // 100 Hz
    std::vector<std::string> dofs = {"arm_left_1_joint", 
        "arm_left_2_joint", 
        "arm_right_1_joint", 
        "arm_right_2_joint",
        "torso_1_joint"};
    robot->set_control_mode("velocity");

    auto start = std::chrono::steady_clock::now();
    while (simu.scheduler().next_time() < 20 && !simu.graphics()->done()) { 
        if (simu.schedule(simu.control_freq())) {
            Eigen::VectorXd commands(5);
            commands << sin(simu.scheduler().current_time() * 4),
                        sin(simu.scheduler().current_time() * 4),
                        sin(simu.scheduler().current_time() * 4),
                        sin(simu.scheduler().current_time() * 4),
                        sin(simu.scheduler().current_time() * 4);
            robot->set_commands(commands, dofs);
        }
        simu.step_world();
    }
    auto end = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsed_seconds = end-start;
    std::cout << "benchmark time: " << elapsed_seconds.count() << "s\n";

    robot.reset();
    return 0;
}
