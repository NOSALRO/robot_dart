#ifndef ROBOT_DART_ROBOTS_HEXAPOD_HPP
#define ROBOT_DART_ROBOTS_HEXAPOD_HPP

#include "robot_dart/robot.hpp"
#include "robot_dart/sensor/force_torque.hpp"

namespace robot_dart {
    namespace robots {
        class Hexapod : public Robot {
        public:
            Hexapod(size_t frequency = 1000, const std::string& urdf = "pexod.urdf") : Robot(urdf)
            {
                set_position_enforced(true);
                skeleton()->enableSelfCollisionCheck();
                set_base_pose(robot_dart::make_vector({0., 0., 0., 0., 0., 0.2}));
            }
        };
    } // namespace robots
} // namespace robot_dart
#endif
