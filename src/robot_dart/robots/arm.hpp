#ifndef ROBOT_DART_ROBOTS_ARM_HPP
#define ROBOT_DART_ROBOTS_ARM_HPP

#include "robot_dart/robot.hpp"
#include "robot_dart/sensor/force_torque.hpp"

namespace robot_dart {
    namespace robots {
        class Arm : public Robot {
        public:
            Arm(const std::string& urdf = "arm.urdf") : Robot(urdf)
            {
                fix_to_world();
                set_position_enforced(true);
            }
        };
    } // namespace robots
} // namespace robot_dart
#endif
