#ifndef ROBOT_DART_PD_CONTROL
#define ROBOT_DART_PD_CONTROL

#include <robot_dart/robot_control.hpp>

namespace robot_dart {

    class PDControl : public RobotControl {
    public:
        using robot_t = std::shared_ptr<Robot>;

        PDControl() {}
        PDControl(const std::vector<double>& ctrl, robot_t robot)
            : RobotControl(ctrl, robot)
        {
            init();

            // Default values for PD controller
            _Kp = 200.0;
            _Kd = 40.0;
        }

        void init()
        {
            _dof = _robot->skeleton()->getNumDofs();

            _start_dof = 0;
            if (!_robot->fixed_to_world()) {
                _start_dof = 6;
            }

            std::vector<size_t> indices;
            std::vector<dart::dynamics::Joint::ActuatorType> types;
            for (size_t i = _start_dof; i < _dof; i++) {
                auto j = _robot->skeleton()->getDof(i)->getJoint();
                indices.push_back(_robot->skeleton()->getIndexOf(j));
                types.push_back(dart::dynamics::Joint::FORCE);
            }
            _robot->set_actuator_types(indices, types);
        }

        void set_commands()
        {
            assert(_dof == _ctrl.size());
            Eigen::VectorXd target_positions = Eigen::VectorXd::Map(_ctrl.data(), _ctrl.size());

            Eigen::VectorXd q = _robot->skeleton()->getPositions();
            Eigen::VectorXd dq = _robot->skeleton()->getVelocities();

            q += dq * _robot->skeleton()->getTimeStep();

            Eigen::VectorXd q_err = target_positions - q;
            Eigen::VectorXd dq_err = -dq;

            const Eigen::MatrixXd& M = _robot->skeleton()->getMassMatrix();
            const Eigen::VectorXd& Cg = _robot->skeleton()->getCoriolisAndGravityForces();

            Eigen::VectorXd commands = M * (_Kp * q_err + _Kd * dq_err) + Cg;
            if (_start_dof > 0)
                commands.segment(0, _start_dof) = Eigen::VectorXd::Zero(_start_dof);
            _robot->skeleton()->setCommands(commands);
        }

        void set_pd(double p, double d)
        {
            _Kp = p;
            _Kd = d;
            init();
        }

    protected:
        double _Kp, _Kd;
        size_t _start_dof;
    };
}

#endif
