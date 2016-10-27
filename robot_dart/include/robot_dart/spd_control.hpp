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
              _stiffness_coeff(1000),
              _damping_coeff(50)
        {
            _robot->set_actuator_types(dart::dynamics::Joint::FORCE);
            init();
        }

        void set_commands()
        {
            assert(_dof == _ctrl.size());

            auto skel = _robot->skeleton();
            _mTargetPositions = Eigen::VectorXd::Map(_ctrl.data(), _ctrl.size());
            _add_spd_forces();
        }

        void init()
        {
            auto skel = _robot->skeleton();
            _dof = skel->getNumDofs();
            _mForces = Eigen::VectorXd::Zero(_dof);
            _mKp = Eigen::MatrixXd::Identity(_dof, _dof);
            _mKd = Eigen::MatrixXd::Identity(_dof, _dof);

            size_t start_dim = 0;
            if (!_robot->fixed_to_world()) {
                for (size_t i = 0; i < 6; ++i) {
                    _mKp(i, i) = 0.0;
                    _mKd(i, i) = 0.0;
                }
                start_dim = 6;
            }

            for (size_t i = start_dim; i < _dof; ++i) {
                _mKp(i, i) = _stiffness_coeff;
                _mKd(i, i) = _damping_coeff;
            }

            _mTargetPositions = skel->getPositions();
        }

        void set_pd(double p, double d)
        {
            _stiffness_coeff = p;
            _damping_coeff = d;
            init();
        }

    protected:
        Eigen::VectorXd _mForces;
        Eigen::MatrixXd _mKp;
        Eigen::MatrixXd _mKd;
        Eigen::VectorXd _mTargetPositions;
        double _stiffness_coeff;
        double _damping_coeff;

        void _add_spd_forces()
        {
            auto skel = _robot->skeleton();

            Eigen::VectorXd q = skel->getPositions();
            Eigen::VectorXd dq = skel->getVelocities();

            Eigen::MatrixXd invM = (skel->getMassMatrix()
                                       + _mKd * skel->getTimeStep())
                                       .inverse();
            Eigen::VectorXd p = -_mKp * (q + dq * skel->getTimeStep() - _mTargetPositions);
            Eigen::VectorXd d = -_mKd * dq;
            Eigen::VectorXd qddot = invM * (-skel->getCoriolisAndGravityForces()
                                               + p + d + skel->getConstraintForces());

            _mForces = p + d - _mKd * qddot * skel->getTimeStep();
            skel->setForces(_mForces);
        }
    };
}

#endif
