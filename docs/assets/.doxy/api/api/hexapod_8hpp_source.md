

# File hexapod.hpp

[**File List**](files.md) **>** [**robot\_dart**](dir_166284c5f0440000a6384365f2a45567.md) **>** [**robots**](dir_087fbdcd93b501a5d3f98df93e9f8cc4.md) **>** [**hexapod.hpp**](hexapod_8hpp.md)

[Go to the documentation of this file](hexapod_8hpp.md)

```C++

#ifndef ROBOT_DART_ROBOTS_HEXAPOD_HPP
#define ROBOT_DART_ROBOTS_HEXAPOD_HPP

#include "robot_dart/robot.hpp"

namespace robot_dart {
    namespace robots {
        class Hexapod : public Robot {
        public:
            Hexapod(const std::string& urdf = "pexod.urdf") : Robot(urdf)
            {
                set_position_enforced(true);
                skeleton()->enableSelfCollisionCheck();
                set_base_pose(robot_dart::make_vector({0., 0., 0., 0., 0., 0.2}));
            }

            void reset() override
            {
                Robot::reset();
                set_base_pose(robot_dart::make_vector({0., 0., 0., 0., 0., 0.2}));
            }
        };
    } // namespace robots
} // namespace robot_dart
#endif

```

