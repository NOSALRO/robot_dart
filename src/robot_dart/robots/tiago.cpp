#include "robot_dart/robots/tiago.hpp"
#include "robot_dart/robot_dart_simu.hpp"

namespace robot_dart {
    namespace robots {
        Tiago::Tiago(size_t frequency, const std::string& urdf, const std::vector<std::pair<std::string, std::string>>& packages)
            : Robot(urdf, packages),
              _ft_wrist(std::make_shared<sensor::ForceTorque>(joint("gripper_tool_joint"), frequency))
        {
            skeleton()->setPosition(2, M_PI / 2.);
            skeleton()->setPosition(5, 0.01);
            set_position_enforced(true);
            set_actuator_types("servo");
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
