#include "robot_dart/robots/icub.hpp"
#include "robot_dart/robot_dart_simu.hpp"

namespace robot_dart {
    namespace robots {
        ICub::ICub(size_t frequency, const std::string& urdf, const std::vector<std::pair<std::string, std::string>>& packages)
            : Robot(urdf, packages),
              _imu(std::make_shared<sensor::IMU>(sensor::IMUConfig(body_node("head"), frequency))),
              _ft_foot_left(std::make_shared<sensor::ForceTorque>(joint("l_ankle_roll"), frequency)),
              _ft_foot_right(std::make_shared<sensor::ForceTorque>(joint("r_ankle_roll"), frequency))
        {
            set_color_mode("material");

            set_position_enforced(true);

            // position iCub
            set_base_pose(robot_dart::make_vector({0., 0., M_PI / 2., 0., 0., 0.46}));
        }

        void ICub::reset()
        {
            Robot::reset();

            // position iCub
            set_base_pose(robot_dart::make_vector({0., 0., M_PI / 2., 0., 0., 0.46}));
        }

        void ICub::_post_addition(RobotDARTSimu* simu)
        {
            // We do not want to add sensors if we are a ghost robot
            if (ghost())
                return;
            simu->add_sensor(_imu);
            simu->add_sensor(_ft_foot_left);
            simu->add_sensor(_ft_foot_right);
        }

        void ICub::_post_removal(RobotDARTSimu* simu)
        {
            simu->remove_sensor(_imu);
            simu->remove_sensor(_ft_foot_left);
            simu->remove_sensor(_ft_foot_right);
        }
    } // namespace robots
} // namespace robot_dart
