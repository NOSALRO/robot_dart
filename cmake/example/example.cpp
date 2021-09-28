#include <robot_dart/control/pd_control.hpp>
#include <robot_dart/robot_dart_simu.hpp>

#ifdef GRAPHIC
#include <robot_dart/gui/magnum/graphics.hpp>
#endif

int main()
{
    std::srand(std::time(NULL));
    auto global_robot = std::make_shared<robot_dart::Robot>("arm.urdf");

    global_robot->fix_to_world();
    global_robot->set_position_enforced(true);
    // g_robot->skeleton()->setPosition(1, M_PI / 2.0);
    Eigen::Vector3d size(0, 0, 0);

    global_robot->set_actuator_types("velocity");

    Eigen::VectorXd ctrl(4);
    ctrl << 0.0, 1.0, -1.5, 1.0;

    global_robot->add_controller(std::make_shared<robot_dart::control::PDControl>(ctrl));

    auto g_robot = global_robot->clone();

    robot_dart::RobotDARTSimu simu;
#ifdef GRAPHIC
    simu.set_graphics(std::make_shared<robot_dart::gui::magnum::Graphics>());
#endif
    simu.add_robot(g_robot);
    std::cout << (g_robot->body_pose("arm_link_5") * size).transpose() << std::endl;
    simu.run(2.5);
    std::cout << (g_robot->body_pose("arm_link_5") * size).transpose() << std::endl;
    ctrl << 0.0, -1.0, 1.5, -1.0;
    g_robot->controllers()[0]->set_parameters(ctrl);
    std::static_pointer_cast<robot_dart::control::PDControl>(g_robot->controllers()[0])->set_pd(20., 0.);
    simu.run(2.5);
    std::cout << (g_robot->body_pose("arm_link_5") * size).transpose() << std::endl;

    return 0;
}
