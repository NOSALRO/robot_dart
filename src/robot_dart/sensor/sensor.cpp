#include "sensor.hpp"
#include "robot_dart/robot_dart_simu.hpp"
#include "robot_dart/utils.hpp"

#include <dart/dynamics/BodyNode.hpp>

namespace robot_dart {
    namespace sensor {
        Sensor::Sensor(RobotDARTSimu* simu, size_t freq) : _simu(simu), _active(false), _frequency(freq), _world_pose(Eigen::Isometry3d::Identity()), _attaching(false), _attached(false) {}

        void Sensor::activate(bool enable)
        {
            _active = false;
            if (enable) {
                init();
            }
        }

        bool Sensor::active() const
        {
            return _active;
        }

        size_t Sensor::frequency() const { return _frequency; }
        void Sensor::set_frequency(size_t freq) { _frequency = freq; }

        void Sensor::set_pose(const Eigen::Isometry3d& tf) { _world_pose = tf; }
        const Eigen::Isometry3d& Sensor::pose() const { return _world_pose; }

        void Sensor::refresh(double t)
        {
            if (_attaching && !_attached) {
                attach_to(_attach_to, _attached_tf);
            }

            if (_attached) {
                _world_pose = _body_attached->getWorldTransform() * _attached_tf;
            }

            calculate(t);
        }

        void Sensor::attach_to(const std::string& body_name, const Eigen::Isometry3d& tf)
        {
            _attach_to = body_name;
            _attaching = true;
            _attached = false;
            _attached_tf = tf;

            bool end = false;
            for (size_t i = 0; i < _simu->num_robots(); i++) {
                auto robot = _simu->robot(i);
                auto names = robot->body_names();
                for (size_t j = 0; j < robot->num_bodies(); j++) {
                    if (names[j] == body_name) {
                        end = true;
                        _attaching = false;
                        _attached = true;

                        _body_attached = robot->skeleton()->getBodyNode(body_name);

                        break;
                    }
                }

                if (end)
                    break;
            }
        }
    } // namespace sensor
} // namespace robot_dart