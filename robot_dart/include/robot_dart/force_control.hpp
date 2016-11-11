#ifndef ROBOT_DART_FORCE_CONTROL
#define ROBOT_DART_FORCE_CONTROL

#include <robot_dart/robot_control.hpp>

namespace robot_dart {

    class ForceControl : public RobotControl {
    public:
        using robot_t = std::shared_ptr<Robot>;

        ForceControl() {}
        ForceControl(const std::vector<double>& ctrl, robot_t robot)
            : RobotControl(ctrl, robot)
        {
            _robot->set_actuator_types(dart::dynamics::Joint::FORCE);
        }

        void set_commands()
        {
            assert(_dof == _ctrl.size());
            Eigen::VectorXd commands = Eigen::VectorXd::Map(_ctrl.data(), _ctrl.size());
            _robot->skeleton()->setCommands(commands);
        }
    };
}

#endif
