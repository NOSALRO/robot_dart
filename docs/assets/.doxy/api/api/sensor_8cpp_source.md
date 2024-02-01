

# File sensor.cpp

[**File List**](files.md) **>** [**robot\_dart**](dir_166284c5f0440000a6384365f2a45567.md) **>** [**sensor**](dir_d1adb19f0b40b70b30ee0daf1901679b.md) **>** [**sensor.cpp**](sensor_8cpp.md)

[Go to the documentation of this file](sensor_8cpp.md)

```C++

#include "sensor.hpp"
#include "robot_dart/robot_dart_simu.hpp"
#include "robot_dart/utils.hpp"
#include "robot_dart/utils_headers_dart_dynamics.hpp"

namespace robot_dart {
    namespace sensor {
        Sensor::Sensor(size_t freq) : _active(false), _frequency(freq), _world_pose(Eigen::Isometry3d::Identity()), _attaching_to_body(false), _attached_to_body(false), _attaching_to_joint(false), _attached_to_joint(false) {}

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

        void Sensor::set_simu(RobotDARTSimu* simu)
        {
            ROBOT_DART_EXCEPTION_ASSERT(simu, "Simulation pointer is null!");
            _simu = simu;
            bool check = static_cast<int>(_frequency) > simu->physics_freq();
            ROBOT_DART_WARNING(check, "Sensor frequency is bigger than simulation physics. Setting it to simulation rate!");
            if (check)
                _frequency = simu->physics_freq();
        }

        const RobotDARTSimu* Sensor::simu() const
        {
            return _simu;
        }

        size_t Sensor::frequency() const { return _frequency; }
        void Sensor::set_frequency(size_t freq) { _frequency = freq; }

        void Sensor::set_pose(const Eigen::Isometry3d& tf) { _world_pose = tf; }
        const Eigen::Isometry3d& Sensor::pose() const { return _world_pose; }

        void Sensor::detach()
        {
            _attached_to_body = false;
            _attached_to_joint = false;
            _body_attached = nullptr;
            _joint_attached = nullptr;
            _active = false;
        }

        void Sensor::refresh(double t)
        {
            if (!_active)
                return;
            if (_attaching_to_body && !_attached_to_body) {
                attach_to_body(_body_attached, _attached_tf);
            }
            else if (_attaching_to_joint && !_attached_to_joint) {
                attach_to_joint(_joint_attached, _attached_tf);
            }

            if (_attached_to_body && _body_attached) {
                _world_pose = _body_attached->getWorldTransform() * _attached_tf;
            }
            else if (_attached_to_joint && _joint_attached) {
                dart::dynamics::BodyNode* body = nullptr;
                Eigen::Isometry3d tf = Eigen::Isometry3d::Identity();

                if (_joint_attached->getParentBodyNode()) {
                    body = _joint_attached->getParentBodyNode();
                    tf = _joint_attached->getTransformFromParentBodyNode();
                }
                else if (_joint_attached->getChildBodyNode()) {
                    body = _joint_attached->getChildBodyNode();
                    tf = _joint_attached->getTransformFromChildBodyNode();
                }

                if (body)
                    _world_pose = body->getWorldTransform() * tf * _attached_tf;
            }
            calculate(t);
        }

        void Sensor::attach_to_body(dart::dynamics::BodyNode* body, const Eigen::Isometry3d& tf)
        {
            _body_attached = body;
            _attached_tf = tf;

            if (_body_attached) {
                _attaching_to_body = false;
                _attached_to_body = true;

                _attaching_to_joint = false;
                _attached_to_joint = false;
                _joint_attached = nullptr;
            }
            else { // we cannot keep attaching to a null BodyNode
                _attaching_to_body = false;
                _attached_to_body = false;
            }
        }

        void Sensor::attach_to_joint(dart::dynamics::Joint* joint, const Eigen::Isometry3d& tf)
        {
            _joint_attached = joint;
            _attached_tf = tf;

            if (_joint_attached) {
                _attaching_to_joint = false;
                _attached_to_joint = true;

                _attaching_to_body = false;
                _attached_to_body = false;
            }
            else { // we cannot keep attaching to a null Joint
                _attaching_to_joint = false;
                _attached_to_joint = false;
            }
        }
        const std::string& Sensor::attached_to() const
        {
            ROBOT_DART_EXCEPTION_ASSERT(_attached_to_body || _attached_to_joint, "Joint is not attached to anything");
            if (_attached_to_body)
                return _body_attached->getName();
            // attached to joint
            return _joint_attached->getName();
        }
    } // namespace sensor
} // namespace robot_dart

```

