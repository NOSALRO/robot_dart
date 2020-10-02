
#include "robot_dart/robot_dart_simu.hpp"
#include "robot_dart/robots/icub.hpp"

namespace robot_dart {
    namespace robots {
        ICub::ICub(RobotDARTSimu* simu, size_t frequency,
            const std::string& urdf, const std::vector<std::pair<std::string, std::string>>& packages)
            : Robot(urdf, packages),
              _imu(std::make_shared<sensor::IMU>(simu, sensor::IMUConfig(body_node("head"), frequency))),
              _ft_foot_left(std::make_shared<sensor::ForceTorque>(simu, joint("l_ankle_roll"), frequency)),
              _ft_foot_right(std::make_shared<sensor::ForceTorque>(simu, joint("r_ankle_roll"), frequency))
        {
            simu->add_sensor(_imu);
            simu->add_sensor(_ft_foot_left);
            simu->add_sensor(_ft_foot_right);

            set_color_mode("material");
            set_color_mode("aspect", "right_foot");
            set_color_mode("aspect", "left_foot");

            set_position_enforced(true);
            skeleton()->setPosition(5, 0.5);
            skeleton()->setPosition(2, 1.57);
        }
    } // namespace robots
} // namespace robot_dart
