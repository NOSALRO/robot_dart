#ifndef ROBOT_DART_CONTROL_OS_CONTROL
#define ROBOT_DART_CONTROL_OS_CONTROL

#include <Eigen/Core>
#include <utility>

#include <robot_dart/control/robot_control.hpp>

namespace robot_dart {
    namespace control {

        // Operational Space control (for open chains)
        // Requires torque-control
        // Extracted and adapted from official DART examples
        class OSControl : public RobotControl {
        public:
            OSControl();
            OSControl(const std::vector<double>& ctrl, const std::string& end_effector, bool control_orientation = false);

            void configure() override;
            Eigen::VectorXd calculate(double) override;

            void set_pd(double p, double d);
            void set_pd(const Eigen::VectorXd& p, const Eigen::VectorXd& d);

            std::pair<Eigen::VectorXd, Eigen::VectorXd> pd() const;

            std::shared_ptr<RobotControl> clone() const override;

        protected:
            Eigen::VectorXd _Kp;
            Eigen::VectorXd _Kd;
            std::string _end_effector;
            bool _control_orientation;
        };
    } // namespace control
} // namespace robot_dart

#endif
