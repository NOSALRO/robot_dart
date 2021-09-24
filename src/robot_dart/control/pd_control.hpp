#ifndef ROBOT_DART_CONTROL_PD_CONTROL
#define ROBOT_DART_CONTROL_PD_CONTROL

#include <Eigen/Core>
#include <utility>

#include <robot_dart/control/robot_control.hpp>
#include <robot_dart/robot.hpp>

#include <dart/dynamics/RevoluteJoint.hpp>

namespace robot_dart {
    namespace control {

        class PDControl : public RobotControl {
        public:
            PDControl();
            PDControl(const Eigen::VectorXd& ctrl, bool full_control = false);
            PDControl(const Eigen::VectorXd& ctrl, const std::vector<std::string>& controllable_dofs);

            void configure() override;
            Eigen::VectorXd calculate(double) override;

            void set_pd(double p, double d);
            void set_pd(const Eigen::VectorXd& p, const Eigen::VectorXd& d);

            std::pair<Eigen::VectorXd, Eigen::VectorXd> pd() const;

            std::shared_ptr<RobotControl> clone() const override;

        protected:
            Eigen::VectorXd _Kp;
            Eigen::VectorXd _Kd;

            inline double angle_dist(const double alfa, const double beta){
              double theta = alfa - beta;
              while (theta < -M_PI)
                theta += 2 * M_PI;
              while (theta > M_PI)
                theta -= 2 * M_PI;
              return theta;
            }

        };
    } // namespace control
} // namespace robot_dart

#endif
