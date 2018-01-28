#ifndef ROBOT_DART_CONTROL_SIMPLE_CONTROL
#define ROBOT_DART_CONTROL_SIMPLE_CONTROL

#include <robot_dart/control/robot_control.hpp>
#include <robot_dart/robot.hpp>

namespace robot_dart {
    namespace control {

        class SimpleControl : public RobotControl {
        public:
            SimpleControl() : RobotControl() {}
            SimpleControl(const std::vector<double>& ctrl, bool full_control = false) : RobotControl(ctrl, full_control) {}

            void configure() override
            {
                if (_ctrl.size() == _control_dof)
                    _active = true;
            }

            Eigen::VectorXd calculate(double t) override
            {
                assert(_control_dof == _ctrl.size());
                Eigen::VectorXd commands = Eigen::VectorXd::Map(_ctrl.data(), _ctrl.size());

                return commands;
            }

            std::shared_ptr<RobotControl> clone() const override
            {
                return std::make_shared<SimpleControl>(*this);
            }
        };
    } // namespace control
} // namespace robot_dart

#endif
