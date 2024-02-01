

# File iiwa.hpp

[**File List**](files.md) **>** [**robot\_dart**](dir_166284c5f0440000a6384365f2a45567.md) **>** [**robots**](dir_087fbdcd93b501a5d3f98df93e9f8cc4.md) **>** [**iiwa.hpp**](iiwa_8hpp.md)

[Go to the documentation of this file](iiwa_8hpp.md)

```C++

#ifndef ROBOT_DART_ROBOTS_IIWA_HPP
#define ROBOT_DART_ROBOTS_IIWA_HPP

#include "robot_dart/robot.hpp"
#include "robot_dart/sensor/force_torque.hpp"

namespace robot_dart {
    namespace robots {
        class Iiwa : public Robot {
        public:
            Iiwa(size_t frequency = 1000, const std::string& urdf = "iiwa/iiwa.urdf", const std::vector<std::pair<std::string, std::string>>& packages = {{"iiwa_description", "iiwa/iiwa_description"}});

            const sensor::ForceTorque& ft_wrist() const { return *_ft_wrist; }

        protected:
            std::shared_ptr<sensor::ForceTorque> _ft_wrist;
            void _post_addition(RobotDARTSimu* simu) override;
            void _post_removal(RobotDARTSimu* simu) override;
        };
    } // namespace robots
} // namespace robot_dart
#endif

```

