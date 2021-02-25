// @HELLO_WORLD_INCLUDE@
#include <robot_dart/robot_dart_simu.hpp>

#ifdef GRAPHIC
#include <robot_dart/gui/magnum/graphics.hpp>
#endif
// @HELLO_WORLD_INCLUDE_END@

int main()
{
    std::srand(std::time(NULL));
    // @HELLO_WORLD_ROBOT_CREATION@
    auto robot = std::make_shared<robot_dart::Robot>("pexod.urdf");
    // @HELLO_WORLD_ROBOT_CREATION_END@
    // @HELLO_WORLD_ROBOT_PLACING@
    robot->set_base_pose(robot_dart::make_tf({0., 0., 0.2}));
    // @HELLO_WORLD_ROBOT_PLACING_END@

    // @HELLO_WORLD_ROBOT_SIMU@
    robot_dart::RobotDARTSimu simu(0.001); // dt=0.001, 1KHz simulation
    simu.add_floor();
    simu.add_robot(robot);
    // @HELLO_WORLD_ROBOT_SIMU_END@

    // @HELLO_WORLD_ROBOT_GRAPHIC@
#ifdef GRAPHIC
    auto graphics = std::make_shared<robot_dart::gui::magnum::Graphics>();
    simu.set_graphics(graphics);
    graphics->look_at({0.5, 3., 0.75}, {0.5, 0., 0.2});
#endif
    // @HELLO_WORLD_ROBOT_GRAPHIC_END@
    // @HELLO_WORLD_ROBOT_RUN@
    simu.run(10.);
    // @HELLO_WORLD_ROBOT_RUN_END@

    return 0;
}
