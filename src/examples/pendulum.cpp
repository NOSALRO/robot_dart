#include <cstdlib>
#include <iostream>
#include <robot_dart/control/simple_control.hpp>
#include <robot_dart/robot_dart_simu.hpp>

#ifdef GRAPHIC
#include <robot_dart/gui/magnum/graphics.hpp>
#endif

struct StateDesc : public robot_dart::descriptor::BaseDescriptor {
    StateDesc(size_t desc_dump = 1) : robot_dart::descriptor::BaseDescriptor(desc_dump) {}

    void operator()()
    {
        if (_simu->robots().size() > 0) {
            states.push_back(_simu->robots()[0]->positions());
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

    auto robot = std::make_shared<robot_dart::Robot>("pendulum.urdf");
    robot->fix_to_world();
    robot->set_position_enforced(false);
    robot->set_positions(robot_dart::make_vector({M_PI}));

    Eigen::VectorXd ctrl(1);
    ctrl << 0.0;

    auto controller1 = std::make_shared<robot_dart::control::SimpleControl>(ctrl);
    robot->add_controller(controller1);
    ctrl << -1.0;
    auto controller2 = std::make_shared<robot_dart::control::SimpleControl>(ctrl);
    robot->add_controller(controller2, 5.);

    robot_dart::RobotDARTSimu simu;
#ifdef GRAPHIC
    simu.set_graphics(std::make_shared<robot_dart::gui::magnum::Graphics>());
#endif
    // <Type>(desc_period)
    simu.add_descriptor<StateDesc>(2);
    simu.add_robot(robot);

    Eigen::Vector3d size(0.0402, 0.05, 1);
    std::cout << (robot->body_pose("pendulum_link_1") * size).transpose() << std::endl;
    simu.run(2.5);
    std::cout << (robot->body_pose("pendulum_link_1") * size).transpose() << std::endl;
    ctrl << 2.5;
    controller1->set_parameters(ctrl);
    simu.run(2.5);
    std::cout << (robot->body_pose("pendulum_link_1") * size).transpose() << std::endl;

    std::cout << std::static_pointer_cast<StateDesc>(simu.descriptor(0))->states.size() << std::endl;

    robot.reset();
    return 0;
}
