#include "pd_control.hpp"
#include "robot_dart/robot.hpp"
#include "robot_dart/utils.hpp"

#include <dart/dynamics/RevoluteJoint.hpp>

namespace robot_dart {
    namespace control {
        PDControl::PDControl() : RobotControl() {}
        PDControl::PDControl(const Eigen::VectorXd& ctrl, bool full_control, bool angular_errors) : RobotControl(ctrl, full_control), _angular_errors(angular_errors) {}
        PDControl::PDControl(const Eigen::VectorXd& ctrl, const std::vector<std::string>& controllable_dofs, bool angular_errors) : RobotControl(ctrl, controllable_dofs), _angular_errors(angular_errors) {}

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
            Eigen::VectorXd& target_positions = _ctrl;

            Eigen::VectorXd q = robot->positions(_controllable_dofs);
            Eigen::VectorXd dq = robot->velocities(_controllable_dofs);

            Eigen::VectorXd error = Eigen::VectorXd::Zero(_control_dof);
            for (int i = 0; i < _control_dof; ++i) {
                if (robot->dof(_controllable_dofs[i])->getJoint()->getType() == dart::dynamics::RevoluteJoint::getStaticType()
                    and _angular_errors) {
                    error(i) = _angle_dist(target_positions(i), q(i));
                }
                else {
                    error(i) = target_positions(i) - q(i);
                }
            }

            /// Compute the simplest PD controller output:
            /// P gain * (target position - current position) + D gain * (0 -
            /// current velocity)
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

        std::shared_ptr<RobotControl> PDControl::clone() const
        {
            return std::make_shared<PDControl>(*this);
        }

        double PDControl::_angle_dist(const double alfa, const double beta)
        {
            double theta = alfa - beta;
            while (theta < -M_PI)
                theta += 2 * M_PI;
            while (theta > M_PI)
                theta -= 2 * M_PI;
            return theta;
        }
    } // namespace control
} // namespace robot_dart
