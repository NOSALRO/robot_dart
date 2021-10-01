#ifndef ROBOT_DART_SENSOR_TORQUE_HPP
#define ROBOT_DART_SENSOR_TORQUE_HPP

#include <robot_dart/sensor/sensor.hpp>

namespace robot_dart {
    namespace sensor {
        class Torque : public Sensor {
        public:
            Torque(dart::dynamics::Joint* joint, size_t frequency = 1000);
            Torque(const std::shared_ptr<Robot>& robot, const std::string& joint_name, size_t frequency = 1000) : Torque(robot->joint(joint_name), frequency) {}

            void init() override;

            void calculate(double) override;

            std::string type() const override;

            const Eigen::VectorXd& torques() const;

            void attach_to_body(dart::dynamics::BodyNode*, const Eigen::Isometry3d&) override
            {
                ROBOT_DART_WARNING(true, "You cannot attach a torque sensor to a body!");
            }

        protected:
            Eigen::VectorXd _torques;
        };
    } // namespace sensor
} // namespace robot_dart

#endif