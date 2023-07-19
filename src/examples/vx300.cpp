#include <robot_dart/control/pd_control.hpp>
#include <robot_dart/robot_dart_simu.hpp>
#include <robot_dart/robots/vx300.hpp>

#ifdef GRAPHIC
#include <robot_dart/gui/magnum/graphics.hpp>
#endif

int main()
{
    auto robot = std::make_shared<robot_dart::robots::Vx300>();
    robot->set_actuator_types("servo");

    Eigen::VectorXd ctrl = robot_dart::make_vector({0.0, 1.0, -1.5, 1.0, 0.5, 0.});

    auto controller = std::make_shared<robot_dart::control::PDControl>(ctrl);
    robot->add_controller(controller);

    robot_dart::RobotDARTSimu simu;
    simu.set_collision_detector("fcl");
#ifdef GRAPHIC
    simu.set_graphics(std::make_shared<robot_dart::gui::magnum::Graphics>());
#endif
    simu.add_robot(robot);
    simu.add_checkerboard_floor();

    for (auto& n : robot->dof_names()) {
        std::cout << n << std::endl;
    }

    simu.run(2.5);

    ctrl << 0.0, -0.5, 0.5, -0.5, 0., 1.;
    controller->set_parameters(ctrl);
    controller->set_pd(20., 0.);
    simu.run(2.5);

    return 0;
}
