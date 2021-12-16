#include <robot_dart/control/pd_control.hpp>
#include <robot_dart/robot_dart_simu.hpp>
#include <robot_dart/robots/iiwa.hpp>

#ifdef GRAPHIC
#include <robot_dart/gui/magnum/graphics.hpp>
#endif

int main()
{
    auto robot = std::make_shared<robot_dart::robots::Iiwa>();

    Eigen::VectorXd ctrl = robot_dart::make_vector({0., M_PI / 3., 0., -M_PI / 4., 0., 0., 0.});

    auto controller = std::make_shared<robot_dart::control::PDControl>(ctrl);
    robot->add_controller(controller);
    controller->set_pd(300., 50.);

    // Add a ghost robot; only visuals, no dynamics, no collision
    auto ghost = robot->clone_ghost();

    robot_dart::RobotDARTSimu simu(0.001);
    simu.set_collision_detector("fcl");
#ifdef GRAPHIC
    auto graphics = std::make_shared<robot_dart::gui::magnum::Graphics>();
    simu.set_graphics(graphics);

    graphics->look_at({0., 3.5, 2.}, {0., 0., 0.25});
#endif
    simu.add_checkerboard_floor();
    simu.add_robot(robot);
    simu.add_robot(ghost);
    simu.set_text_panel("IIWA simulation");
    simu.run(20.);

    robot.reset();
    return 0;
}