#ifndef ROBOT_DART_ROBOTS_TIAGO_HPP
#define ROBOT_DART_ROBOTS_TIAGO_HPP

#include "robot_dart/robot.hpp"
#include "robot_dart/sensor/force_torque.hpp"

namespace robot_dart {
    namespace robots {
        /// datasheet: https://pal-robotics.com/wp-content/uploads/2021/07/Datasheet-complete_TIAGo-2021.pdf
        class Tiago : public Robot {
        public:
            Tiago(size_t frequency = 1000, const std::string& urdf = "tiago/tiago_steel.urdf", const std::vector<std::pair<std::string, std::string>>& packages = {{"tiago_description", "tiago/tiago_description"}});

            const sensor::ForceTorque& ft_wrist() const { return *_ft_wrist; }

        protected:
            std::shared_ptr<sensor::ForceTorque> _ft_wrist;
            void _post_addition(RobotDARTSimu* simu) override;
            void _post_removal(RobotDARTSimu* simu) override;
        };
    } // namespace robots
} // namespace robot_dart
#endif
