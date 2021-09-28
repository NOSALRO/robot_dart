#include <iostream>

#include <robot_dart/robot_dart_simu.hpp>

#ifdef GRAPHIC
#include <robot_dart/gui/magnum/graphics.hpp>
#endif

int main()
{
    std::srand(std::time(NULL));

    std::vector<std::pair<std::string, std::string>> packages = {{"a1_description", "unitree_a1/a1_description"}};
    auto robot = std::make_shared<robot_dart::Robot>("unitree_a1/a1.urdf", packages);
    robot->set_color_mode("material");

    robot->set_self_collision(true);
    robot->set_actuator_types("servo");
    robot->set_position_enforced(true);
    robot->set_base_pose(robot_dart::make_vector({0., 0., 0., 0., 0., 0.5}));

    auto names = robot->dof_names(true, true, true);
    names = std::vector<std::string>(names.begin() + 6, names.end());
    // standing pose
    robot->set_positions(robot_dart::make_vector({0.0, 0.67, -1.3, -0.0, 0.67, -1.3, 0.0, 0.67, -1.3, -0.0, 0.67, -1.3}), names);

    robot_dart::RobotDARTSimu simu(0.001);
    simu.set_collision_detector("fcl");
#ifdef GRAPHIC
    auto graphics = std::make_shared<robot_dart::gui::magnum::Graphics>();
    simu.set_graphics(graphics);

    graphics->look_at({0., 3.5, 2.}, {0., 0., 0.25});
#endif
    simu.add_checkerboard_floor();
    simu.add_robot(robot);
    simu.run(20.);

    robot.reset();
    return 0;
}