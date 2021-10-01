#ifndef ROBOT_DART_CONTROL_ROBOT_CONTROL
#define ROBOT_DART_CONTROL_ROBOT_CONTROL

#include <robot_dart/utils.hpp>

#include <memory>
#include <vector>

namespace robot_dart {
    class Robot;

    namespace control {

        class RobotControl {
        public:
            RobotControl();
            RobotControl(const Eigen::VectorXd& ctrl, bool full_control = false);
            RobotControl(const Eigen::VectorXd& ctrl, const std::vector<std::string>& controllable_dofs);
            virtual ~RobotControl() {}

            void set_parameters(const Eigen::VectorXd& ctrl);
            const Eigen::VectorXd& parameters() const;

            void init();

            void set_robot(const std::shared_ptr<Robot>& robot);
            std::shared_ptr<Robot> robot() const;

            void activate(bool enable = true);
            bool active() const;

            const std::vector<std::string>& controllable_dofs() const;

            double weight() const;
            void set_weight(double weight);

            virtual void configure() = 0;
            // TO-DO: Maybe make this const?
            virtual Eigen::VectorXd calculate(double t) = 0;
            virtual std::shared_ptr<RobotControl> clone() const = 0;

        protected:
            std::weak_ptr<Robot> _robot;
            Eigen::VectorXd _ctrl;
            double _weight;
            bool _active, _check_free = false;
            int _dof, _control_dof;
            std::vector<std::string> _controllable_dofs;
        };
    } // namespace control
} // namespace robot_dart

#endif
