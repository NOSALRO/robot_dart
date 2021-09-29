
#include "robot_dart/robots/talos.hpp"
#include "robot_dart/robot_dart_simu.hpp"

namespace robot_dart {
    namespace robots {
        Talos::Talos(size_t frequency, const std::string& urdf, const std::vector<std::pair<std::string, std::string>>& packages)
            : Robot(urdf, packages),
              _imu(std::make_shared<sensor::IMU>(sensor::IMUConfig(body_node("imu_link"), frequency))),
              _ft_foot_left(std::make_shared<sensor::ForceTorque>(joint("leg_left_6_joint"), frequency)),
              _ft_foot_right(std::make_shared<sensor::ForceTorque>(joint("leg_right_6_joint"), frequency)),
              _ft_wrist_left(std::make_shared<sensor::ForceTorque>(joint("wrist_left_ft_joint"), frequency)),
              _ft_wrist_right(std::make_shared<sensor::ForceTorque>(joint("wrist_right_ft_joint"), frequency)),
              _frequency(frequency)
        {
            // use position/torque limits
            set_position_enforced(true);

            // set a position abobe the floor
            skeleton()->setPosition(5, 1.1);

            // rotate the robot
            skeleton()->setPosition(2, 1.57);
        }

        void Talos::_post_addition(RobotDARTSimu* simu)
        {
            // We do not want to add sensors if we are a ghost robot
            if (ghost())
                return;
            simu->add_sensor(_imu);

            simu->add_sensor(_ft_foot_left);
            simu->add_sensor(_ft_foot_right);
            simu->add_sensor(_ft_wrist_left);
            simu->add_sensor(_ft_wrist_right);

            // torques sensors
            std::vector<std::string> joints = {
                // torso
                "torso_1_joint",
                "torso_2_joint",
                // arm_left
                "arm_left_1_joint", "arm_left_2_joint",
                "arm_left_3_joint", "arm_left_4_joint",
                // arm_right
                "arm_right_1_joint", "arm_right_2_joint",
                "arm_right_3_joint", "arm_right_4_joint",
                // leg_left
                "leg_left_1_joint", "leg_left_2_joint", "leg_left_3_joint",
                "leg_left_4_joint", "leg_left_5_joint", "leg_left_6_joint",
                // leg_right
                "leg_right_1_joint", "leg_right_2_joint", "leg_right_3_joint",
                "leg_right_4_joint", "leg_right_5_joint", "leg_right_6_joint"

            };
            for (auto& s : joints) {
                auto t = std::make_shared<sensor::Torque>(joint(s), _frequency);
                _torques[s] = t;
                simu->add_sensor(t);
            }
        }

        void Talos::_post_removal(RobotDARTSimu* simu)
        {
            simu->remove_sensor(_imu);

            simu->remove_sensor(_ft_foot_left);
            simu->remove_sensor(_ft_foot_right);
            simu->remove_sensor(_ft_wrist_left);
            simu->remove_sensor(_ft_wrist_right);

            for (auto& t : _torques) {
                simu->remove_sensor(t.second);
            }
        }
    } // namespace robots
} // namespace robot_dart
