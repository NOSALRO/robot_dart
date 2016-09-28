#ifndef ROBOT_DART_ROBOT_CONTROL
#define ROBOT_DART_ROBOT_CONTROL

#include <robot_dart/robot.hpp>

namespace robot_dart {

    class RobotControl {
    public:
        using robot_t = std::shared_ptr<Robot>;

        RobotControl() {}
        RobotControl(const std::vector<double>& ctrl, robot_t robot)
            : _robot(robot), _ctrl(ctrl)
        {
            _dof = _robot->skeleton()->getNumDofs();
        }

        virtual void set_parameters(const std::vector<double>& ctrl)
        {
            _ctrl = ctrl;
        }

        std::vector<double> parameters() const
        {
            return _ctrl;
        }

        robot_t robot()
        {
            return _robot;
        }

        virtual void update(double t)
        {
            set_commands();
        }

        virtual void set_commands()
        {
        }

    protected:
        robot_t _robot;

        std::vector<double> _ctrl;
        size_t _dof;
    };
}

#endif
