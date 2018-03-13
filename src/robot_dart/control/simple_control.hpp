#ifndef ROBOT_DART_CONTROL_SIMPLE_CONTROL
#define ROBOT_DART_CONTROL_SIMPLE_CONTROL

#include <robot_dart/control/robot_control.hpp>

namespace robot_dart {
    namespace control {

        class SimpleControl : public RobotControl {
        public:
            SimpleControl();
            SimpleControl(const std::vector<double>& ctrl, bool full_control = false);

            void configure() override;
            Eigen::VectorXd calculate(double) override;
            std::shared_ptr<RobotControl> clone() const override;
        };
    } // namespace control
} // namespace robot_dart

#endif
