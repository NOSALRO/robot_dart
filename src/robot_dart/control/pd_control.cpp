#include "pd_control.hpp"
#include "robot_dart/robot.hpp"
#include "robot_dart/utils.hpp"

namespace robot_dart {
    namespace control {
        PDControl::PDControl() : RobotControl() {}
        PDControl::PDControl(const std::vector<double>& ctrl, bool full_control) : RobotControl(ctrl, full_control) {}

        void PDControl::configure()
        {
            if (_ctrl.size() == _control_dof)
                _active = true;

	    set_pd(10.,0.1);
        }

        Eigen::VectorXd PDControl::calculate(double)
        {
            ROBOT_DART_ASSERT(_control_dof == _ctrl.size(), "PDControl: Controller parameters size is not the same as DOFs of the robot", Eigen::VectorXd::Zero(_control_dof));
            auto robot = _robot.lock();
            Eigen::VectorXd target_positions = Eigen::VectorXd::Zero(_dof);
            target_positions.tail(_control_dof) = Eigen::VectorXd::Map(_ctrl.data(), _ctrl.size());

            Eigen::VectorXd q = robot->skeleton()->getPositions();
            Eigen::VectorXd dq = robot->skeleton()->getVelocities();

            /// Compute the simplest PD controller output:
            /// P gain * (target position - current position) + D gain * (0 - current velocity)
            Eigen::VectorXd commands = _Kp.array() * (target_positions.array() - q.array()) - _Kd.array() * dq.array();

            return commands.tail(_control_dof);
        }

        void PDControl::set_pd(double p, double d)
        {
	    if(_control_dof != 1) {
		std::cout << "[WARNING] Setting all the gains to Kp = " << p << " and Kd = " << d << std::endl;
	    }
            _Kp = Eigen::VectorXd::Constant(_control_dof, p);
            _Kd = Eigen::VectorXd::Constant(_control_dof, d);
        }

        void PDControl::set_pd(const Eigen::VectorXd& p, const Eigen::VectorXd& d)
        {
	    assert((size_t)p.size() == _control_dof);
	    assert((size_t)d.size() == _control_dof);
            _Kp = p;
            _Kd = d;
        }

        std::pair<double, double> PDControl::pd() const
        {
	    assert(_control_dof == 1);
            return std::make_pair(_Kp(0), _Kd(0));
        }

        void PDControl::pd(Eigen::VectorXd& p, Eigen::VectorXd& d) const
        {
            p = _Kp;
            d = _Kd;
        }

        std::shared_ptr<RobotControl> PDControl::clone() const
        {
            return std::make_shared<PDControl>(*this);
        }
    } // namespace control
} // namespace robot_dart
