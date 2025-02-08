

# File force\_torque.hpp

[**File List**](files.md) **>** [**robot\_dart**](dir_166284c5f0440000a6384365f2a45567.md) **>** [**sensor**](dir_d1adb19f0b40b70b30ee0daf1901679b.md) **>** [**force\_torque.hpp**](force__torque_8hpp.md)

[Go to the documentation of this file](force__torque_8hpp.md)


```C++
#ifndef ROBOT_DART_SENSOR_FORCE_TORQUE_HPP
#define ROBOT_DART_SENSOR_FORCE_TORQUE_HPP

#include <robot_dart/sensor/sensor.hpp>

namespace robot_dart {
    namespace sensor {
        class ForceTorque : public Sensor {
        public:
            ForceTorque(dart::dynamics::Joint* joint, size_t frequency = 1000, const std::string& direction = "child_to_parent");
            ForceTorque(const std::shared_ptr<Robot>& robot, const std::string& joint_name, size_t frequency = 1000, const std::string& direction = "child_to_parent") : ForceTorque(robot->joint(joint_name), frequency, direction) {}

            void init() override;

            void calculate(double) override;

            std::string type() const override;

            Eigen::Vector3d force() const;
            Eigen::Vector3d torque() const;
            const Eigen::Vector6d& wrench() const;

            void attach_to_body(dart::dynamics::BodyNode*, const Eigen::Isometry3d&) override
            {
                ROBOT_DART_WARNING(true, "You cannot attach a force/torque sensor to a body!");
            }

        protected:
            std::string _direction;

            Eigen::Vector6d _wrench;
        };
    } // namespace sensor
} // namespace robot_dart

#endif
```


