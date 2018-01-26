#ifndef ROBOT_DART_SIMPLE_CONTROL
#define ROBOT_DART_SIMPLE_CONTROL

#include <robot_dart/robot.hpp>
#include <robot_dart/robot_control.hpp>

namespace robot_dart {

    class SimpleControl : public RobotControl {
    public:
        SimpleControl() : RobotControl() {}
        SimpleControl(const std::vector<double>& ctrl) : RobotControl(ctrl) {}

        void init()
        {
            _dof = _robot->skeleton()->getNumDofs();
            _start_dof = 0;
            if (!_robot->fixed())
                _start_dof = 6;

            if (_ctrl.size() == _dof)
                _active = true;
        }

        Eigen::VectorXd commands(double t) override
        {
            assert(_dof == _ctrl.size());
            Eigen::VectorXd commands = Eigen::VectorXd::Map(_ctrl.data(), _ctrl.size());

            if (_start_dof > 0)
                commands.segment(0, _start_dof) = Eigen::VectorXd::Zero(_start_dof);

            return commands;
        }

    protected:
        size_t _start_dof, _dof;
    };
} // namespace robot_dart

#endif
