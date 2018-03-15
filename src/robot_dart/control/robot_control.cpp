#include "robot_control.hpp"
#include "robot_dart/robot.hpp"
#include "robot_dart/utils.hpp"

#include <dart/dynamics/DegreeOfFreedom.hpp>

namespace robot_dart {
    namespace control {
        RobotControl::RobotControl() : _weight(1.), _active(false), _full_control(false) {}
        RobotControl::RobotControl(const std::vector<double>& ctrl, bool full_control) : _ctrl(ctrl), _weight(1.), _active(false), _full_control(full_control) {}

        void RobotControl::set_parameters(const std::vector<double>& ctrl)
        {
            _ctrl = ctrl;
            _active = false;
            init();
        }

        std::vector<double> RobotControl::parameters() const
        {
            return _ctrl;
        }

        void RobotControl::init()
        {
            ROBOT_DART_ASSERT(_robot, "RobotControl: parent robot should be initialized; use set_robot()", );
            _dof = _robot->skeleton()->getNumDofs();
            _start_dof = 0;
            if (_robot->free() && !_full_control)
                _start_dof = 6;
            _control_dof = _dof - _start_dof;

            for (size_t i = 0; i < _start_dof; i++)
                _robot->skeleton()->getDof(i)->getJoint()->setActuatorType(dart::dynamics::Joint::FORCE);

            configure();
        }

        void RobotControl::set_robot(const std::shared_ptr<Robot>& robot)
        {
            _robot = robot;
        }

        std::shared_ptr<Robot> RobotControl::robot() const
        {
            return _robot;
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

        bool RobotControl::fully_controlled() const
        {
            return _full_control;
        }

        void RobotControl::set_full_control(bool enable)
        {
            _full_control = enable;
            _active = false;
            init();
        }

        double RobotControl::weight() const
        {
            return _weight;
        }

        void RobotControl::set_weight(double weight)
        {
            _weight = weight;
        }

        Eigen::VectorXd RobotControl::commands(double t)
        {
            Eigen::VectorXd coms = Eigen::VectorXd::Zero(_dof);
            coms.tail(_control_dof) = calculate(t);

            return coms;
        }
    } // namespace control
} // namespace robot_dart