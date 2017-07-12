#ifndef ROBOT_DART_POSITION_CONTROL
#define ROBOT_DART_POSITION_CONTROL

#include <robot_dart/robot_control.hpp>

namespace robot_dart {

    class PositionControl : public RobotControl {
    public:
        using robot_t = std::shared_ptr<Robot>;

        PositionControl() {}
        PositionControl(const std::vector<double>& ctrl, robot_t robot)
            : RobotControl(ctrl, robot), _control_root_joint(true)
        {
            init();
        }

        void init()
        {
            _dof = _robot->skeleton()->getNumDofs();

            _start_dof = 0;
            if (!_robot->fixed_to_world() && _control_root_joint) {
                _start_dof = _robot->skeleton()->getRootBodyNode()->getParentJoint()->getNumDofs();
            }

            std::vector<size_t> indices;
            std::vector<dart::dynamics::Joint::ActuatorType> types;
            for (size_t i = _start_dof; i < _dof; i++) {
                auto j = _robot->skeleton()->getDof(i)->getJoint();
                indices.push_back(_robot->skeleton()->getIndexOf(j));
                types.push_back(dart::dynamics::Joint::VELOCITY);
            }
            _robot->set_actuator_types(indices, types);
        }

        void set_commands()
        {
            assert(_dof == _ctrl.size());
            Eigen::VectorXd target_positions = Eigen::VectorXd::Map(_ctrl.data(), _ctrl.size());
            Eigen::VectorXd q = _robot->skeleton()->getPositions();
            Eigen::VectorXd q_err = target_positions - q;

            double gain = 1.0 / (dart::math::constants<double>::pi() * _robot->skeleton()->getTimeStep());
            Eigen::VectorXd vel = q_err * gain;
            if (_start_dof > 0)
                vel.segment(0, _start_dof) = Eigen::VectorXd::Zero(_start_dof);

            _robot->skeleton()->setCommands(vel);
        }

        void control_root_joint(bool enable = true) { _control_root_joint = enable; }
        bool root_joint_controlled() { return _control_root_joint; }

    protected:
        size_t _start_dof;
        bool _control_root_joint;
    };
}

#endif
