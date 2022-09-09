
#include "robot_dart/robots/ur3e.hpp"
#include "robot_dart/robot_dart_simu.hpp"

namespace robot_dart {
    namespace robots {
        Ur3e::Ur3e(size_t frequency, const std::string& urdf, const std::vector<std::pair<std::string, std::string>>& packages)
            : Robot(urdf, packages),
              _ft_wrist(std::make_shared<sensor::ForceTorque>(joint("wrist_3-flange"), frequency))
        {
            fix_to_world();
            set_position_enforced(true);
            set_color_mode("material");
        }

        void Ur3e::_post_addition(RobotDARTSimu* simu)
        {
            // We do not want to add sensors if we are a ghost robot
            if (ghost())
                return;
            simu->add_sensor(_ft_wrist);
        }

        void Ur3e::_post_removal(RobotDARTSimu* simu)
        {
            simu->remove_sensor(_ft_wrist);
        }
    } // namespace robots
} // namespace robot_dart
