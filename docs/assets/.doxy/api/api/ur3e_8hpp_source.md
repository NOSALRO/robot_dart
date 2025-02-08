

# File ur3e.hpp

[**File List**](files.md) **>** [**robot\_dart**](dir_166284c5f0440000a6384365f2a45567.md) **>** [**robots**](dir_087fbdcd93b501a5d3f98df93e9f8cc4.md) **>** [**ur3e.hpp**](ur3e_8hpp.md)

[Go to the documentation of this file](ur3e_8hpp.md)


```C++
#ifndef ROBOT_DART_ROBOTS_UR3E_HPP
#define ROBOT_DART_ROBOTS_UR3E_HPP

#include "robot_dart/robot.hpp"
#include "robot_dart/sensor/force_torque.hpp"

namespace robot_dart {
    namespace robots {
        class Ur3e : public Robot {
        public:
            Ur3e(size_t frequency = 1000, const std::string& urdf = "ur3e/ur3e.urdf", const std::vector<std::pair<std::string, std::string>>& packages = {{"ur3e_description", "ur3e/ur3e_description"}});

            const sensor::ForceTorque& ft_wrist() const { return *_ft_wrist; }

        protected:
            std::shared_ptr<sensor::ForceTorque> _ft_wrist;
            void _post_addition(RobotDARTSimu* simu) override;
            void _post_removal(RobotDARTSimu* simu) override;
        };

        class Ur3eHand : public Ur3e {
        public:
            Ur3eHand(size_t frequency = 1000, const std::string& urdf = "ur3e/ur3e_with_schunk_hand.urdf", const std::vector<std::pair<std::string, std::string>>& packages = {{"ur3e_description", "ur3e/ur3e_description"}}) : Ur3e(frequency, urdf, packages) {}
        };
    } // namespace robots
} // namespace robot_dart
#endif
```


