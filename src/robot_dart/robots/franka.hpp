#ifndef ROBOT_DART_ROBOTS_FRANKA_HPP
#define ROBOT_DART_ROBOTS_FRANKA_HPP

#include "robot_dart/robot.hpp"
#include "robot_dart/sensor/force_torque.hpp"

namespace robot_dart {
    namespace robots {
        class Franka : public Robot {
        public:
            Franka(size_t frequency = 1000, const std::string& urdf = "franka/franka.urdf", const std::vector<std::pair<std::string, std::string>>& packages = {{"franka_description", "franka/franka_description"}});

            const sensor::ForceTorque& ft_wrist() const { return *_ft_wrist; }

        protected:
            std::shared_ptr<sensor::ForceTorque> _ft_wrist;
            void _post_addition(RobotDARTSimu* simu) override;
            void _post_removal(RobotDARTSimu* simu) override;
        };
    } // namespace robots
} // namespace robot_dart
#endif
