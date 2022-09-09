#include <robot_dart/robot_dart_simu.hpp>
#include <robot_dart/robots/franka.hpp>

#include <robot_dart/control/pd_control.hpp>

#ifdef GRAPHIC
#include <robot_dart/gui/magnum/graphics.hpp>
#endif

int main()
{
    auto robot = std::make_shared<robot_dart::robots::Franka>();
    robot->set_actuator_types("torque");

    // add a PD-controller to the arm
    // set desired positions
    Eigen::VectorXd ctrl = robot_dart::make_vector({0., M_PI / 4., 0., -M_PI / 4., 0., M_PI / 2., 0., 0.});

    // add the controller to the robot
    auto controller = std::make_shared<robot_dart::control::PDControl>(ctrl);
    robot->add_controller(controller);
    controller->set_pd(300., 50.);

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
    robot.reset();
    return 0;
}
