#include <robot_dart/control/pd_control.hpp>
#include <robot_dart/robot_dart_simu.hpp>
#include <robot_dart/robots/iiwa.hpp>

int main()
{
    robot_dart::RobotDARTSimu simu(0.001);
    auto robot = std::make_shared<robot_dart::robots::Iiwa>();
    robot->set_actuator_types("servo");
    simu.add_robot(robot);

    //@SET_COLLISION_DETECTOR@
    simu.set_collision_detector("fcl"); // collision_detector can be "dart", "fcl", "ode" or "bullet" (case does not matter)
    //@SET_COLLISION_DETECTOR_END@

    // check if self-collision is enabled
    // @SELF_COLLISIONS@
    if (!robot->self_colliding()) {
        std::cout << "Self collision is not enabled" << std::endl;
        // set self collisions to true and adjacent collisions to false
        robot->set_self_collision(true, false);
    }
    // @SELF_COLLISIONS_END@

    return 0;
}