#include <robot_dart/robot_dart_simu.hpp>
#include <robot_dart/robots/ur3e.hpp>

#include <robot_dart/control/pd_control.hpp>

#ifdef GRAPHIC
#include <robot_dart/gui/magnum/graphics.hpp>
#endif

int main()
{
    auto robot = std::make_shared<robot_dart::robots::Ur3e>();
    robot->set_actuator_types("torque");

    for (size_t i = 0; i < robot->num_dofs(); i++)
        std::cout << "[" << i << "] " << robot->dof_name(i) << std::endl;

    std::vector<std::string> dofs = {"shoulder_pan_joint",
        "shoulder_lift_joint",
        "elbow_joint",
        "wrist_1_joint",
        "wrist_2_joint",
        "wrist_3_joint"};
    Eigen::VectorXd ctrl = robot_dart::make_vector({0, -M_PI / 2.0, M_PI / 2.0, 0.0, 0.0, 0.0});

    // add the controller to the robot
    auto controller = std::make_shared<robot_dart::control::PDControl>(ctrl, dofs);
    robot->add_controller(controller);
    controller->set_pd(2500., 250.);

    std::cout << "PD:" << controller->calculate(0).transpose() << std::endl;

    // choose time step of 0.001 seconds
    robot_dart::RobotDARTSimu simu(0.001);
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

    simu.run(30.);
    std::cout << robot->positions().transpose() << std::endl;
    robot.reset();

    return 0;
}
