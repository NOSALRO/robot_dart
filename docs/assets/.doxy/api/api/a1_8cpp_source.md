

# File a1.cpp

[**File List**](files.md) **>** [**robot\_dart**](dir_166284c5f0440000a6384365f2a45567.md) **>** [**robots**](dir_087fbdcd93b501a5d3f98df93e9f8cc4.md) **>** [**a1.cpp**](a1_8cpp.md)

[Go to the documentation of this file](a1_8cpp.md)

```C++

#include "robot_dart/robots/a1.hpp"
#include "robot_dart/robot_dart_simu.hpp"

namespace robot_dart {
    namespace robots {
        A1::A1(size_t frequency, const std::string& urdf, const std::vector<std::pair<std::string, std::string>>& packages)
            : Robot(urdf, packages),
              _imu(std::make_shared<sensor::IMU>(sensor::IMUConfig(body_node("imu_link"), frequency)))
        {
            set_color_mode("material");
            set_self_collision(true);
            set_position_enforced(true);

            // put above ground
            set_base_pose(robot_dart::make_vector({0., 0., 0., 0., 0., 0.5}));

            // standing pose
            auto names = dof_names(true, true, true);
            names = std::vector<std::string>(names.begin() + 6, names.end());
            set_positions(robot_dart::make_vector({0.0, 0.67, -1.3, -0.0, 0.67, -1.3, 0.0, 0.67, -1.3, -0.0, 0.67, -1.3}), names);
        }

        void A1::reset()
        {
            Robot::reset();

            // put above ground
            set_base_pose(robot_dart::make_vector({0., 0., 0., 0., 0., 0.5}));

            // standing pose
            auto names = dof_names(true, true, true);
            names = std::vector<std::string>(names.begin() + 6, names.end());
            set_positions(robot_dart::make_vector({0.0, 0.67, -1.3, -0.0, 0.67, -1.3, 0.0, 0.67, -1.3, -0.0, 0.67, -1.3}), names);
        }
    } // namespace robots
} // namespace robot_dart

```

