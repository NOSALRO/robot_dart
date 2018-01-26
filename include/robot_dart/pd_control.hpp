#ifndef ROBOT_DART_PD_CONTROL
#define ROBOT_DART_PD_CONTROL

#include <robot_dart/robot.hpp>
#include <robot_dart/robot_control.hpp>

namespace robot_dart {

    class PDControl : public RobotControl {
    public:
        PDControl() : RobotControl() {}
        PDControl(const std::vector<double>& ctrl) : RobotControl(ctrl)
        {
            // Default values for PD controller
            _Kp = 10.0;
            _Kd = 0.1;
        }

        void init() override
        {
            _dof = _robot->skeleton()->getNumDofs();
            _start_dof = 0;
            if (!_robot->fixed())
                _start_dof = 6;
            _prev_error = Eigen::VectorXd::Zero(_dof);

            if (_ctrl.size() == _dof)
                _active = true;
        }

        Eigen::VectorXd commands(double t) override
        {
            assert(_dof == _ctrl.size());
            Eigen::VectorXd target_positions = Eigen::VectorXd::Map(_ctrl.data(), _ctrl.size());

            Eigen::VectorXd q = _robot->skeleton()->getPositions();
            Eigen::VectorXd dq = _robot->skeleton()->getVelocities();

            q += dq * _robot->skeleton()->getTimeStep();

            Eigen::VectorXd q_err = target_positions - q;
            Eigen::VectorXd dq_err = (q_err - _prev_error) / _robot->skeleton()->getTimeStep();
            _prev_error = q_err;

            Eigen::VectorXd commands = _Kp * q_err + _Kd * dq_err;
            if (_start_dof > 0)
                commands.segment(0, _start_dof) = Eigen::VectorXd::Zero(_start_dof);

            return commands;
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
        double _Kp, _Kd;
        Eigen::VectorXd _prev_error;
        size_t _start_dof, _dof;
    };
} // namespace robot_dart

#endif
