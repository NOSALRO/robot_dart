#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <robot_dart/control/pd_control.hpp>
#include <robot_dart/robot_dart_simu.hpp>

#include <dart/collision/fcl/FCLCollisionDetector.hpp>
#include <dart/constraint/ConstraintSolver.hpp>

#ifdef GRAPHIC
#include <robot_dart/gui/magnum/graphics.hpp>
#endif

int main()
{
    std::srand(std::time(NULL));

    std::vector<std::pair<std::string, std::string>> packages = {{"talos_description", "/Users/jmouret/git/resibots/robot_dart/talos_robot/talos_description/"}};
    auto global_robot = std::make_shared<robot_dart::Robot>("talos.urdf", packages);

    global_robot->set_position_enforced(true);
    global_robot->skeleton()->setPosition(5, 1.2);
    global_robot->skeleton()->setPosition(2, 1.57);

    // Set actuator types to VELOCITY motors so that they stay in position without any controller
    global_robot->set_actuator_types(dart::dynamics::Joint::VELOCITY);
    // First 6-DOFs should always be FORCE if robot is floating base
    for (size_t i = 0; i < 6; i++)
        global_robot->set_actuator_type(i, dart::dynamics::Joint::FORCE);

    robot_dart::RobotDARTSimu simu(0.001);
    simu.world()->getConstraintSolver()->setCollisionDetector(dart::collision::FCLCollisionDetector::create());
#ifdef GRAPHIC
    auto graphics = std::make_shared<robot_dart::gui::magnum::Graphics<>>(simu.world());
    simu.set_graphics(graphics);
    graphics->enable_shadows(false);
    graphics->look_at({0., 3.5, 2.}, {0., 0., 0.25});
#endif
    simu.add_checkerboard_floor();
    simu.add_robot(global_robot);
    simu.run(100.);

    global_robot.reset();
    return 0;
}
