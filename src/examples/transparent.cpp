#include <iostream>
#include <robot_dart/robot_dart_simu.hpp>

#include <robot_dart/control/pd_control.hpp>

#ifdef GRAPHIC
#include <robot_dart/gui/magnum/graphics.hpp>
#endif

inline std::shared_ptr<robot_dart::Robot> random_box(size_t num = 0)
{
    // random pose
    Eigen::Vector6d pose = Eigen::Vector6d::Random();
    // make sure it is above the ground
    pose(5) += 1.5;
    // random size
    Eigen::Vector3d size = Eigen::Vector3d::Random().array() * Eigen::Vector3d(0.1, 0.2, 0.1).array() + 0.3;
    return robot_dart::Robot::create_box(size, pose, "free", 1., dart::Color::Red(1.0), "box_" + std::to_string(num));
}

inline std::shared_ptr<robot_dart::Robot> random_sphere(size_t num = 0)
{
    // random pose
    Eigen::Vector6d pose = Eigen::Vector6d::Random();
    // make sure it is above the ground
    pose(5) += 1.5;
    // random size
    Eigen::Vector3d size = Eigen::Vector3d::Random()[0] * Eigen::Vector3d(0.2, 0.2, 0.2).array() + 0.3;
    // Transparent spheres
    Eigen::Vector4d color = robot_dart::make_vector({0., 0., 1., 0.5});
    return robot_dart::Robot::create_ellipsoid(size, pose, "free", 1., color, "sphere_" + std::to_string(num));
}

int main()
{
    // choose time step of 0.001 seconds
    robot_dart::RobotDARTSimu simu(0.001);
#ifdef GRAPHIC
    auto graphics = std::make_shared<robot_dart::gui::magnum::Graphics>();
    simu.set_graphics(graphics);
    // set the camera at position (0, 3, 1) looking at the center (0, 0, 0)
    graphics->look_at({0., 3., 1.}, {0., 0., 0.});
#endif

    // add floor of square size of 10 meters and height of 0.2 meters
    simu.add_floor(10., 0.2);

    // add random objects to the world
    for (size_t i = 0; i < 5; i++) {
        simu.add_robot(random_box(i));
        simu.add_robot(random_sphere(i));
    }

    // add a simple arm
    auto arm_robot = std::make_shared<robot_dart::Robot>("arm.urdf");
    // pin the arm to world
    arm_robot->fix_to_world();
    arm_robot->set_position_enforced(true);

    // add a PD-controller to the arm
    // set desired positions
    Eigen::VectorXd ctrl(4);
    ctrl << 0.0, 1.0, -1.5, 1.0;
    // add the controller to the robot
    arm_robot->add_controller(std::make_shared<robot_dart::control::PDControl>(ctrl));

    // add the arm to the simulator
    simu.add_robot(arm_robot);

    // run the simulator for 5 seconds
    simu.run(10.);
    return 0;
}