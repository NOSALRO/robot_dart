#include "pd_control.hpp"
#include "robot_dart/robot.hpp"
#include "robot_dart/utils.hpp"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wold-style-cast"
#pragma GCC diagnostic ignored "-Wzero-as-null-pointer-constant"
#include <dart/dynamics/BallJoint.hpp>
#include <dart/dynamics/EulerJoint.hpp>
#include <dart/dynamics/FreeJoint.hpp>
#include <dart/dynamics/RevoluteJoint.hpp>
#pragma GCC diagnostic pop

namespace robot_dart {
    namespace control {
        PDControl::PDControl() : RobotControl() {}
        PDControl::PDControl(const Eigen::VectorXd& ctrl, bool full_control, bool use_angular_errors) : RobotControl(ctrl, full_control), _use_angular_errors(use_angular_errors) {}
        PDControl::PDControl(const Eigen::VectorXd& ctrl, const std::vector<std::string>& controllable_dofs, bool use_angular_errors) : RobotControl(ctrl, controllable_dofs), _use_angular_errors(use_angular_errors) {}

        void PDControl::configure()
        {
            if (_ctrl.size() == _control_dof)
                _active = true;

            if (_Kp.size() == 0)
                set_pd(10., 0.1);
        }

        Eigen::VectorXd PDControl::calculate(double)
        {
            ROBOT_DART_ASSERT(_control_dof == _ctrl.size(), "PDControl: Controller parameters size is not the same as DOFs of the robot", Eigen::VectorXd::Zero(_control_dof));
            auto robot = _robot.lock();

            Eigen::VectorXd dq = robot->velocities(_controllable_dofs);

            Eigen::VectorXd error;
            if (!_use_angular_errors) {
                Eigen::VectorXd q = robot->positions(_controllable_dofs);
                error = _ctrl - q;
            }
            else {
                error = Eigen::VectorXd::Zero(_control_dof);

                std::unordered_map<size_t, Eigen::VectorXd> joint_vals, joint_desired, errors;

                for (int i = 0; i < _control_dof; ++i) {
                    auto dof = robot->dof(_controllable_dofs[i]);
                    size_t joint_index = dof->getJoint()->getJointIndexInSkeleton();
                    if (joint_vals.find(joint_index) == joint_vals.end()) {
                        joint_vals[joint_index] = dof->getJoint()->getPositions();
                        joint_desired[joint_index] = dof->getJoint()->getPositions();
                    }

                    joint_desired[joint_index][dof->getIndexInJoint()] = _ctrl[i];
                }

                for (int i = 0; i < _control_dof; ++i) {
                    auto dof = robot->dof(_controllable_dofs[i]);
                    size_t joint_index = dof->getJoint()->getJointIndexInSkeleton();
                    size_t dof_index_in_joint = dof->getIndexInJoint();

                    Eigen::VectorXd val;
                    if (errors.find(joint_index) == errors.end()) {
                        val = Eigen::VectorXd(dof->getJoint()->getNumDofs());

                        std::string joint_type = robot->dof(_controllable_dofs[i])->getJoint()->getType();
                        if (joint_type == dart::dynamics::RevoluteJoint::getStaticType()) {
                            val[dof_index_in_joint] = _angle_dist(_ctrl[i], joint_vals[joint_index][dof_index_in_joint]);
                        }
                        else if (joint_type == dart::dynamics::BallJoint::getStaticType()) {
                            Eigen::Matrix3d R_desired = dart::math::expMapRot(joint_desired[joint_index]);
                            Eigen::Matrix3d R_current = dart::math::expMapRot(joint_vals[joint_index]);
                            val = dart::math::logMap(R_desired * R_current.transpose());
                        }
                        else if (joint_type == dart::dynamics::EulerJoint::getStaticType()) {
                            // TO-DO: Check if this is 100% correct
                            for (size_t d = 0; d < dof->getJoint()->getNumDofs(); d++)
                                val[d] = _angle_dist(joint_desired[joint_index][d], joint_vals[joint_index][d]);
                        }
                        else if (joint_type == dart::dynamics::FreeJoint::getStaticType()) {
                            auto free_joint = static_cast<dart::dynamics::FreeJoint*>(dof->getJoint());

                            Eigen::Isometry3d tf_desired = free_joint->convertToTransform(joint_desired[joint_index]);
                            Eigen::Isometry3d tf_current = free_joint->convertToTransform(joint_vals[joint_index]);

                            val.tail(3) = tf_desired.translation() - tf_current.translation();
                            val.head(3) = dart::math::logMap(tf_desired.linear().matrix() * tf_current.linear().matrix().transpose());
                        }
                        else {
                            val[dof_index_in_joint] = _ctrl[i] - joint_vals[joint_index][dof_index_in_joint];
                        }

                        errors[joint_index] = val;
                    }
                    else
                        val = errors[joint_index];
                    error(i) = val[dof_index_in_joint];
                }
            }

            /// Compute the simplest PD controller output:
            /// P gain * (target position - current position) + D gain * (0 - current velocity)
            Eigen::VectorXd commands = _Kp.array() * error.array() - _Kd.array() * dq.array();

            return commands;
        }

        void PDControl::set_pd(double Kp, double Kd)
        {
            _Kp = Eigen::VectorXd::Constant(_control_dof, Kp);
            _Kd = Eigen::VectorXd::Constant(_control_dof, Kd);
        }

        void PDControl::set_pd(const Eigen::VectorXd& Kp, const Eigen::VectorXd& Kd)
        {
            ROBOT_DART_ASSERT(Kp.size() == _control_dof, "PDControl: The Kp size is not the same as the DOFs!", );
            ROBOT_DART_ASSERT(Kd.size() == _control_dof, "PDControl: The Kd size is not the same as the DOFs!", );
            _Kp = Kp;
            _Kd = Kd;
        }

        std::pair<Eigen::VectorXd, Eigen::VectorXd> PDControl::pd() const
        {
            return std::make_pair(_Kp, _Kd);
        }

        bool PDControl::using_angular_errors() const { return _use_angular_errors; }

        void PDControl::set_use_angular_errors(bool enable) { _use_angular_errors = enable; }

        std::shared_ptr<RobotControl> PDControl::clone() const
        {
            return std::make_shared<PDControl>(*this);
        }

        double PDControl::_angle_dist(double target, double current)
        {
            double theta = target - current;
            while (theta < -M_PI)
                theta += 2 * M_PI;
            while (theta > M_PI)
                theta -= 2 * M_PI;
            return theta;
        }
    } // namespace control
} // namespace robot_dart
