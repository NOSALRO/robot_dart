

# File arm.hpp

[**File List**](files.md) **>** [**robot\_dart**](dir_166284c5f0440000a6384365f2a45567.md) **>** [**robots**](dir_087fbdcd93b501a5d3f98df93e9f8cc4.md) **>** [**arm.hpp**](arm_8hpp.md)

[Go to the documentation of this file](arm_8hpp.md)

```C++

#ifndef ROBOT_DART_ROBOTS_ARM_HPP
#define ROBOT_DART_ROBOTS_ARM_HPP

#include "robot_dart/robot.hpp"

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

```

