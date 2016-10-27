#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <robot_dart/robot_dart_simu.hpp>
#include <robot_dart/force_control.hpp>

#ifdef GRAPHIC
#include <robot_dart/graphics.hpp>

struct Params {
    struct graphics : robot_dart::defaults::graphics {
    };
};
#endif

int main()
{
    std::srand(std::time(NULL));
    auto global_robot = std::make_shared<robot_dart::Robot>("res/models/arm.urdf");
    auto g_robot = global_robot->clone();
    g_robot->fix_to_world();
    g_robot->set_position_enforced(true);
    // g_robot->skeleton()->setPosition(1, M_PI / 2.0);
    Eigen::Vector3d size(0, 0, 0);

    std::vector<double> ctrl;
    ctrl = {0.0, 0.0, 0.0, 0.0};

#ifdef GRAPHIC
    robot_dart::RobotDARTSimu<robot_dart::robot_control<robot_dart::ForceControl>, robot_dart::graph<robot_dart::graphics::Graphics<Params>>> simu(ctrl, g_robot);
#else
    robot_dart::RobotDARTSimu<robot_dart::robot_control<robot_dart::ForceControl>> simu(ctrl, g_robot);
#endif
    std::cout << (g_robot->body_trans("arm_link_5") * size).transpose() << std::endl;
    simu.run(1);
    // std::cout << simu.energy() << std::endl;
    std::cout << (g_robot->body_trans("arm_link_5") * size).transpose() << std::endl;
    ctrl = {0.0, 0.0, 0.0, 1.0};
    simu.controller().set_parameters(ctrl);
    simu.run(5);
    // std::cout << simu.energy() << std::endl;
    std::cout << (g_robot->body_trans("arm_link_5") * size).transpose() << std::endl;

    global_robot.reset();
    g_robot.reset();
    return 0;
}
