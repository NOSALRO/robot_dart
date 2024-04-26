

# File pendulum.hpp

[**File List**](files.md) **>** [**robot\_dart**](dir_166284c5f0440000a6384365f2a45567.md) **>** [**robots**](dir_087fbdcd93b501a5d3f98df93e9f8cc4.md) **>** [**pendulum.hpp**](pendulum_8hpp.md)

[Go to the documentation of this file](pendulum_8hpp.md)


```C++
#ifndef ROBOT_DART_ROBOTS_PENDULUM_HPP
#define ROBOT_DART_ROBOTS_PENDULUM_HPP

#include "robot_dart/robot.hpp"

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
```


