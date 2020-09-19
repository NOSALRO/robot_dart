#include "force_torque.hpp"

#include <robot_dart/robot_dart_simu.hpp>

#include <dart/dynamics/BodyNode.hpp>
#include <dart/dynamics/Joint.hpp>

namespace robot_dart {
    namespace sensor {
        ForceTorque::ForceTorque(RobotDARTSimu* simu, const std::string& joint_name, size_t frequency, const std::string& direction) : Sensor(simu, frequency), _joint_name(joint_name), _direction(direction)
        {
            attach_to(joint_name, Eigen::Isometry3d::Identity());
        }

        void ForceTorque::init()
        {
            _force.setZero();
            _torque.setZero();

            attach_to(_joint_name, Eigen::Isometry3d::Identity());
            _active = true;
        }

        void ForceTorque::calculate(double t)
        {
            if (!_attached)
                return; // cannot compute anything if not attached to a joint

            // // Update transformations: in theory these should not change
            // if (_joint_attached->getParentBodyNode()) {
            //     _body_attached = _joint_attached->getParentBodyNode();
            //     _attached_tf = _joint_attached->getTransformFromParentBodyNode();
            // }
            // else if (_joint_attached->getChildBodyNode()) {
            //     _body_attached = _joint_attached->getChildBodyNode();
            //     _attached_tf = _joint_attached->getTransformFromChildBodyNode();
            // }
            auto robot = _robot.lock();

            Eigen::Vector6d F1, F2;
            std::tie(F1, F2) = robot->force_torque(robot->joint_index(_joint_name));

            // We always compute things in SENSOR frame (aka joint frame)
            if (_direction == "parent_to_child") {
                _force = F2.tail(3);
                _torque = F2.head(3);
            }
            else // "child_to_parent" (default)
            {
                _force = -F2.tail(3);
                _torque = -F2.head(3);
            }

            Eigen::Matrix3d R = _joint_attached->getTransformFromChildBodyNode().linear().transpose();
            _force = R * _force;
            _torque = R * _torque;
        }

        std::string ForceTorque::type() const { return "ft"; }

        const Eigen::Vector3d& ForceTorque::force() const
        {
            return _force;
        }

        const Eigen::Vector3d& ForceTorque::torque() const
        {
            return _torque;
        }

        void ForceTorque::attach_to(const std::string& joint_name, const Eigen::Isometry3d&)
        {
            _attach_to = joint_name;
            _attaching = true;
            _attached = false;
            _attached_tf = Eigen::Isometry3d::Identity();

            bool end = false;
            for (size_t i = 0; i < _simu->num_robots(); i++) {
                auto robot = _simu->robot(i);
                auto names = robot->joint_names();
                for (size_t j = 0; j < robot->num_joints(); j++) {
                    if (names[j] == joint_name) {
                        end = true;
                        _attaching = false;
                        _attached = true;

                        _joint_attached = robot->skeleton()->getJoint(joint_name);
                        _robot = robot;
                        if (_joint_attached->getParentBodyNode()) {
                            _body_attached = _joint_attached->getParentBodyNode();
                            _attached_tf = _joint_attached->getTransformFromParentBodyNode();
                        }
                        else if (_joint_attached->getChildBodyNode()) {
                            _body_attached = _joint_attached->getChildBodyNode();
                            _attached_tf = _joint_attached->getTransformFromChildBodyNode();
                        }
                        // else {
                        //     _attaching = true;
                        //     _attached = false;
                        // }

                        break;
                    }
                }

                if (end)
                    break;
            }
        }
    } // namespace sensor
} // namespace robot_dart