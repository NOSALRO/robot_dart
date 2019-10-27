#include "pd_control.hpp"
#include "robot_dart/robot.hpp"
#include "robot_dart/utils.hpp"

namespace robot_dart {
    namespace control {
        PDControl::PDControl() : RobotControl() {}
        PDControl::PDControl(const std::vector<double>& ctrl, bool full_control) : RobotControl(ctrl, full_control) {}

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
            Eigen::VectorXd target_positions = Eigen::VectorXd::Map(_ctrl.data(), _ctrl.size());

            Eigen::VectorXd q = get_positions();
            Eigen::VectorXd dq = get_velocities();

            /// Compute the simplest PD controller output:
            /// P gain * (target position - current position) + D gain * (0 - current velocity)
            Eigen::VectorXd commands = _Kp.array() * (target_positions.array() - q.array()) - _Kd.array() * dq.array();

            return commands;
        }

        void PDControl::set_pd(double Kp, double Kd)
        {
            _Kp = Eigen::VectorXd::Constant(_control_dof, Kp);
            _Kd = Eigen::VectorXd::Constant(_control_dof, Kd);
        }

        void PDControl::set_pd(const Eigen::VectorXd& Kp, const Eigen::VectorXd& Kd)
        {
            ROBOT_DART_ASSERT(static_cast<size_t>(Kp.size()) == _control_dof, "PDControl: The Kp size is not the same as the DOFs!", );
            ROBOT_DART_ASSERT(static_cast<size_t>(Kd.size()) == _control_dof, "PDControl: The Kd size is not the same as the DOFs!", );
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
    } // namespace control
} // namespace robot_dart
