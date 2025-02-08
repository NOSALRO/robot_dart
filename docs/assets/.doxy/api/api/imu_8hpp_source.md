

# File imu.hpp

[**File List**](files.md) **>** [**robot\_dart**](dir_166284c5f0440000a6384365f2a45567.md) **>** [**sensor**](dir_d1adb19f0b40b70b30ee0daf1901679b.md) **>** [**imu.hpp**](imu_8hpp.md)

[Go to the documentation of this file](imu_8hpp.md)


```C++
#ifndef ROBOT_DART_SENSOR_IMU_HPP
#define ROBOT_DART_SENSOR_IMU_HPP

#include <robot_dart/sensor/sensor.hpp>

namespace robot_dart {
    namespace sensor {
        // TO-DO: Implement some noise models (e.g., https://github.com/ethz-asl/kalibr/wiki/IMU-Noise-Model)
        struct IMUConfig {
            IMUConfig(dart::dynamics::BodyNode* b, size_t f) : gyro_bias(Eigen::Vector3d::Zero()), accel_bias(Eigen::Vector3d::Zero()), body(b), frequency(f){};
            IMUConfig(const Eigen::Vector3d& gyro_bias, const Eigen::Vector3d& accel_bias, dart::dynamics::BodyNode* b, size_t f) : gyro_bias(gyro_bias), accel_bias(accel_bias), body(b), frequency(f){};
            IMUConfig() : gyro_bias(Eigen::Vector3d::Zero()), accel_bias(Eigen::Vector3d::Zero()), body(nullptr), frequency(200) {}

            // We assume fixed bias; TO-DO: Make this time-dependent
            Eigen::Vector3d gyro_bias = Eigen::Vector3d::Zero();
            Eigen::Vector3d accel_bias = Eigen::Vector3d::Zero();

            // // We assume white Gaussian noise // TO-DO: Implement this
            // Eigen::Vector3d _gyro_std = Eigen::Vector3d::Zero();
            // Eigen::Vector3d _accel_std = Eigen::Vector3d::Zero();

            // BodyNode/Link attached to
            dart::dynamics::BodyNode* body = nullptr;
            // Eigen::Isometry3d _tf = Eigen::Isometry3d::Identity();

            // Frequency
            size_t frequency = 200;
        };

        class IMU : public Sensor {
        public:
            IMU(const IMUConfig& config);

            void init() override;

            void calculate(double) override;

            std::string type() const override;

            const Eigen::AngleAxisd& angular_position() const;
            Eigen::Vector3d angular_position_vec() const;
            const Eigen::Vector3d& angular_velocity() const;
            const Eigen::Vector3d& linear_acceleration() const;

            void attach_to_joint(dart::dynamics::Joint*, const Eigen::Isometry3d&) override
            {
                ROBOT_DART_WARNING(true, "You cannot attach an IMU sensor to a joint!");
            }

        protected:
            // double _prev_time = 0.;
            IMUConfig _config;

            Eigen::AngleAxisd _angular_pos; // TO-DO: Check how to do this as close as possible to real sensors
            Eigen::Vector3d _angular_vel;
            Eigen::Vector3d _linear_accel;
        };
    } // namespace sensor
} // namespace robot_dart

#endif
```


