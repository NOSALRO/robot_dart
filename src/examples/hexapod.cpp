#include <iostream>
#include <robot_dart/robot_dart_simu.hpp>

#ifdef GRAPHIC
#include <robot_dart/gui/magnum/graphics.hpp>
#endif

int main()
{
    std::srand(std::time(NULL));
    auto robot = std::make_shared<robot_dart::Robot>("pexod.urdf");

    robot->set_position_enforced(true);

    robot->set_actuator_types("servo");
    robot->skeleton()->enableSelfCollisionCheck();

    robot->set_base_pose(robot_dart::make_vector({0., 0., 0., 0., 0., 0.2}));

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
