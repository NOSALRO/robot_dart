
#include "robot_dart/robots/tiago.hpp"
#include "robot_dart/robot_dart_simu.hpp"

namespace robot_dart {
    namespace robots {
        Tiago::Tiago(RobotDARTSimu* simu, size_t frequency,
            const std::string& urdf, const std::vector<std::pair<std::string, std::string>>& packages)
            : Robot(urdf, packages),
              _ft_wrist(std::make_shared<sensor::ForceTorque>(joint("gripper_tool_joint"), frequency))
        {
            simu->add_sensor(_ft_wrist);
            auto p = positions();
            p[2] = M_PI / 2.;
            p[5] = 0.001;
            set_positions(p);
            set_position_enforced(true);

            set_actuator_types("servo");

        }
    } // namespace robots
} // namespace robot_dart
