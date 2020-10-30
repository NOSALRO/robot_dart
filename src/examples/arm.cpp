#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <robot_dart/control/pd_control.hpp>
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
    std::srand(std::time(NULL));
    auto robot = std::make_shared<robot_dart::Robot>("arm.urdf");

    robot->fix_to_world();
    robot->set_position_enforced(true);

    robot->set_actuator_types("velocity");

    Eigen::VectorXd ctrl = robot_dart::make_vector({0.0, 1.0, -1.5, 1.0});

    auto controller = std::make_shared<robot_dart::control::PDControl>(ctrl);
    robot->add_controller(controller);

    robot_dart::RobotDARTSimu simu;
#ifdef GRAPHIC
    simu.set_graphics(std::make_shared<robot_dart::gui::magnum::Graphics>());
#endif
    auto state_desc = std::make_shared<StateDesc>();
    simu.add_descriptor(state_desc);
    simu.add_robot(robot);
    std::cout << robot->body_pose("arm_link_5").translation().transpose() << std::endl;

    simu.run(2.5);
    std::cout << robot->body_pose("arm_link_5").translation().transpose() << std::endl;

    ctrl << 0.0, -1.0, 1.5, -1.0;
    controller->set_parameters(ctrl);
    controller->set_pd(20., 0.);
    simu.run(2.5);
    std::cout << robot->body_pose("arm_link_5").translation().transpose() << std::endl;

    std::cout << state_desc->states.size() << std::endl;

    robot.reset();
    return 0;
}
