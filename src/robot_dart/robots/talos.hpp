#ifndef ROBOT_DART_ROBOTS_TALOS_HPP
#define ROBOT_DART_ROBOTS_TALOS_HPP

#include "robot_dart/robot.hpp"
#include "robot_dart/sensor/force_torque.hpp"
#include "robot_dart/sensor/imu.hpp"

namespace robot_dart {
    namespace robots {
        class Talos : public Robot {
        public:
            Talos(RobotDARTSimu* simu, size_t frequency = 1000, const std::string& urdf = "talos/talos.urdf", const std::vector<std::pair<std::string, std::string>>& packages = {{"talos_description", "talos/talos_description"}});

            const std::shared_ptr<sensor::IMU>& imu() const { return _imu; }
            const std::shared_ptr<sensor::ForceTorque>& ft_foot_left() const { return _ft_foot_left; }
            const std::shared_ptr<sensor::ForceTorque>& ft_foot_right() const { return _ft_foot_right; }
            const std::shared_ptr<sensor::ForceTorque>& ft_wrist_left() const { return _ft_wrist_left; }
            const std::shared_ptr<sensor::ForceTorque>&  ft_wrist_right() const { return _ft_wrist_right; }

        protected:
            std::shared_ptr<sensor::IMU> _imu;
            std::shared_ptr<sensor::ForceTorque> _ft_foot_left;
            std::shared_ptr<sensor::ForceTorque> _ft_foot_right;
            std::shared_ptr<sensor::ForceTorque> _ft_wrist_left;
            std::shared_ptr<sensor::ForceTorque> _ft_wrist_right;
        };
    } // namespace robots
}
#endif
