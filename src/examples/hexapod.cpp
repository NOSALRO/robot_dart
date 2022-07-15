#include <robot_dart/robot_dart_simu.hpp>
#include <robot_dart/robots/hexapod.hpp>

#ifdef GRAPHIC
#include <robot_dart/gui/magnum/graphics.hpp>
#endif

int main()
{
    // @HEXAPOD@
    auto robot = std::make_shared<robot_dart::robots::Hexapod>();
    // @HEXAPOD_END@
    robot->set_actuator_types("servo");

    robot_dart::RobotDARTSimu simu(0.001);
#ifdef GRAPHIC
    auto graphics = std::make_shared<robot_dart::gui::magnum::Graphics>();
    simu.set_graphics(graphics);
    graphics->look_at({0.5, 3., 0.75}, {0.5, 0., 0.2});
#endif
    simu.add_floor();
    simu.add_robot(robot);
    simu.run(10.);

    std::cout << robot->base_pose_vec().tail(3).transpose() << std::endl;

    robot.reset();
    return 0;
}
