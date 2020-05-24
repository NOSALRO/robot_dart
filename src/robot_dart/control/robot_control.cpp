#include "robot_control.hpp"
#include "robot_dart/robot.hpp"
#include "robot_dart/utils.hpp"

#include <dart/dynamics/DegreeOfFreedom.hpp>

namespace robot_dart {
    namespace control {
        RobotControl::RobotControl() : _weight(1.), _active(false), _full_control(false) {}
        RobotControl::RobotControl(const Eigen::VectorXd& ctrl, bool full_control) : _ctrl(ctrl), _weight(1.), _active(false), _full_control(full_control) {}

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
            _start_dof = 0;
            if (robot->free() && !_full_control)
                _start_dof = 6;
            _control_dof = _dof - _start_dof;
#if DART_VERSION_AT_LEAST(6, 7, 0)
            _mimic_dofs.clear();
            for (size_t i = _start_dof; i < robot->skeleton()->getNumDofs(); i++) {
                auto joint = robot->skeleton()->getDof(i)->getJoint();
                if (joint->getActuatorType() == dart::dynamics::Joint::MIMIC)
                    _mimic_dofs.push_back(i);
            }

            _control_dof -= _mimic_dofs.size();
#endif

            for (size_t i = 0; i < _start_dof; i++)
                robot->skeleton()->getDof(i)->getJoint()->setActuatorType(dart::dynamics::Joint::FORCE);

            configure();
        }

        void RobotControl::set_robot(const std::shared_ptr<Robot>& robot)
        {
            _robot = robot;
        }

        std::shared_ptr<Robot> RobotControl::robot() const
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

        bool RobotControl::fully_controlled() const
        {
            return _full_control;
        }

        void RobotControl::set_full_control(bool enable)
        {
            if (enable != _full_control) {
                _full_control = enable;
                _active = false;
                init();
            }
        }

        double RobotControl::weight() const
        {
            return _weight;
        }

        void RobotControl::set_weight(double weight)
        {
            _weight = weight;
        }

        Eigen::VectorXd RobotControl::get_positions() const
        {
            return _get_vector_mimic(_robot.lock()->skeleton()->getPositions());
        }

        void RobotControl::set_positions(const Eigen::VectorXd& positions)
        {
            _robot.lock()->skeleton()->setPositions(_set_vector_mimic(positions));
        }

        Eigen::VectorXd RobotControl::get_velocities() const
        {
            return _get_vector_mimic(_robot.lock()->skeleton()->getVelocities());
        }

        void RobotControl::set_velocities(const Eigen::VectorXd& velocities)
        {
            _robot.lock()->skeleton()->setVelocities(_set_vector_mimic(velocities));
        }

        Eigen::VectorXd RobotControl::get_accelerations() const
        {
            return _get_vector_mimic(_robot.lock()->skeleton()->getAccelerations());
        }

        void RobotControl::set_accelerations(const Eigen::VectorXd& accelerations)
        {
            _robot.lock()->skeleton()->setAccelerations(_set_vector_mimic(accelerations));
        }

        Eigen::VectorXd RobotControl::get_forces() const
        {
            return _get_vector_mimic(_robot.lock()->skeleton()->getForces());
        }

        void RobotControl::set_forces(const Eigen::VectorXd& forces)
        {
            _robot.lock()->skeleton()->setForces(_set_vector_mimic(forces));
        }

        Eigen::VectorXd RobotControl::commands(double t)
        {
            Eigen::VectorXd coms = _set_vector_mimic(calculate(t));

            return coms;
        }

        Eigen::VectorXd RobotControl::_get_vector_mimic(const Eigen::VectorXd& vec) const
        {
#if DART_VERSION_AT_LEAST(6, 7, 0)
            Eigen::VectorXd ret = Eigen::VectorXd::Zero(_control_dof);
            size_t k = 0;
            for (size_t i = _start_dof; i < _dof; i++) {
                auto it = std::find(_mimic_dofs.begin(), _mimic_dofs.end(), i);
                if (it == _mimic_dofs.end())
                    ret(k++) = vec(i);
            }

            return ret;
#else
            return vec.tail(_control_dof);
#endif
        }

        Eigen::VectorXd RobotControl::_set_vector_mimic(const Eigen::VectorXd& vec) const
        {
#if DART_VERSION_AT_LEAST(6, 7, 0)
            Eigen::VectorXd ret = Eigen::VectorXd::Zero(_dof);
            size_t k = 0;
            for (size_t i = _start_dof; i < _dof; i++) {
                auto it = std::find(_mimic_dofs.begin(), _mimic_dofs.end(), i);
                if (it == _mimic_dofs.end())
                    ret(i) = vec(k++);
            }

            return ret;
#else
            Eigen::VectorXd ret = Eigen::VectorXd::Zero(_dof);
            ret.tail(_control_dof) = vec;
            return ret;
#endif
        }
    } // namespace control
} // namespace robot_dart