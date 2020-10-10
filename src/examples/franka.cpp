#include <iostream>
#include <robot_dart/robot_dart_simu.hpp>

#include <robot_dart/control/pd_control.hpp>

#ifdef GRAPHIC
#include <robot_dart/gui/magnum/graphics.hpp>
#endif

int main()
{
    std::srand(std::time(NULL));

    std::vector<std::pair<std::string, std::string>> packages = {{"franka_description", "franka/franka_description"}};
    auto global_robot = std::make_shared<robot_dart::Robot>("franka/franka.urdf", packages);
    global_robot->set_color_mode("material");

    // pin the arm to world
    global_robot->fix_to_world();
    global_robot->set_position_enforced(true);

    global_robot->set_actuator_types("torque");

    // add a PD-controller to the arm
    // set desired positions
    Eigen::VectorXd ctrl(8);
    ctrl << 0., M_PI / 4., 0., -M_PI / 4, 0., M_PI / 2., 0., 0.;

    // add the controller to the robot
    global_robot->add_controller(std::make_shared<robot_dart::control::PDControl>(ctrl));
    std::static_pointer_cast<robot_dart::control::PDControl>(global_robot->controllers()[0])->set_pd(300., 50.);

    // choose time step of 0.001 seconds
    robot_dart::RobotDARTSimu simu(0.001);
    simu.set_collision_detector("fcl");

#ifdef GRAPHIC
    auto graphics = std::make_shared<robot_dart::gui::magnum::Graphics>(&simu);
    simu.set_graphics(graphics);
    // set the camera at position (0, 3, 1) looking at the center (0, 0, 0)
    graphics->look_at({0., 3., 1.}, {0., 0., 0.});
#endif

    simu.add_checkerboard_floor();
    simu.add_robot(global_robot);

    simu.run(30.);
    global_robot.reset();
    return 0;
}
