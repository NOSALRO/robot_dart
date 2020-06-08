#include <iostream>
#include <robot_dart/robot_dart_simu.hpp>

#ifdef GRAPHIC
#include <robot_dart/gui/magnum/graphics.hpp>
#endif

int main()
{
    std::srand(std::time(NULL));
    auto global_robot = std::make_shared<robot_dart::Robot>("pexod.urdf");

    global_robot->set_position_enforced(true);

    global_robot->set_actuator_types(dart::dynamics::Joint::SERVO);
    global_robot->skeleton()->enableSelfCollisionCheck();

    auto g_robot = global_robot->clone();
    g_robot->skeleton()->setPosition(5, 0.2);

    robot_dart::RobotDARTSimu simu(0.001);
#ifdef GRAPHIC
    auto graphics = std::make_shared<robot_dart::gui::magnum::Graphics>(&simu);
    simu.set_graphics(graphics);
    graphics->look_at({0.5, 3., 0.75}, {0.5, 0., 0.2});
#endif
    simu.add_floor();
    simu.add_robot(g_robot);
    simu.run(10.);

    std::cout << g_robot->skeleton()->getPositions().head(6).tail(3).transpose() << std::endl;

    g_robot.reset();
    global_robot.reset();
    return 0;
}
