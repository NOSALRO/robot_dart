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
        Torque::Torque(RobotDARTSimu* simu, dart::dynamics::Joint* joint, size_t frequency, const std::string& direction)
        :   Sensor(simu, frequency),
            _direction(direction),
            _torques(joint->getNumDofs()),
            _motion_subspace(Eigen::MatrixXd(6, joint->getNumDofs()))
        {
            attach_to_joint(joint, Eigen::Isometry3d::Identity());
            update_motion_subspace(joint);
        }

        void Torque::init()
        {
            _wrench.setZero();
            _torques.setZero();

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
            _wrench = (_direction == "parent_to_child") ? F2 : -F2;

            Eigen::Matrix3d R = _joint_attached->getTransformFromChildBodyNode().linear().transpose();
            _wrench.head<3>() = R * _wrench.head<3>();
            _wrench.tail<3>() = R * _wrench.tail<3>();

            // get forces for only the only degrees of freedom in this joint
            _torques = _motion_subspace.transpose() * _wrench;
        }

        std::string Torque::type() const { return "t"; }

        const Eigen::VectorXd& Torque::torques() const
        {
            return _torques;
        }

        void Torque::update_motion_subspace(dart::dynamics::Joint* joint)
        {
            _motion_subspace.setZero();

            if(joint->getType() == "BallJoint")
            {
                _motion_subspace.block<3,3>(0,0) = Eigen::Matrix3d::Identity();
            }
            else if(joint->getType() == "EulerJoint")
            {
                assert(false && "Torque sensor for euler joint not implemented.");
            }
            else if(joint->getType() == "FreeJoint")
            {
                _motion_subspace.diagonal() = Eigen::Vector6d::Ones();
            }
            else if(joint->getType() == "PlanarJoint")
            {
                auto planar_joint = static_cast<dart::dynamics::PlanarJoint*>(joint);
                _motion_subspace.block<3,1>(0,0) = planar_joint->getRotationalAxis();
                _motion_subspace.block<3,1>(3,1) = planar_joint->getTranslationalAxis1();
                _motion_subspace.block<3,1>(3,2) = planar_joint->getTranslationalAxis2();
            }
            else if(joint->getType() == "PrismaticJoint")
            {
                _motion_subspace.block<3,1>(3,0) = static_cast<dart::dynamics::PrismaticJoint*>(joint)->getAxis();
            }
            else if(joint->getType() == "RevoluteJoint")
            {
                _motion_subspace.block<3,1>(0,0) = static_cast<dart::dynamics::RevoluteJoint*>(joint)->getAxis();
            }
            else if(joint->getType() == "ScrewJoint")
            {
                double pitch = static_cast<dart::dynamics::ScrewJoint*>(joint)->getPitch();
                Eigen::Vector3d axis = static_cast<dart::dynamics::ScrewJoint*>(joint)->getAxis();
                _motion_subspace.block<3,1>(0,0) = axis;
                _motion_subspace.block<3,1>(3,0) = pitch * axis;
            }
            else if(joint->getType() == "TranslationalJoint")
            {
                _motion_subspace.block<3,3>(3,0) = Eigen::Matrix3d::Identity();
            }
            else if(joint->getType() == "TranslationalJoint2D")
            {
                auto translational_joint_2d = static_cast<dart::dynamics::TranslationalJoint2D*>(joint);
                _motion_subspace.block<3,1>(0,3) = translational_joint_2d->getTranslationalAxis1();
                _motion_subspace.block<3,1>(1,3) = translational_joint_2d->getTranslationalAxis2();
            }
            else if(joint->getType() == "UniversalJoint")
            {
                assert(false && "Torque sensor for universal joint not implemented.");
            }
            else if(joint->getType() == "WeldJoint")
            {
                assert(false && "Torque sensor for weld joint not implemented.");
            }
            else
            {
                assert(false && "unknown type of joint for a torque sensor.");
            }
        }


    } // namespace sensor
} // namespace robot_dart