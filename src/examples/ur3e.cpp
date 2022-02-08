#include <robot_dart/robot_dart_simu.hpp>
#include <robot_dart/robots/ur3e.hpp>

#include <robot_dart/control/pd_control.hpp>

#ifdef GRAPHIC
#include <robot_dart/gui/magnum/graphics.hpp>
#endif

int main(int argc, char** argv)
{
    double freq = 1000;
    bool hand = (argc > 1 && std::string(argv[1]) == "--hand") ? true : false;
    auto robot = hand ? std::make_shared<robot_dart::robots::Ur3e>(freq, "ur3e/ur3e_with_schunk_hand.urdf") :  std::make_shared<robot_dart::robots::Ur3e>();
    
    robot->set_actuator_types("torque");


    std::vector<std::string> dofs = {"shoulder_pan_joint",
        "shoulder_lift_joint",
        "elbow_joint",
        "wrist_1_joint",
        "wrist_2_joint",
        "wrist_3_joint"};
    auto up = robot->position_upper_limits(dofs);
    auto low = robot->position_lower_limits(dofs);
    for (size_t i = 0; i < dofs.size(); ++i)
        std::cout << "[" << i << "] " << dofs[i]
                  << " -> [" << low[i] << "," << up[i] << "]" << std::endl;

    Eigen::VectorXd ctrl = robot_dart::make_vector({0, -M_PI / 2.0, M_PI / 2.0, 0.0, 0.0, 0.0});
    // add the controller to the robot
    auto controller = std::make_shared<robot_dart::control::PDControl>(ctrl, dofs);
    robot->add_controller(controller);

    controller->set_pd(5000., 50.);

    // choose time step of 0.001 seconds
    robot_dart::RobotDARTSimu simu(1. / freq);
    simu.set_collision_detector("fcl");
    simu.enable_status_bar(true, 20); // change the font size

#ifdef GRAPHIC
    auto graphics = std::make_shared<robot_dart::gui::magnum::Graphics>();
    simu.set_graphics(graphics);
    // set the camera at position (0, 3, 1) looking at the center (0, 0, 0)
    graphics->look_at({0., 3., 1.}, {0., 0., 0.});
#endif

    simu.add_checkerboard_floor();
    simu.add_robot(robot);

    simu.run(10.);
    std::cout<<"robot->pos: " << robot->positions().transpose() << std::endl;
    robot.reset();

    return 0;
}
