

# File torque.cpp

[**File List**](files.md) **>** [**robot\_dart**](dir_166284c5f0440000a6384365f2a45567.md) **>** [**sensor**](dir_d1adb19f0b40b70b30ee0daf1901679b.md) **>** [**torque.cpp**](torque_8cpp.md)

[Go to the documentation of this file](torque_8cpp.md)


```C++
#include "torque.hpp"

#include <robot_dart/robot_dart_simu.hpp>
#include <robot_dart/utils_headers_dart_dynamics.hpp>

namespace robot_dart {
    namespace sensor {
        Torque::Torque(dart::dynamics::Joint* joint, size_t frequency) : Sensor(frequency), _torques(joint->getNumDofs())
        {
            attach_to_joint(joint, Eigen::Isometry3d::Identity());
        }

        void Torque::init()
        {
            _torques.setZero();

            attach_to_joint(_joint_attached, Eigen::Isometry3d::Identity());
            _active = true;
        }

        void Torque::calculate(double)
        {
            if (!_attached_to_joint)
                return; // cannot compute anything if not attached to a joint

            Eigen::Vector6d wrench = Eigen::Vector6d::Zero();
            auto child_body = _joint_attached->getChildBodyNode();
            ROBOT_DART_ASSERT(child_body != nullptr, "Child BodyNode is nullptr", );

            wrench = child_body->getBodyForce();

            // get forces for only the only degrees of freedom in this joint
            _torques = _joint_attached->getRelativeJacobian().transpose() * wrench;
        }

        std::string Torque::type() const { return "t"; }

        const Eigen::VectorXd& Torque::torques() const
        {
            return _torques;
        }
    } // namespace sensor
} // namespace robot_dart
```


