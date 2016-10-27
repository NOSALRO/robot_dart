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
              mSpeed(0.0)
        {
            _robot->set_actuator_types(dart::dynamics::Joint::FORCE);
            auto skel = _robot->skeleton();

            mForces = Eigen::VectorXd::Zero(_dof);
            mKp = Eigen::MatrixXd::Identity(_dof, _dof);
            mKd = Eigen::MatrixXd::Identity(_dof, _dof);

            size_t start_dim = 0;
            if (!_robot->fixed_to_world()) {
                for (size_t i = 0; i < 6; ++i) {
                    mKp(i, i) = 0.0;
                    mKd(i, i) = 0.0;
                }
                start_dim = 6;
            }

            for (size_t i = start_dim; i < _dof; ++i) {
                mKp(i, i) = 1000;
                mKd(i, i) = 50;
            }

            mTargetPositions = skel->getPositions();
        }

        void set_commands()
        {
            assert(_dof == _ctrl.size());

            auto skel = _robot->skeleton();
            mTargetPositions = Eigen::VectorXd::Map(_ctrl.data(), _ctrl.size());
            _reset();
            _add_spd_forces();
        }

    protected:
        Eigen::VectorXd mForces;
        Eigen::MatrixXd mKp;
        Eigen::MatrixXd mKd;
        Eigen::VectorXd mTargetPositions;
        double mSpeed;

        void _reset()
        {
            mForces.setZero();
        }

        void _add_spd_forces()
        {
            auto skel = _robot->skeleton();

            Eigen::VectorXd q = skel->getPositions();
            Eigen::VectorXd dq = skel->getVelocities();

            Eigen::MatrixXd invM = (skel->getMassMatrix()
                                       + mKd * skel->getTimeStep())
                                       .inverse();
            Eigen::VectorXd p = -mKp * (q + dq * skel->getTimeStep() - mTargetPositions);
            Eigen::VectorXd d = -mKd * dq;
            Eigen::VectorXd qddot = invM * (-skel->getCoriolisAndGravityForces()
                                               + p + d + skel->getConstraintForces());

            mForces += p + d - mKd * qddot * skel->getTimeStep();
            skel->setForces(mForces);
        }
    };
}

#endif
