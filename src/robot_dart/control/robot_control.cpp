#include "robot_control.hpp"
#include "robot_dart/robots/robot.hpp"
#include "robot_dart/utils.hpp"

#include <dart/dynamics/DegreeOfFreedom.hpp>

namespace robot_dart {
    namespace control {
        RobotControl::RobotControl() : _weight(1.), _active(false), _check_free(true) {}
        RobotControl::RobotControl(const Eigen::VectorXd& ctrl, const std::vector<std::string>& controllable_dofs) : _ctrl(ctrl), _weight(1.), _active(false), _check_free(false), _controllable_dofs(controllable_dofs) {}
        RobotControl::RobotControl(const Eigen::VectorXd& ctrl, bool full_control) : _ctrl(ctrl), _weight(1.), _active(false), _check_free(!full_control) {}

        void RobotControl::set_parameters(const Eigen::VectorXd& ctrl)
        {
            _ctrl = ctrl;
            _active = false;
            init();
        }

        const Eigen::VectorXd& RobotControl::parameters() const
        {
            return _ctrl;
        }

        void RobotControl::init()
        {
            ROBOT_DART_ASSERT(_robot.use_count() > 0, "RobotControl: parent robot should be initialized; use set_robot()", );
            auto robot = _robot.lock();
            _dof = robot->skeleton()->getNumDofs();

            if (_check_free && robot->free()) {
                auto names = robot->dof_names(true, true, true);
                _controllable_dofs = std::vector<std::string>(names.begin() + 6, names.end());
            }
            else if (_controllable_dofs.empty()) {
                // we cannot control mimic, locked and passive joints
                _controllable_dofs = robot->dof_names(true, true, true);
            }

            _control_dof = _controllable_dofs.size();

            configure();
        }

        void RobotControl::set_robot(const std::shared_ptr<robots::Robot>& robot)
        {
            _robot = robot;
        }

        std::shared_ptr<robots::Robot> RobotControl::robot() const
        {
            return _robot.lock();
        }

        void RobotControl::activate(bool enable)
        {
            _active = false;
            if (enable) {
                init();
            }
        }

        bool RobotControl::active() const
        {
            return _active;
        }

        const std::vector<std::string>& RobotControl::controllable_dofs() const { return _controllable_dofs; }

        double RobotControl::weight() const
        {
            return _weight;
        }

        void RobotControl::set_weight(double weight)
        {
            _weight = weight;
        }
    } // namespace control
} // namespace robot_dart