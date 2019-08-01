#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <robot_dart/control/pd_control.hpp>
#include <robot_dart/robot_dart_simu.hpp>

#include <dart/collision/fcl/FCLCollisionDetector.hpp>
#include <dart/constraint/ConstraintSolver.hpp>

#ifdef GRAPHIC
#include <robot_dart/graphics/graphics.hpp>
#endif

int main()
{
    std::srand(std::time(NULL));

    std::vector<std::pair<std::string, std::string>> packages = {{"iiwa14", std::string(RESPATH) + "/models/meshes"}};
    auto global_robot = std::make_shared<robot_dart::Robot>("res/models/iiwa14.urdf", packages);

    global_robot->fix_to_world();
    global_robot->set_position_enforced(true);

    std::vector<double> ctrl;
    ctrl = {0., M_PI / 3., 0., -M_PI / 4., 0., 0., 0.};

    global_robot->add_controller(std::make_shared<robot_dart::control::PDControl>(ctrl));
    std::static_pointer_cast<robot_dart::control::PDControl>(global_robot->controllers()[0])->set_pd(300., 50.);

    robot_dart::RobotDARTSimu simu(0.001);
    simu.world()->getConstraintSolver()->setCollisionDetector(dart::collision::FCLCollisionDetector::create());
#ifdef GRAPHIC
    simu.set_graphics(std::make_shared<robot_dart::graphics::Graphics>(simu.world()));
    std::static_pointer_cast<robot_dart::graphics::Graphics>(simu.graphics())->look_at({0., 3.5, 2.}, {0., 0., 0.25});
#if DART_VERSION_AT_LEAST(6, 8, 0) // GridShape for OSG is available only for DART version >=6.8.0
    std::static_pointer_cast<robot_dart::graphics::Graphics>(simu.graphics())->add_grid(0.25, 20); // adds a 20x20 grid with 25cm tiles
#endif
#endif
    simu.add_robot(global_robot);
    simu.run(20.);

    global_robot.reset();
    return 0;
}
