
#include "robot_dart/robots/talos.hpp"
#include "robot_dart/robot_dart_simu.hpp"

namespace robot_dart {
    namespace robots {
        Talos::Talos(RobotDARTSimu* simu, size_t frequency, const std::string& urdf, const std::vector<std::pair<std::string, std::string>>& packages)
            : Robot(urdf, packages),
              _imu(std::make_shared<sensor::IMU>(sensor::IMUConfig(body_node("imu_link"), frequency))),
              _ft_foot_left(std::make_shared<sensor::ForceTorque>(joint("leg_left_6_joint"), frequency)),
              _ft_foot_right(std::make_shared<sensor::ForceTorque>(joint("leg_right_6_joint"), frequency)),
              _ft_wrist_left(std::make_shared<sensor::ForceTorque>(joint("wrist_left_ft_joint"), frequency)),
              _ft_wrist_right(std::make_shared<sensor::ForceTorque>(joint("wrist_right_ft_joint"), frequency))
        {
            simu->add_sensor(_imu);
            simu->add_sensor(_ft_foot_left);
            simu->add_sensor(_ft_foot_right);
            simu->add_sensor(_ft_wrist_left);
            simu->add_sensor(_ft_wrist_right);
            // TODO torque sensors

            set_position_enforced(true);
            skeleton()->setPosition(5, 1.1);
            skeleton()->setPosition(2, 1.57);
        }
    } // namespace robots
} // namespace robot_dart
