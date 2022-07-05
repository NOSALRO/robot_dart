#ifndef ROBOT_DART_ROBOTS_TIAGO_HPP
#define ROBOT_DART_ROBOTS_TIAGO_HPP

#include "robot_dart/robot.hpp"
#include "robot_dart/sensor/force_torque.hpp"

namespace robot_dart {
    namespace robots {
        /// datasheet: https://pal-robotics.com/wp-content/uploads/2021/07/Datasheet-complete_TIAGo-2021.pdf
        class Tiago : public Robot {
        public:
            Tiago(size_t frequency = 1000, const std::string& urdf = "tiago/tiago_steel.urdf", const std::vector<std::pair<std::string, std::string>>& packages = {{"tiago_description", "tiago/tiago_description"}});

            void reset() override;

            const sensor::ForceTorque& ft_wrist() const { return *_ft_wrist; }

            std::vector<std::string> caster_joints() const { return {"caster_back_left_2_joint", "caster_back_left_1_joint", "caster_back_right_2_joint", "caster_back_right_1_joint", "caster_front_left_2_joint", "caster_front_left_1_joint", "caster_front_right_2_joint", "caster_front_right_1_joint"}; }

            void set_actuator_types(const std::string& type, const std::vector<std::string>& joint_names = {}, bool override_mimic = false, bool override_base = false, bool override_caster = false);
            void set_actuator_type(const std::string& type, const std::string& joint_name, bool override_mimic = false, bool override_base = false, bool override_caster = false);

        protected:
            std::shared_ptr<sensor::ForceTorque> _ft_wrist;
            void _post_addition(RobotDARTSimu* simu) override;
            void _post_removal(RobotDARTSimu* simu) override;
        };
    } // namespace robots
} // namespace robot_dart
#endif
