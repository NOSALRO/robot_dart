#ifndef ROBOT_DART_ROBOTS_PENDULUM_HPP
#define ROBOT_DART_ROBOTS_PENDULUM_HPP

#include "robot_dart/robot.hpp"
#include "robot_dart/sensor/force_torque.hpp"

namespace robot_dart {
    namespace robots {
        class Pendulum : public Robot {
        public:
            Pendulum(const std::string& urdf = "pendulum.urdf") : Robot(urdf)
            {
                fix_to_world();
                set_position_enforced(true);
                set_positions(robot_dart::make_vector({M_PI}));
            }
        };
    } // namespace robots
} // namespace robot_dart
#endif
