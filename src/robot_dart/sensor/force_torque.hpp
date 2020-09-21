#ifndef ROBOT_DART_SENSOR_FORCE_TORQUE_HPP
#define ROBOT_DART_SENSOR_FORCE_TORQUE_HPP

#include <robot_dart/sensor/sensor.hpp>

namespace robot_dart {
    namespace sensor {
        class ForceTorque : public Sensor {
        public:
            ForceTorque(RobotDARTSimu* simu, dart::dynamics::Joint* joint, size_t frequency = 1000, const std::string& direction = "child_to_parent");
            ForceTorque(RobotDARTSimu* simu, const std::shared_ptr<Robot>& robot, const std::string& joint_name, size_t frequency = 1000, const std::string& direction = "child_to_parent") : ForceTorque(simu, robot->joint(joint_name), frequency, direction) {}

            void init() override;

            void calculate(double t) override;

            std::string type() const override;

            const Eigen::Vector3d& force() const;
            const Eigen::Vector3d& torque() const;

            void attach_to_body(dart::dynamics::BodyNode* body, const Eigen::Isometry3d& tf = Eigen::Isometry3d::Identity()) override
            {
                ROBOT_DART_WARNING(true, "You cannot attach a force/torque sensor to a body!");
            }

        protected:
            std::string _direction;

            Eigen::Vector3d _force;
            Eigen::Vector3d _torque;
        };
    } // namespace sensor
} // namespace robot_dart

#endif