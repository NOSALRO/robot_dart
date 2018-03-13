#include "simple_control.hpp"
#include "robot_dart/robot.hpp"

namespace robot_dart {
    namespace control {
        SimpleControl::SimpleControl() : RobotControl() {}
        SimpleControl::SimpleControl(const std::vector<double>& ctrl, bool full_control) : RobotControl(ctrl, full_control) {}

        void SimpleControl::configure()
        {
            if (_ctrl.size() == _control_dof)
                _active = true;
        }

        Eigen::VectorXd SimpleControl::calculate(double)
        {
            assert(_control_dof == _ctrl.size());
            Eigen::VectorXd commands = Eigen::VectorXd::Map(_ctrl.data(), _ctrl.size());

            return commands;
        }

        std::shared_ptr<RobotControl> SimpleControl::clone() const
        {
            return std::make_shared<SimpleControl>(*this);
        }
    } // namespace control
} // namespace robot_dart