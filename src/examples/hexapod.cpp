#include <iostream>
#include <robot_dart/robot_dart_simu.hpp>

#include <robot_dart/control/hexa_control.hpp>

#ifdef GRAPHIC
#include <robot_dart/graphics/graphics.hpp>
#endif

#include <dart/collision/bullet/BulletCollisionDetector.hpp>
#include <dart/constraint/ConstraintSolver.hpp>

int main()
{
    std::srand(std::time(NULL));
    auto global_robot = std::make_shared<robot_dart::Robot>("res/models/pexod.urdf");

    global_robot->set_position_enforced(true);

    global_robot->set_actuator_types(dart::dynamics::Joint::SERVO);
    global_robot->skeleton()->enableSelfCollisionCheck();

    auto g_robot = global_robot->clone();
    g_robot->skeleton()->setPosition(5, 0.15);

    // std::vector<double> ctrl(18, 0.);
    // g_robot->add_controller(std::make_shared<robot_dart::SimpleControl>());
    std::vector<double> ctrl = {1, 0, 0.5, 0.25, 0.25, 0.5, 1, 0.5, 0.5, 0.25, 0.75, 0.5, 1, 0, 0.5, 0.25, 0.25, 0.5, 1, 0, 0.5, 0.25, 0.75, 0.5, 1, 0.5, 0.5, 0.25, 0.25, 0.5, 1, 0, 0.5, 0.25, 0.75, 0.5};

    double ctrl_dt = 0.015;
    g_robot->add_controller(std::make_shared<robot_dart::control::HexaControl>(ctrl_dt, ctrl));
    std::static_pointer_cast<robot_dart::control::HexaControl>(g_robot->controller(0))->set_h_params(std::vector<double>(1, ctrl_dt));

    robot_dart::RobotDARTSimu simu(0.001);
#ifdef GRAPHIC
    simu.set_graphics(std::make_shared<robot_dart::graphics::Graphics>(simu.world()));
    std::static_pointer_cast<robot_dart::graphics::Graphics>(simu.graphics())->look_at({0.5, 3., 0.75}, {0.5, 0., 0.2});
    std::static_pointer_cast<robot_dart::graphics::Graphics>(simu.graphics())->add_grid(0.25,20); // adds a 20x20 grid with 25cm tiles 
#endif
    simu.world()->getConstraintSolver()->setCollisionDetector(dart::collision::BulletCollisionDetector::create());
    simu.add_floor();
    simu.add_robot(g_robot);
    simu.run(3);

    std::cout << g_robot->skeleton()->getPositions().head(6).tail(3).transpose() << std::endl;

    g_robot.reset();
    global_robot.reset();
    return 0;
}
