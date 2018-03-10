#include <iostream>
#include <robot_dart/control/pd_control.hpp>
#include <robot_dart/robot_dart_simu.hpp>

#ifdef GRAPHIC
#include <robot_dart/graphics/graphics.hpp>
#endif

std::shared_ptr<robot_dart::Robot> random_box(size_t num = 0)
{
    // random pose
    Eigen::Vector6d pose = Eigen::Vector6d::Random();
    // make sure it is above the ground
    pose(5) += 1.5;
    // random size
    Eigen::Vector3d size = Eigen::Vector3d::Random().array() * Eigen::Vector3d(0.1, 0.2, 0.1).array() + 0.3;
    return robot_dart::Robot::create_box(size, pose, "free", 1., dart::Color::Red(1.0), std::to_string(num));
}

int main()
{
    std::srand(std::time(NULL));
    robot_dart::RobotDARTSimu simu(0.001);
#ifdef GRAPHIC
    simu.set_graphics(std::make_shared<robot_dart::graphics::Graphics>(simu.world()));
    std::static_pointer_cast<robot_dart::graphics::Graphics>(simu.graphics())->look_at({0., 3., 1.});
#endif

    // add floor
    simu.add_floor(10., 0.2);

    // add random objects
    for (size_t i = 0; i < 10; i++)
        simu.add_robot(random_box(i));

    // add a simple arm
    auto arm_robot = std::make_shared<robot_dart::Robot>("res/models/arm.urdf");
    arm_robot->fix_to_world();
    arm_robot->set_position_enforced(true);

    // add a PD-controller to the arm
    std::vector<double> ctrl = {0.0, 1.0, -1.5, 1.0};
    arm_robot->add_controller(std::make_shared<robot_dart::control::PDControl>(ctrl));

    simu.add_robot(arm_robot);

    simu.run(5.);
    return 0;
}