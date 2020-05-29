#include "simple_control.hpp"
#include "robot_dart/robot.hpp"
#include "robot_dart/utils.hpp"

namespace robot_dart {
    namespace control {
        SimpleControl::SimpleControl() : RobotControl() {}
        SimpleControl::SimpleControl(const Eigen::VectorXd& ctrl, bool full_control) : RobotControl(ctrl, full_control) {}
        SimpleControl::SimpleControl(const Eigen::VectorXd& ctrl, const std::vector<std::string>& controllable_dofs) : RobotControl(ctrl, controllable_dofs) {}

        void SimpleControl::configure()
        {
            if (_ctrl.size() == _control_dof)
                _active = true;
        }

        Eigen::VectorXd SimpleControl::calculate(double)
        {
            ROBOT_DART_ASSERT(_control_dof == _ctrl.size(), "SimpleControl: Controller parameters size is not the same as DOFs of the robot", Eigen::VectorXd::Zero(_control_dof));
            return _ctrl;
        }

        std::shared_ptr<RobotControl> SimpleControl::clone() const
        {
            return std::make_shared<SimpleControl>(*this);
        }
    } // namespace control
} // namespace robot_dart