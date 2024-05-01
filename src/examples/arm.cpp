#include <robot_dart/control/pd_control.hpp>
#include <robot_dart/robot_dart_simu.hpp>
#include <robot_dart/robots/arm.hpp>

#ifdef GRAPHIC
#include <robot_dart/gui/magnum/graphics.hpp>
#endif

int main()
{
    // @SIMPLE_ARM@
    auto robot = std::make_shared<robot_dart::robots::Arm>();
    // @SIMPLE_ARM_END@
    robot->set_actuator_types("velocity");

    Eigen::VectorXd ctrl = robot_dart::make_vector({0.0, 1.0, -1.5, 1.0});

    auto controller = std::make_shared<robot_dart::control::PDControl>(ctrl);
    robot->add_controller(controller);

    robot_dart::RobotDARTSimu simu;
#ifdef GRAPHIC
    simu.set_graphics(std::make_shared<robot_dart::gui::magnum::Graphics>());
#endif
    simu.add_robot(robot);
    std::cout << robot->body_pose("arm_link_5").translation().transpose() << std::endl;

    simu.run(2.5);
    std::cout << robot->body_pose("arm_link_5").translation().transpose() << std::endl;

    ctrl << 0.0, -1.0, 1.5, -1.0;
    controller->set_parameters(ctrl);
    controller->set_pd(20., 0.);
    simu.run(2.5);
    std::cout << robot->body_pose("arm_link_5").translation().transpose() << std::endl;

    return 0;
}
