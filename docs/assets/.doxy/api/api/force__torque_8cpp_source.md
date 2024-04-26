

# File force\_torque.cpp

[**File List**](files.md) **>** [**robot\_dart**](dir_166284c5f0440000a6384365f2a45567.md) **>** [**sensor**](dir_d1adb19f0b40b70b30ee0daf1901679b.md) **>** [**force\_torque.cpp**](force__torque_8cpp.md)

[Go to the documentation of this file](force__torque_8cpp.md)


```C++
#include "force_torque.hpp"

#include <robot_dart/utils_headers_dart_dynamics.hpp>

namespace robot_dart {
    namespace sensor {
        ForceTorque::ForceTorque(dart::dynamics::Joint* joint, size_t frequency, const std::string& direction) : Sensor(frequency), _direction(direction)
        {
            attach_to_joint(joint, Eigen::Isometry3d::Identity());
        }

        void ForceTorque::init()
        {
            _wrench.setZero();

            attach_to_joint(_joint_attached, Eigen::Isometry3d::Identity());
            _active = true;
        }

        void ForceTorque::calculate(double)
        {
            if (!_attached_to_joint)
                return; // cannot compute anything if not attached to a joint

            Eigen::Vector6d wrench = Eigen::Vector6d::Zero();
            auto child_body = _joint_attached->getChildBodyNode();
            ROBOT_DART_ASSERT(child_body != nullptr, "Child BodyNode is nullptr", );

            wrench = -dart::math::dAdT(_joint_attached->getTransformFromChildBodyNode(), child_body->getBodyForce());

            // We always compute things in SENSOR frame (aka joint frame)
            if (_direction == "parent_to_child") {
                _wrench = wrench;
            }
            else // "child_to_parent" (default)
            {
                _wrench = -wrench;
            }
        }

        std::string ForceTorque::type() const { return "ft"; }

        Eigen::Vector3d ForceTorque::force() const
        {
            return _wrench.tail(3);
        }

        Eigen::Vector3d ForceTorque::torque() const
        {
            return _wrench.head(3);
        }

        const Eigen::Vector6d& ForceTorque::wrench() const
        {
            return _wrench;
        }
    } // namespace sensor
} // namespace robot_dart
```


