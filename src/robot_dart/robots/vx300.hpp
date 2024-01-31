#ifndef ROBOT_DART_ROBOTS_VX300_HPP
#define ROBOT_DART_ROBOTS_VX300_HPP

#include "robot_dart/robot.hpp"

namespace robot_dart {
    namespace robots {
        class Vx300 : public Robot {
        public:
            Vx300(const std::string& urdf = "vx300/vx300.urdf", const std::vector<std::pair<std::string, std::string>>& packages = {{"interbotix_xsarm_descriptions", "vx300"}}) : Robot(urdf, packages)
            {
                fix_to_world();
                set_position_enforced(true);
                set_color_mode("aspect");
            }
        };
    } // namespace robots
} // namespace robot_dart
#endif
