#include "pd_control.hpp"
#include "robot_dart/robot.hpp"
#include "robot_dart/utils.hpp"

namespace robot_dart {
    namespace control {
        PDControl::PDControl() : RobotControl() {}
        PDControl::PDControl(const std::vector<double>& ctrl, bool full_control) : RobotControl(ctrl, full_control) {}

        void PDControl::configure()
        {
            _prev_error = Eigen::VectorXd::Zero(_dof);

            if (_ctrl.size() == _control_dof)
                _active = true;
        }

        Eigen::VectorXd PDControl::calculate(double)
        {
            ROBOT_DART_ASSERT(_control_dof == _ctrl.size(), "PDControl: Controller parameters size is not the same as DOFs of the robot", Eigen::VectorXd::Zero(_control_dof));
            auto robot = _robot.lock();
            Eigen::VectorXd target_positions = Eigen::VectorXd::Zero(_dof);
            target_positions.tail(_control_dof) = Eigen::VectorXd::Map(_ctrl.data(), _ctrl.size());

            Eigen::VectorXd q = robot->skeleton()->getPositions();
            Eigen::VectorXd dq = robot->skeleton()->getVelocities();

            q += dq * robot->skeleton()->getTimeStep();

            Eigen::VectorXd q_err = target_positions - q;
            Eigen::VectorXd dq_err = (q_err - _prev_error) / robot->skeleton()->getTimeStep();
            _prev_error = q_err;

            Eigen::VectorXd commands = _Kp * q_err + _Kd * dq_err;

            return commands.tail(_control_dof);
        }

        void PDControl::set_pd(double p, double d)
        {
            _Kp = p;
            _Kd = d;
        }

        std::pair<double, double> PDControl::pd() const
        {
            return std::make_pair(_Kp, _Kd);
        }

        std::shared_ptr<RobotControl> PDControl::clone() const
        {
            return std::make_shared<PDControl>(*this);
        }
    } // namespace control
} // namespace robot_dart