#ifndef ROBOT_DART_CONTROL_PD_CONTROL
#define ROBOT_DART_CONTROL_PD_CONTROL

#include <Eigen/Core>
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
            void set_pd(const Eigen::VectorXd& p, const Eigen::VectorXd& d);

            std::pair<Eigen::VectorXd, Eigen::VectorXd> pd() const;

            std::shared_ptr<RobotControl> clone() const override;

        protected:
            Eigen::VectorXd _Kp;
            Eigen::VectorXd _Kd;
        };
    } // namespace control
} // namespace robot_dart

#endif
