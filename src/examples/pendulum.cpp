#include <robot_dart/control/simple_control.hpp>
#include <robot_dart/robot_dart_simu.hpp>
#include <robot_dart/robots/pendulum.hpp>

#ifdef GRAPHIC
#include <robot_dart/gui/magnum/graphics.hpp>
#endif

int main()
{
    auto robot = std::make_shared<robot_dart::robots::Pendulum>();

    Eigen::VectorXd ctrl(1);
    ctrl << 0.0;
    // @SIMPLE_CONTROL@
    auto controller1 = std::make_shared<robot_dart::control::SimpleControl>(ctrl);
    // add the controller to the robot, with a default weight of 1.0
    robot->add_controller(controller1);
    // @SIMPLE_CONTROL_END@
    ctrl << -1.0;
    auto controller2 = std::make_shared<robot_dart::control::SimpleControl>(ctrl);
    // add the controller to the robot, with a weight of 5.0
    robot->add_controller(controller2, 5.);

    // initialize the simulation with a default timestep of 0.015s
    robot_dart::RobotDARTSimu simu;
#ifdef GRAPHIC
    simu.set_graphics(std::make_shared<robot_dart::gui::magnum::Graphics>());
#endif
    simu.add_robot(robot);
    // the the dimensions of the pendulum to calculate the end effector position
    Eigen::Vector3d size(0.0402, 0.05, 1);
    std::cout << (robot->body_pose("pendulum_link_1") * size).transpose() << std::endl;
    simu.run(2.5);
    std::cout << (robot->body_pose("pendulum_link_1") * size).transpose() << std::endl;
    ctrl << 2.5;
    controller1->set_parameters(ctrl);
    simu.run(2.5);
    std::cout << (robot->body_pose("pendulum_link_1") * size).transpose() << std::endl;
    // reset the positions, velocities, and accelerations of the robot, clear internal and external forces, and also commands
    robot.reset();
    return 0;
}
