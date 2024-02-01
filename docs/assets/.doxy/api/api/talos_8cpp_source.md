

# File talos.cpp

[**File List**](files.md) **>** [**robot\_dart**](dir_166284c5f0440000a6384365f2a45567.md) **>** [**robots**](dir_087fbdcd93b501a5d3f98df93e9f8cc4.md) **>** [**talos.cpp**](talos_8cpp.md)

[Go to the documentation of this file](talos_8cpp.md)

```C++

#include "robot_dart/robots/talos.hpp"
#include "robot_dart/robot_dart_simu.hpp"

namespace robot_dart {
    namespace robots {
        Talos::Talos(size_t frequency, const std::string& urdf, const std::vector<std::pair<std::string, std::string>>& packages)
            : Robot(urdf, packages),
              _imu(std::make_shared<sensor::IMU>(sensor::IMUConfig(body_node("imu_link"), frequency))),
              _ft_foot_left(std::make_shared<sensor::ForceTorque>(joint("leg_left_6_joint"), frequency, "parent_to_child")),
              _ft_foot_right(std::make_shared<sensor::ForceTorque>(joint("leg_right_6_joint"), frequency, "parent_to_child")),
              _ft_wrist_left(std::make_shared<sensor::ForceTorque>(joint("wrist_left_ft_joint"), frequency, "parent_to_child")),
              _ft_wrist_right(std::make_shared<sensor::ForceTorque>(joint("wrist_right_ft_joint"), frequency, "parent_to_child")),
              _frequency(frequency)
        {
            // use position/torque limits
            set_position_enforced(true);

            // position Talos
            set_base_pose(robot_dart::make_vector({0., 0., M_PI / 2., 0., 0., 1.1}));
        }

        void Talos::reset()
        {
            Robot::reset();

            // position Talos
            set_base_pose(robot_dart::make_vector({0., 0., M_PI / 2., 0., 0., 1.1}));
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

        void TalosFastCollision::_post_addition(RobotDARTSimu* simu)
        {
            Talos::_post_addition(simu);
            auto vec = TalosFastCollision::collision_vec();
            for (auto& t : vec) {
                simu->set_collision_masks(simu->robots().size() - 1, std::get<0>(t), std::get<1>(t), std::get<2>(t));
            }
        }

        std::vector<std::tuple<std::string, uint32_t, uint32_t>> TalosFastCollision::collision_vec()
        {
            std::vector<std::tuple<std::string, uint32_t, uint32_t>> vec;
            vec.push_back(std::make_tuple("leg_right_6_link", 0x1, 0x1 | 0x4 | 0x8 | 0x10 | 0x20 | 0x40 | 0x80 | 0x100 | 0x200 | 0x400 | 0x800 | 0x1000 | 0x2000 | 0x4000 | 0x8000 | 0x10000));
            vec.push_back(std::make_tuple("leg_right_4_link", 0x2, 0x2 | 0x4 | 0x8 | 0x10 | 0x20 | 0x40 | 0x80 | 0x100 | 0x200 | 0x400 | 0x800 | 0x1000 | 0x2000 | 0x4000 | 0x8000 | 0x10000));
            vec.push_back(std::make_tuple("leg_left_6_link", 0x4, 0x1 | 0x2 | 0x4 | 0x10 | 0x20 | 0x40 | 0x80 | 0x100 | 0x200 | 0x400 | 0x800 | 0x1000 | 0x2000 | 0x4000 | 0x8000 | 0x10000));
            vec.push_back(std::make_tuple("leg_left_4_link", 0x8, 0x1 | 0x2 | 0x8 | 0x10 | 0x20 | 0x40 | 0x80 | 0x100 | 0x200 | 0x400 | 0x800 | 0x1000 | 0x2000 | 0x4000 | 0x8000 | 0x10000));
            vec.push_back(std::make_tuple("leg_left_1_link", 0x10, 0x1 | 0x2 | 0x4 | 0x8 | 0x10 | 0x40 | 0x80 | 0x100 | 0x200 | 0x400 | 0x800 | 0x1000 | 0x2000 | 0x4000 | 0x8000 | 0x10000));
            vec.push_back(std::make_tuple("leg_left_3_link", 0x20, 0x1 | 0x2 | 0x4 | 0x8 | 0x20 | 0x40 | 0x80 | 0x100 | 0x200 | 0x400 | 0x800 | 0x1000 | 0x2000 | 0x4000 | 0x8000 | 0x10000));
            vec.push_back(std::make_tuple("leg_right_1_link", 0x40, 0x1 | 0x2 | 0x4 | 0x8 | 0x10 | 0x20 | 0x40 | 0x100 | 0x200 | 0x400 | 0x800 | 0x1000 | 0x2000 | 0x4000 | 0x8000 | 0x10000));
            vec.push_back(std::make_tuple("leg_right_3_link", 0x80, 0x1 | 0x2 | 0x4 | 0x8 | 0x10 | 0x20 | 0x80 | 0x100 | 0x200 | 0x400 | 0x800 | 0x1000 | 0x2000 | 0x4000 | 0x8000 | 0x10000));
            vec.push_back(std::make_tuple("arm_left_7_link", 0x100, 0x1 | 0x2 | 0x4 | 0x8 | 0x10 | 0x20 | 0x40 | 0x80 | 0x100 | 0x400 | 0x800 | 0x1000 | 0x2000 | 0x4000 | 0x8000 | 0x10000));
            vec.push_back(std::make_tuple("arm_left_5_link", 0x200, 0x1 | 0x2 | 0x4 | 0x8 | 0x10 | 0x20 | 0x40 | 0x80 | 0x200 | 0x400 | 0x800 | 0x1000 | 0x2000 | 0x4000 | 0x8000 | 0x10000));
            vec.push_back(std::make_tuple("arm_right_7_link", 0x400, 0x1 | 0x2 | 0x4 | 0x8 | 0x10 | 0x20 | 0x40 | 0x80 | 0x100 | 0x200 | 0x400 | 0x1000 | 0x2000 | 0x4000 | 0x8000 | 0x10000));
            vec.push_back(std::make_tuple("arm_right_5_link", 0x800, 0x1 | 0x2 | 0x4 | 0x8 | 0x10 | 0x20 | 0x40 | 0x80 | 0x100 | 0x200 | 0x800 | 0x1000 | 0x2000 | 0x4000 | 0x8000 | 0x10000));
            vec.push_back(std::make_tuple("torso_2_link", 0x1000, 0x1 | 0x2 | 0x4 | 0x8 | 0x10 | 0x20 | 0x40 | 0x80 | 0x100 | 0x200 | 0x800 | 0x1000 | 0x4000 | 0x8000));
            vec.push_back(std::make_tuple("base_link", 0x2000, 0x1 | 0x2 | 0x4 | 0x8 | 0x10 | 0x20 | 0x40 | 0x80 | 0x100 | 0x200 | 0x800 | 0x2000 | 0x10000));
            vec.push_back(std::make_tuple("leg_right_2_link", 0x4000, 0x1 | 0x2 | 0x4 | 0x8 | 0x10 | 0x20 | 0x40 | 0x80 | 0x100 | 0x200 | 0x800 | 0x1000 | 0x4000 | 0x8000 | 0x10000));
            vec.push_back(std::make_tuple("leg_left_2_link", 0x8000, 0x1 | 0x2 | 0x4 | 0x8 | 0x10 | 0x20 | 0x40 | 0x80 | 0x100 | 0x200 | 0x800 | 0x1000 | 0x4000 | 0x8000 | 0x10000));
            vec.push_back(std::make_tuple("head_2_link", 0x10000, 0x1 | 0x2 | 0x4 | 0x8 | 0x10 | 0x20 | 0x40 | 0x80 | 0x100 | 0x200 | 0x800 | 0x2000 | 0x4000 | 0x8000 | 0x10000));
            return vec;
        }
    } // namespace robots
} // namespace robot_dart

```

