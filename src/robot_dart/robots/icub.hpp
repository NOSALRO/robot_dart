#ifndef ROBOT_DART_ROBOTS_ICUB_HPP
#define ROBOT_DART_ROBOTS_ICUB_HPP

#include "robot_dart/robot.hpp"
#include "robot_dart/sensor/force_torque.hpp"
#include "robot_dart/sensor/imu.hpp"

namespace robot_dart {
    namespace robots {
        class ICub : public Robot {
        public:
            ICub(size_t frequency = 1000, const std::string& urdf = "icub/icub.urdf", const std::vector<std::pair<std::string, std::string>>& packages = {{"icub_description", "icub/icub_description"}});

            const sensor::IMU& imu() const { return *_imu; }
            const sensor::ForceTorque& ft_foot_left() const { return *_ft_foot_left; }
            const sensor::ForceTorque& ft_foot_right() const { return *_ft_foot_right; }

        protected:
            std::shared_ptr<sensor::IMU> _imu;
            std::shared_ptr<sensor::ForceTorque> _ft_foot_left;
            std::shared_ptr<sensor::ForceTorque> _ft_foot_right;

            void _post_addition(RobotDARTSimu* simu) override;
            void _post_removal(RobotDARTSimu* simu) override;
        };
    } // namespace robots
} // namespace robot_dart
#endif
