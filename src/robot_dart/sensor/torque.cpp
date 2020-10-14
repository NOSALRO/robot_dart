#include "torque.hpp"

#include <robot_dart/robot_dart_simu.hpp>

#include <dart/dynamics/BodyNode.hpp>
#include <dart/dynamics/Joint.hpp>

#include <dart/dynamics/BallJoint.hpp>
#include <dart/dynamics/EulerJoint.hpp>
#include <dart/dynamics/FreeJoint.hpp>
#include <dart/dynamics/PlanarJoint.hpp>
#include <dart/dynamics/PrismaticJoint.hpp>
#include <dart/dynamics/RevoluteJoint.hpp>
#include <dart/dynamics/ScrewJoint.hpp>
#include <dart/dynamics/TranslationalJoint.hpp>
#include <dart/dynamics/TranslationalJoint2D.hpp>
#include <dart/dynamics/UniversalJoint.hpp>
#include <dart/dynamics/WeldJoint.hpp>

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

        void Torque::calculate(double t)
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