#ifndef ROBOT_DART_SENSOR_FORCE_TORQUE_HPP
#define ROBOT_DART_SENSOR_FORCE_TORQUE_HPP

#include <robot_dart/sensor/sensor.hpp>

namespace dart {
    namespace dynamics {
        class Joint;
    }
} // namespace dart

namespace robot_dart {
    class Robot;

    namespace sensor {
        class ForceTorque : public Sensor {
        public:
            ForceTorque(RobotDARTSimu* simu, const std::string& joint_name, size_t frequency = 1000, const std::string& direction = "child_to_parent");

            void init() override;

            void calculate(double t) override;

            std::string type() const override;

            const Eigen::Vector3d& force() const;
            const Eigen::Vector3d& torque() const;

            void attach_to(const std::string& joint_name, const Eigen::Isometry3d& tf = Eigen::Isometry3d::Identity()) override;

        protected:
            std::weak_ptr<Robot> _robot;
            std::string _joint_name;
            dart::dynamics::Joint* _joint_attached;

            std::string _direction;

            Eigen::Vector3d _force;
            Eigen::Vector3d _torque;
        };
    } // namespace sensor
} // namespace robot_dart

#endif