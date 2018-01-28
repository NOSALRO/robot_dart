#ifndef ROBOT_DART_PD_CONTROL
#define ROBOT_DART_PD_CONTROL

#include <robot_dart/robot.hpp>
#include <robot_dart/robot_control.hpp>

namespace robot_dart {

    class PDControl : public RobotControl {
    public:
        PDControl() : RobotControl() {}
        PDControl(const std::vector<double>& ctrl, bool full_control = false) : RobotControl(ctrl, full_control) {}

        void configure() override
        {
            _prev_error = Eigen::VectorXd::Zero(_dof);

            if (_ctrl.size() == _control_dof)
                _active = true;
        }

        Eigen::VectorXd calculate(double t) override
        {
            assert(_control_dof == _ctrl.size());
            Eigen::VectorXd target_positions = Eigen::VectorXd::Zero(_dof);
            target_positions.tail(_control_dof) = Eigen::VectorXd::Map(_ctrl.data(), _ctrl.size());

            Eigen::VectorXd q = _robot->skeleton()->getPositions();
            Eigen::VectorXd dq = _robot->skeleton()->getVelocities();

            q += dq * _robot->skeleton()->getTimeStep();

            Eigen::VectorXd q_err = target_positions - q;
            Eigen::VectorXd dq_err = (q_err - _prev_error) / _robot->skeleton()->getTimeStep();
            _prev_error = q_err;

            Eigen::VectorXd commands = _Kp * q_err + _Kd * dq_err;

            return commands.tail(_control_dof);
        }

        void set_pd(double p, double d)
        {
            _Kp = p;
            _Kd = d;
        }

        std::shared_ptr<RobotControl> clone() const override
        {
            return std::make_shared<PDControl>(*this);
        }

    protected:
        double _Kp = 10.;
        double _Kd = 0.1;
        Eigen::VectorXd _prev_error;
    };
} // namespace robot_dart

#endif
