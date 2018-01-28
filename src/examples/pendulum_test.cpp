#include <cstdlib>
#include <iostream>
#include <robot_dart/robot_dart_simu.hpp>
#include <robot_dart/simple_control.hpp>

#ifdef GRAPHIC
#include <robot_dart/graphics.hpp>
#endif

struct StateDesc : public robot_dart::BaseDescriptor {
    StateDesc(const robot_dart::RobotDARTSimu& simu, size_t desc_dump = 1) : BaseDescriptor(simu, desc_dump) {}

    void operator()()
    {
        if (_simu.robots().size() > 0) {
            states.push_back(_simu.robots()[0]->skeleton()->getPositions());
        }
    }

    std::vector<Eigen::VectorXd> states;
};

int main()
{
    // std::vector<robot_dart::RobotDamage> brk = {};
    // Examples of damages
    // robot_dart::RobotDamage dmg;
    // dmg.type = "blocked_joint";
    // dmg.data = "arm_joint_4";
    // dmg.extra = new double(1.0);
    // brk.push_back(dmg);
    // dmg.type = "blocked_joint";
    // dmg.data = "arm_joint_2";
    // dmg.extra = nullptr;
    // brk.push_back(dmg);
    // dmg.type = "blocked_joint";
    // dmg.data = "arm_joint_3";
    // brk.push_back(dmg);

    auto global_robot = std::make_shared<robot_dart::Robot>("res/models/pendulum.urdf");
    auto g_robot = global_robot->clone();
    g_robot->fix_to_world();
    g_robot->set_position_enforced(false);
    g_robot->skeleton()->setPosition(0, M_PI);
    Eigen::Vector3d size(0.0402, 0.05, 1);

    std::vector<double> ctrl;
    ctrl = {0.0};

    g_robot->add_controller(std::make_shared<robot_dart::SimpleControl>(ctrl));
    ctrl = {-1.0};
    g_robot->add_controller(std::make_shared<robot_dart::SimpleControl>(ctrl), 5.);

    robot_dart::RobotDARTSimu simu;
#ifdef GRAPHIC
    simu.set_graphics(std::make_shared<robot_dart::Graphics>(simu.world()));
#endif
    // <Type>(desc_period)
    simu.add_descriptor<StateDesc>(2);
    simu.add_robot(g_robot);
    std::cout << (g_robot->body_trans("pendulum_link_1") * size).transpose() << std::endl;
    simu.run(1);
    // std::cout << simu.energy() << std::endl;
    std::cout << (g_robot->body_trans("pendulum_link_1") * size).transpose() << std::endl;
    //     std::cout << g_robot->end_effector_pos().transpose() << std::endl;
    ctrl = {2.5};
    g_robot->controllers()[0]->set_parameters(ctrl);
    simu.run(0.5);
    // std::cout << simu.energy() << std::endl;
    std::cout << (g_robot->body_trans("pendulum_link_1") * size).transpose() << std::endl;
    //     std::cout << g_robot->end_effector_pos().transpose() << std::endl;

    std::cout << std::static_pointer_cast<StateDesc>(simu.descriptor(0))->states.size() << std::endl;

    global_robot.reset();
    g_robot.reset();
    return 0;
}
