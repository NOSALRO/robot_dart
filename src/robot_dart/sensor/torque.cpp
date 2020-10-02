#include "torque.hpp"

#include <robot_dart/robot_dart_simu.hpp>

#include <dart/dynamics/BodyNode.hpp>
#include <dart/dynamics/Joint.hpp>

namespace robot_dart {
    namespace sensor {
        Torque::Torque(RobotDARTSimu* simu, dart::dynamics::Joint* joint, size_t frequency, const std::string& direction)
        :   Sensor(simu, frequency),
            _direction(direction),
            _torque(joint->getNumDofs())
        {
            attach_to_joint(joint, Eigen::Isometry3d::Identity());
        }

        void Torque::init()
        {
            _torque.setZero();

            attach_to_joint(_joint_attached, Eigen::Isometry3d::Identity());
            _active = true;
        }

        void Torque::calculate(double t)
        {
            if (!_attached_to_joint)
                return; // cannot compute anything if not attached to a joint

            Eigen::Vector6d F2 = Eigen::Vector6d::Zero();
            auto child_body = _joint_attached->getChildBodyNode();
            // ROBOT_DART_ASSERT(child_body != nullptr, "Child BodyNode is nullptr", {});
            if (child_body)
                F2 = -dart::math::dAdT(_joint_attached->getTransformFromChildBodyNode(), child_body->getBodyForce());

            // We always compute things in SENSOR frame (aka joint frame)
            if (_direction == "parent_to_child")
                _torque = F2.head(3);
            else // "child_to_parent" (default)
                _torque = -F2.head(3);

            Eigen::Matrix3d R = _joint_attached->getTransformFromChildBodyNode().linear().transpose();
            _torque = R * _torque;
        }

        std::string Torque::type() const { return "t"; }

        const Eigen::VectorXd& Torque::torque() const
        {
            return _torque;
        }
    } // namespace sensor
} // namespace robot_dart