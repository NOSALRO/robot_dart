

# File pd\_control.hpp

[**File List**](files.md) **>** [**control**](dir_1a1ccbdd0954eb7721b1a771872472c9.md) **>** [**pd\_control.hpp**](pd__control_8hpp.md)

[Go to the documentation of this file](pd__control_8hpp.md)

```C++

#ifndef ROBOT_DART_CONTROL_PD_CONTROL
#define ROBOT_DART_CONTROL_PD_CONTROL

#include <utility>

#include <robot_dart/control/robot_control.hpp>
#include <robot_dart/robot.hpp>

namespace robot_dart {
    namespace control {

        class PDControl : public RobotControl {
        public:
            PDControl();
            PDControl(const Eigen::VectorXd& ctrl, bool full_control = false, bool use_angular_errors = true);
            PDControl(const Eigen::VectorXd& ctrl, const std::vector<std::string>& controllable_dofs, bool use_angular_errors = true);

            void configure() override;
            Eigen::VectorXd calculate(double) override;

            void set_pd(double p, double d);
            void set_pd(const Eigen::VectorXd& p, const Eigen::VectorXd& d);

            std::pair<Eigen::VectorXd, Eigen::VectorXd> pd() const;

            bool using_angular_errors() const;
            void set_use_angular_errors(bool enable = true);

            std::shared_ptr<RobotControl> clone() const override;

        protected:
            Eigen::VectorXd _Kp;
            Eigen::VectorXd _Kd;
            bool _use_angular_errors;

            static double _angle_dist(double target, double current);
        };
    } // namespace control
} // namespace robot_dart
#endif

```

