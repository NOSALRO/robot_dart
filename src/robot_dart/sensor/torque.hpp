#ifndef ROBOT_DART_SENSOR_TORQUE_HPP
#define ROBOT_DART_SENSOR_TORQUE_HPP

#include <robot_dart/sensor/sensor.hpp>

namespace robot_dart {
    namespace sensor {
        class Torque : public Sensor {
        public:
            Torque(RobotDARTSimu* simu, dart::dynamics::Joint* joint, size_t frequency = 1000, const std::string& direction = "child_to_parent");
            Torque(RobotDARTSimu* simu, const std::shared_ptr<Robot>& robot, const std::string& joint_name, size_t frequency = 1000, const std::string& direction = "child_to_parent") 
                : Torque(simu, robot->joint(joint_name), frequency, direction) {}

            void init() override;

            void calculate(double t) override;

            std::string type() const override;

            const Eigen::VectorXd& torque() const;

            void attach_to_body(dart::dynamics::BodyNode* body, const Eigen::Isometry3d& tf = Eigen::Isometry3d::Identity()) override
            {
                ROBOT_DART_WARNING(true, "You cannot attach a torque sensor to a body!");
            }

        protected:
            std::string _direction;

            Eigen::VectorXd _torque;
        };
    } // namespace sensor
} // namespace robot_dart

#endif