#ifndef ROBOT_DART_POLICY_CONTROL
#define ROBOT_DART_POLICY_CONTROL

#include <robot_dart/robot.hpp>
#include <robot_dart/robot_control.hpp>

namespace robot_dart {

    template <typename Policy>
    class PolicyControl : public RobotControl {
    public:
        PolicyControl() : RobotControl() {}
        PolicyControl(const std::vector<double>& ctrl, bool full_control = false) : RobotControl(ctrl, full_control) {}

        void configure() override
        {
            _policy.set_params(_ctrl);
            if (_policy.output_size() == _control_dof)
                _active = true;
        }

        Eigen::VectorXd calculate(double t) override
        {
            assert(_control_dof == _policy.output_size());
            Eigen::VectorXd commands = _policy.query(_robot, t);

            return commands;
        }

        std::shared_ptr<RobotControl> clone() const override
        {
            return std::make_shared<PolicyControl>(*this);
        }

    protected:
        Policy _policy;
    };
} // namespace robot_dart

#endif
