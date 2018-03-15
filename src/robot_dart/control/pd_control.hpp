#ifndef ROBOT_DART_CONTROL_PD_CONTROL
#define ROBOT_DART_CONTROL_PD_CONTROL

#include <utility>

#include <robot_dart/control/robot_control.hpp>

namespace robot_dart {
    namespace control {

        class PDControl : public RobotControl {
        public:
            PDControl();
            PDControl(const std::vector<double>& ctrl, bool full_control = false);

            void configure() override;
            Eigen::VectorXd calculate(double) override;

            void set_pd(double p, double d);
            std::pair<double, double> pd() const;

            std::shared_ptr<RobotControl> clone() const override;

        protected:
            double _Kp = 10.;
            double _Kd = 0.1;
            Eigen::VectorXd _prev_error;
        };
    } // namespace control
} // namespace robot_dart

#endif
