#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <robot_dart/pd_control.hpp>
#include <robot_dart/robot_dart_simu.hpp>

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

    global_robot->fix_to_world();
    global_robot->set_position_enforced(true);
    // g_robot->skeleton()->setPosition(1, M_PI / 2.0);
    Eigen::Vector3d size(0, 0, 0);

    global_robot->set_actuator_types(dart::dynamics::Joint::VELOCITY);

    std::vector<double> ctrl;
    ctrl = {0.0, 1.0, -1.5, 1.0};

    global_robot->add_controller(std::make_shared<robot_dart::PDControl>(ctrl));

    auto g_robot = global_robot->clone();

#ifdef GRAPHIC
    robot_dart::RobotDARTSimu<robot_dart::graphics<robot_dart::Graphics<Params>>> simu(0.001);
#else
    robot_dart::RobotDARTSimu<> simu(0.001);
#endif
    simu.add_robot(g_robot);
    std::cout << (g_robot->body_trans("arm_link_5") * size).transpose() << std::endl;
    simu.run(2);
    // std::cout << simu.energy() << std::endl;
    std::cout << (g_robot->body_trans("arm_link_5") * size).transpose() << std::endl;
    ctrl = {0.0, -1.0, 1.5, -1.0};
    g_robot->controllers()[0]->set_parameters(ctrl);
    std::static_pointer_cast<robot_dart::PDControl>(g_robot->controllers()[0])->set_pd(20., 0.);
    simu.run(2);
    // std::cout << simu.energy() << std::endl;
    std::cout << (g_robot->body_trans("arm_link_5") * size).transpose() << std::endl;

    global_robot.reset();
    g_robot.reset();
    return 0;
}
