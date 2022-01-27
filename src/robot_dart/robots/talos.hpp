#ifndef ROBOT_DART_ROBOTS_TALOS_HPP
#define ROBOT_DART_ROBOTS_TALOS_HPP

#include "robot_dart/robot.hpp"
#include "robot_dart/sensor/force_torque.hpp"
#include "robot_dart/sensor/imu.hpp"
#include "robot_dart/sensor/torque.hpp"

namespace robot_dart {
    namespace robots {
        /// datasheet: https://pal-robotics.com/wp-content/uploads/2019/07/Datasheet_TALOS.pdf
        class Talos : public Robot {
        public:
            Talos(size_t frequency = 1000, const std::string& urdf = "talos/talos.urdf", const std::vector<std::pair<std::string, std::string>>& packages = {{"talos_description", "talos/talos_description"}});

            const sensor::IMU& imu() const { return *_imu; }
            const sensor::ForceTorque& ft_foot_left() const { return *_ft_foot_left; }
            const sensor::ForceTorque& ft_foot_right() const { return *_ft_foot_right; }
            const sensor::ForceTorque& ft_wrist_left() const { return *_ft_wrist_left; }
            const sensor::ForceTorque& ft_wrist_right() const { return *_ft_wrist_right; }

            using torque_map_t = std::unordered_map<std::string, std::shared_ptr<sensor::Torque>>;
            const torque_map_t& torques() const { return _torques; }

        protected:
            std::shared_ptr<sensor::IMU> _imu;
            std::shared_ptr<sensor::ForceTorque> _ft_foot_left;
            std::shared_ptr<sensor::ForceTorque> _ft_foot_right;
            std::shared_ptr<sensor::ForceTorque> _ft_wrist_left;
            std::shared_ptr<sensor::ForceTorque> _ft_wrist_right;
            torque_map_t _torques;
            size_t _frequency;

            void _post_addition(RobotDARTSimu* simu) override;
            void _post_removal(RobotDARTSimu* simu) override;
        };

        class TalosLight : public Talos {
        public:
            TalosLight(size_t frequency = 1000, const std::string& urdf = "talos/talos_fast.urdf", const std::vector<std::pair<std::string, std::string>>& packages = {{"talos_description", "talos/talos_description"}}) : Talos(frequency, urdf, packages) {}
        };

        //for talos_fast_collision.urdf or talos_box.urdf which have simple box collision shapes
        class TalosFastCollision : public Talos {
        public:
            TalosFastCollision(size_t frequency = 1000, const std::string& urdf = "talos/talos_fast_collision.urdf", const std::vector<std::pair<std::string, std::string>>& packages = {{"talos_description", "talos/talos_description"}}) : Talos(frequency, urdf, packages) {}
            std::vector<std::tuple<std::string, uint32_t, uint32_t>> collision_vec();
        };
    } // namespace robots
} // namespace robot_dart
#endif
