#include "os_control.hpp"
#include "robot_dart/robot.hpp"
#include "robot_dart/utils.hpp"

#include <dart/dynamics/BodyNode.hpp>
#include <dart/dynamics/DegreeOfFreedom.hpp>

namespace robot_dart {
    namespace control {
        OSControl::OSControl() : RobotControl() {}
        OSControl::OSControl(const Eigen::VectorXd& ctrl, const std::string& end_effector, bool control_orientation) : RobotControl(ctrl, false), _end_effector(end_effector), _control_orientation(control_orientation) {}

        void OSControl::configure()
        {
            if ((_control_orientation && _ctrl.size() == 6) || (!_control_orientation && _ctrl.size() == 3))
                _active = true;

            auto robot = _robot.lock();

            bool is_end_effector_invalid = (robot->skeleton()->getBodyNode(_end_effector) == nullptr);
            if (is_end_effector_invalid) {
                ROBOT_DART_WARNING(is_end_effector_invalid, "End-effector body node does not exist! OSControl is not activated.");
                _active = false;
            }

            bool is_torque = true;
            for (size_t i = 0; i < robot->skeleton()->getNumDofs() && is_torque; ++i) {
                is_torque = robot->skeleton()->getDof(i)->getJoint()->getActuatorType() == dart::dynamics::Joint::FORCE;
            }

            if (!is_torque) {
                ROBOT_DART_WARNING(!is_torque, "OSControl works only with torque-controlled robots.");
                _active = false;
            }

            if (_Kp.size() == 0)
                set_pd(1., 0.1);
        }

        Eigen::VectorXd OSControl::calculate(double)
        {
            // for now assume no orientation
            ROBOT_DART_ASSERT(_ctrl.size() == 3 && !_control_orientation, "OSControl: Operational control with orientation not supported yet!", Eigen::VectorXd::Zero(_control_dof));
            auto robot = _robot.lock();
            Eigen::VectorXd& target_position = _ctrl;

            auto bd = robot->skeleton()->getBodyNode(_end_effector);

            // Get equation of motions
            Eigen::VectorXd x = bd->getTransform().translation();
            Eigen::VectorXd dx = bd->getLinearVelocity();
            Eigen::MatrixXd invM = robot->skeleton()->getInvMassMatrix();
            Eigen::VectorXd Cg = robot->skeleton()->getCoriolisAndGravityForces();
            dart::math::LinearJacobian Jv = bd->getLinearJacobian();
            dart::math::LinearJacobian dJv = bd->getLinearJacobianDeriv();
            Eigen::VectorXd dq = robot->skeleton()->getVelocities();

            // Compute operational space values
            Eigen::MatrixXd A = Jv * invM;
            Eigen::VectorXd b = dJv * dq;
            Eigen::MatrixXd M2 = Jv * invM * Jv.transpose();

            size_t dim = target_position.size();
            Eigen::MatrixXd Kp = Eigen::MatrixXd::Zero(dim, dim);
            Kp.diagonal().array() = _Kp.array();
            Eigen::MatrixXd Kv = Eigen::MatrixXd::Zero(dim, dim);
            Kv.diagonal().array() = _Kd.array();

            // Compute virtual operational space spring force at the end effector
            Eigen::Vector3d f = -Kp * (x - target_position) - Kv * dx;

            // Compute desired operational space acceleration given f
            Eigen::Vector3d desired_ddx = b + M2 * f;

            // Gravity compensation
            Eigen::VectorXd commands = Cg;

            // Compute joint space forces to achieve the desired acceleration by solving:
            // A tau + b = desired_ddx
            commands += A.colPivHouseholderQr().solve(desired_ddx - b);

            return commands;
        }

        void OSControl::set_pd(double Kp, double Kd)
        {
            _Kp = Eigen::VectorXd::Constant(_ctrl.size(), Kp);
            _Kd = Eigen::VectorXd::Constant(_ctrl.size(), Kd);
        }

        void OSControl::set_pd(const Eigen::VectorXd& Kp, const Eigen::VectorXd& Kd)
        {
            ROBOT_DART_ASSERT(static_cast<size_t>(Kp.size()) == _ctrl.size(), "OSControl: The Kp size is not the same as the DOFs!", );
            ROBOT_DART_ASSERT(static_cast<size_t>(Kd.size()) == _ctrl.size(), "OSControl: The Kd size is not the same as the DOFs!", );
            _Kp = Kp;
            _Kd = Kd;
        }

        std::pair<Eigen::VectorXd, Eigen::VectorXd> OSControl::pd() const
        {
            return std::make_pair(_Kp, _Kd);
        }

        std::shared_ptr<RobotControl> OSControl::clone() const
        {
            return std::make_shared<OSControl>(*this);
        }
    } // namespace control
} // namespace robot_dart
