#include "robot_dart/robots/a1.hpp"
#include "robot_dart/robot_dart_simu.hpp"

namespace robot_dart {
    namespace robots {
        A1::A1(const std::string& urdf, const std::vector<std::pair<std::string, std::string>>& packages)
            : Robot(urdf, packages)
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
    } // namespace robots
} // namespace robot_dart
