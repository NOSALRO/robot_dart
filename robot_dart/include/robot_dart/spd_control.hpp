#ifndef ROBOT_DART_SPD_CONTROL
#define ROBOT_DART_SPD_CONTROL

#include <robot_dart/robot_control.hpp>

namespace robot_dart {

    class SPDControl : public RobotControl {
    public:
        using robot_t = std::shared_ptr<Robot>;

        SPDControl() {}
        SPDControl(const std::vector<double>& ctrl, robot_t robot)
            : RobotControl(ctrl, robot),
              _Kp(1500),
              _Kd(50)
        {
            init();
        }

        void set_commands()
        {
            assert(_dof == _ctrl.size());

            auto skel = _robot->skeleton();
            Eigen::VectorXd target_positions = Eigen::VectorXd::Map(_ctrl.data(), _ctrl.size());

            Eigen::VectorXd q = skel->getPositions();
            Eigen::VectorXd dq = skel->getVelocities();

            Eigen::MatrixXd invM = (skel->getMassMatrix()
                                       + _mKd * skel->getTimeStep())
                                       .inverse();
            Eigen::VectorXd p = -_mKp * (q + dq * skel->getTimeStep() - target_positions);
            Eigen::VectorXd d = -_mKd * dq;
            Eigen::VectorXd qddot = invM * (-skel->getCoriolisAndGravityForces() + p + d);

            Eigen::VectorXd forces = p + d - _mKd * qddot * skel->getTimeStep();
            skel->setForces(forces);
        }

        void init()
        {
            auto skel = _robot->skeleton();
            _dof = skel->getNumDofs();
            _robot->set_actuator_types(dart::dynamics::Joint::FORCE);

            _mForces = Eigen::VectorXd::Zero(_dof);
            _mKp = Eigen::MatrixXd::Identity(_dof, _dof);
            _mKd = Eigen::MatrixXd::Identity(_dof, _dof);

            size_t start_dof = 0;
            if (!_robot->fixed_to_world()) {
                for (size_t i = 0; i < 6; ++i) {
                    _mKp(i, i) = 0.0;
                    _mKd(i, i) = 0.0;
                }
                start_dof = 6;
            }

            for (size_t i = start_dof; i < _dof; ++i) {
                _mKp(i, i) = _Kp;
                _mKd(i, i) = _Kd;
            }
        }

        void set_pd(double p, double d)
        {
            _Kp = p;
            _Kd = d;
            init();
        }

    protected:
        Eigen::VectorXd _mForces;
        Eigen::MatrixXd _mKp, _mKd;
        double _Kp, _Kd;
    };
}

#endif
