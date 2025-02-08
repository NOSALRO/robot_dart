

# File tiago.cpp

[**File List**](files.md) **>** [**robot\_dart**](dir_166284c5f0440000a6384365f2a45567.md) **>** [**robots**](dir_087fbdcd93b501a5d3f98df93e9f8cc4.md) **>** [**tiago.cpp**](tiago_8cpp.md)

[Go to the documentation of this file](tiago_8cpp.md)


```C++
#include "robot_dart/robots/tiago.hpp"
#include "robot_dart/robot_dart_simu.hpp"

namespace robot_dart {
    namespace robots {
        Tiago::Tiago(size_t frequency, const std::string& urdf, const std::vector<std::pair<std::string, std::string>>& packages)
            : Robot(urdf, packages),
              _ft_wrist(std::make_shared<sensor::ForceTorque>(joint("gripper_tool_joint"), frequency))
        {
            set_position_enforced(true);
            // We use servo actuators, but not for the caster joints
            set_actuator_types("servo");

            // position Tiago
            set_base_pose(robot_dart::make_vector({0., 0., M_PI / 2., 0., 0., 0.}));
        }

        void Tiago::reset()
        {
            Robot::reset();

            // position Tiago
            set_base_pose(robot_dart::make_vector({0., 0., M_PI / 2., 0., 0., 0.}));
        }

        void Tiago::set_actuator_types(const std::string& type, const std::vector<std::string>& joint_names, bool override_mimic, bool override_base, bool override_caster)
        {
            auto jt = joint_names;
            if (!override_caster) {
                if (joint_names.size() == 0)
                    jt = Robot::joint_names();
                for (const auto& jnt : caster_joints()) {
                    auto it = std::find(jt.begin(), jt.end(), jnt);
                    if (it != jt.end()) {
                        jt.erase(it);
                    }
                }
            }
            Robot::set_actuator_types(type, jt, override_mimic, override_base);
        }

        void Tiago::set_actuator_type(const std::string& type, const std::string& joint_name, bool override_mimic, bool override_base, bool override_caster)
        {
            set_actuator_types(type, {joint_name}, override_mimic, override_base, override_caster);
        }

        void Tiago::_post_addition(RobotDARTSimu* simu)
        {
            // We do not want to add sensors if we are a ghost robot
            if (ghost())
                return;
            simu->add_sensor(_ft_wrist);
        }

        void Tiago::_post_removal(RobotDARTSimu* simu)
        {
            simu->remove_sensor(_ft_wrist);
        }

    } // namespace robots
} // namespace robot_dart
```


