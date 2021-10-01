#include "imu.hpp"

#include <robot_dart/robot_dart_simu.hpp>
#include <robot_dart/utils_headers_dart_dynamics.hpp>

namespace robot_dart {
    namespace sensor {
        IMU::IMU(const IMUConfig& config) : Sensor(config.frequency), _config(config) {}

        void IMU::init()
        {
            _angular_vel.setZero();
            _linear_accel.setZero();

            attach_to_body(_config.body, Eigen::Isometry3d::Identity());
            if (_simu)
                _active = true;
        }

        void IMU::calculate(double)
        {
            if (!_attached_to_body)
                return; // cannot compute anything if not attached to a link
            ROBOT_DART_EXCEPTION_ASSERT(_simu, "Simulation pointer is null!");

            Eigen::Vector3d tmp = dart::math::logMap(_body_attached->getTransform(dart::dynamics::Frame::World(), _body_attached).linear().matrix());
            _angular_pos = Eigen::AngleAxisd(tmp.norm(), tmp.normalized());
            _angular_vel = _body_attached->getSpatialVelocity().head(3); // angular velocity with respect to the world, in local coordinates
            _linear_accel = _body_attached->getSpatialAcceleration().tail(3); // linear acceleration with respect to the world, in local coordinates

            // add biases
            _angular_vel += _config.gyro_bias;
            _linear_accel += _config.accel_bias;

            // add gravity to acceleration
            _linear_accel -= _world_pose.linear().transpose() * _simu->gravity();
        }

        std::string IMU::type() const { return "imu"; }

        const Eigen::AngleAxisd& IMU::angular_position() const
        {
            return _angular_pos;
        }

        Eigen::Vector3d IMU::angular_position_vec() const
        {
            return _angular_pos.angle() * _angular_pos.axis();
        }

        const Eigen::Vector3d& IMU::angular_velocity() const
        {
            return _angular_vel;
        }

        const Eigen::Vector3d& IMU::linear_acceleration() const
        {
            return _linear_accel;
        }
    } // namespace sensor
} // namespace robot_dart