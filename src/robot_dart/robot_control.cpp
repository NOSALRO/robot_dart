#include <robot_dart/robot_control.hpp>

#include <dart/dynamics/DegreeOfFreedom.hpp>

#include <robot_dart/robot.hpp>

namespace robot_dart {

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
} // namespace robot_dart