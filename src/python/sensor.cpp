#include "robot_dart.hpp"
#include "utils_headers_dart.hpp"
#include "utils_headers_pybind11.hpp"

#include <robot_dart/robot_dart_simu.hpp>
#include <robot_dart/sensor/force_torque.hpp>
#include <robot_dart/sensor/imu.hpp>
#include <robot_dart/sensor/sensor.hpp>
#include <robot_dart/sensor/torque.hpp>

namespace robot_dart {
    namespace python {
        void py_sensors(py::module& m) 
        {
            auto sensormodule = m.def_submodule("sensor");

            using namespace robot_dart;
            using Sensor = sensor::Sensor;

            // Sensor class
            class PySensor : public Sensor {
            public:
                using Sensor::Sensor;

                /* Trampolines */
                void init() override 
                {
                    PYBIND11_OVERLOAD_PURE(
                        void, /* return type */
                        Sensor, /* parent class */
                        init, /* name */
                        /* arguments */
                    );
                }

                void calculate(double t) override 
                {
                    PYBIND11_OVERLOAD_PURE(
                        void, /* return type */
                        Sensor, /* parent class */
                        calculate, /* name */
                        t
                        /* arguments */
                    );
                }

                std::string type() const override 
                {
                    PYBIND11_OVERLOAD_PURE(
                        std::string, /* return type */
                        Sensor, /* parent class */
                        type, /* name */
                        /* arguments */
                    );
                }

                void attach_to_body(dart::dynamics::BodyNode* body, const Eigen::Isometry3d& tf = Eigen::Isometry3d::Identity()) override 
                {
                    PYBIND11_OVERLOAD(
                        void, /* return type */
                        Sensor, /* parent class */
                        attach_to_body, /* name */
                        body,
                        tf
                        /* arguments */
                    );
                }

                void attach_to_joint(dart::dynamics::Joint* joint, const Eigen::Isometry3d& tf = Eigen::Isometry3d::Identity()) override 
                {
                    PYBIND11_OVERLOAD(
                        void, /* return type */
                        Sensor, /* parent class */
                        attach_to_joint, /* name */
                        joint,
                        tf
                        /* arguments */
                    );
                }
            };

            class PublicistSensor : public Sensor {
            public:
                using Sensor::_active;
                using Sensor::_frequency;
                using Sensor::_simu;

                using Sensor::_world_pose;

                using Sensor::_attached_to_body;
                using Sensor::_attached_to_joint;
                using Sensor::_attaching_to_body;
                using Sensor::_attaching_to_joint;

                using Sensor::_attached_tf;
                using Sensor::_body_attached;
                using Sensor::_joint_attached;
            };

            py::class_<Sensor, PySensor, std::shared_ptr<Sensor>>(sensormodule, "Sensor")
                .def(py::init<size_t>(),
                    py::arg("freq") = 40)

                .def_readwrite("_active", &PublicistSensor::_active)
                .def_readwrite("_frequency", &PublicistSensor::_frequency)
                .def_readonly("_simu", &PublicistSensor::_simu)

                .def_readwrite("_attached_to_body", &PublicistSensor::_attached_to_body)
                .def_readwrite("_attached_to_joint", &PublicistSensor::_attached_to_joint)
                .def_readwrite("_attaching_to_body", &PublicistSensor::_attaching_to_body)
                .def_readwrite("_attaching_to_joint", &PublicistSensor::_attaching_to_joint)
                .def_readwrite("_attached_tf", &PublicistSensor::_attached_tf)
                .def_readwrite("_body_attached", &PublicistSensor::_body_attached)
                .def_readwrite("_joint_attached", &PublicistSensor::_joint_attached)

                .def("activate", &Sensor::activate,
                    py::arg("enable") = true)
                .def("active", &Sensor::active)

                .def("set_simu", &Sensor::set_simu)
                .def("simu", &Sensor::simu, py::return_value_policy::reference)

                .def("frequency", &Sensor::frequency)
                .def("set_frequency", &Sensor::set_frequency,
                    py::arg("freq"))

                .def("set_pose", &Sensor::set_pose,
                    py::arg("tf"))
                .def("pose", &Sensor::pose)

                .def("refresh", &Sensor::refresh,
                    py::arg("t"))

                .def("init", &Sensor::init)
                .def("calculate", &Sensor::calculate,
                    py::arg("t"))
                .def("type", &Sensor::type)

                .def("attach_to_body", static_cast<void (Sensor::*)(dart::dynamics::BodyNode*, const Eigen::Isometry3d& tf)>(&Sensor::attach_to_body),
                    py::arg("body"),
                    py::arg("tf") = Eigen::Isometry3d::Identity())

                .def("attach_to_body", static_cast<void (Sensor::*)(const std::shared_ptr<Robot>&, const std::string&, const Eigen::Isometry3d& tf)>(&Sensor::attach_to_body),
                    py::arg("robot"),
                    py::arg("body_name"),
                    py::arg("tf") = Eigen::Isometry3d::Identity())

                .def("attach_to_joint", static_cast<void (Sensor::*)(dart::dynamics::Joint*, const Eigen::Isometry3d& tf)>(&Sensor::attach_to_joint),
                    py::arg("joint"),
                    py::arg("tf") = Eigen::Isometry3d::Identity())

                .def("attach_to_joint", static_cast<void (Sensor::*)(const std::shared_ptr<Robot>&, const std::string&, const Eigen::Isometry3d& tf)>(&Sensor::attach_to_joint),
                    py::arg("robot"),
                    py::arg("joint_name"),
                    py::arg("tf") = Eigen::Isometry3d::Identity());

            // Torque Sensor class
            class PublicistTSensor : public sensor::Torque {
            public:
                using sensor::Torque::_torques;
            };

            py::class_<sensor::Torque, Sensor, std::shared_ptr<sensor::Torque>>(sensormodule, "Torque")
                .def(py::init<dart::dynamics::Joint*, size_t>(),
                    py::arg("joint"),
                    py::arg("frequency") = 1000)
                .def(py::init<const std::shared_ptr<Robot>&, const std::string&, size_t>(),
                    py::arg("robot"),
                    py::arg("joint_name"),
                    py::arg("frequency") = 1000)

                .def_readonly("_torques", &PublicistTSensor::_torques)

                .def("init", &sensor::Torque::init)
                .def("calculate", &sensor::Torque::calculate,
                    py::arg("t"))
                .def("type", &sensor::Torque::type)

                .def("torques", &sensor::Torque::torques)

                .def("attach_to_body", static_cast<void (sensor::Torque::*)(dart::dynamics::BodyNode*, const Eigen::Isometry3d& tf)>(&sensor::Torque::attach_to_body),
                    py::arg("body"),
                    py::arg("tf") = Eigen::Isometry3d::Identity());

            // Force-Torque Sensor Class
            class PublicistFTSensor : public sensor::ForceTorque {
            public:
                using sensor::ForceTorque::_direction;

                using sensor::ForceTorque::_wrench;
            };

            py::class_<sensor::ForceTorque, Sensor, std::shared_ptr<sensor::ForceTorque>>(sensormodule, "ForceTorque")
                .def(py::init<dart::dynamics::Joint*, size_t, const std::string&>(),
                    py::arg("joint"),
                    py::arg("frequency") = 1000,
                    py::arg("direction") = "child_to_parent")
                .def(py::init<const std::shared_ptr<Robot>&, const std::string&, size_t, const std::string&>(),
                    py::arg("robot"),
                    py::arg("joint_name"),
                    py::arg("frequency") = 1000,
                    py::arg("direction") = "child_to_parent")

                .def_readwrite("_direction", &PublicistFTSensor::_direction)
                .def_readonly("_wrench", &PublicistFTSensor::_wrench)

                .def("init", &sensor::ForceTorque::init)
                .def("calculate", &sensor::ForceTorque::calculate,
                    py::arg("t"))
                .def("type", &sensor::ForceTorque::type)

                .def("force", &sensor::ForceTorque::force)
                .def("torque", &sensor::ForceTorque::torque)
                .def("wrench", &sensor::ForceTorque::wrench)

                .def("attach_to_body", static_cast<void (sensor::ForceTorque::*)(dart::dynamics::BodyNode*, const Eigen::Isometry3d& tf)>(&sensor::ForceTorque::attach_to_body),
                    py::arg("body"),
                    py::arg("tf") = Eigen::Isometry3d::Identity());

            // IMU Sensor Class
            py::class_<sensor::IMUConfig>(sensormodule, "IMUConfig")
                .def(py::init<const Eigen::Vector3d&, const Eigen::Vector3d&, dart::dynamics::BodyNode*, size_t>(),
                    py::arg("gyro_bias") = Eigen::Vector3d::Zero(),
                    py::arg("accel_bias") = Eigen::Vector3d::Zero(),
                    py::arg("body") = nullptr,
                    py::arg("frequency") = 200)
                .def(py::init<dart::dynamics::BodyNode*, size_t>(),
                    py::arg("body") = nullptr,
                    py::arg("frequency") = 200)

                .def_readwrite("gyro_bias", &sensor::IMUConfig::gyro_bias)
                .def_readwrite("accel_bias", &sensor::IMUConfig::accel_bias)
                .def_readwrite("body", &sensor::IMUConfig::body)
                .def_readwrite("frequency", &sensor::IMUConfig::frequency);

            class PublicistIMUSensor : public sensor::IMU {
            public:
                using sensor::IMU::_config;

                using sensor::IMU::_angular_pos;
                using sensor::IMU::_angular_vel;
                using sensor::IMU::_linear_accel;
            };

            py::class_<sensor::IMU, Sensor, std::shared_ptr<sensor::IMU>>(sensormodule, "IMU")
                .def(py::init<const sensor::IMUConfig&>(),
                    py::arg("config"))

                .def_readonly("_config", &PublicistIMUSensor::_config)
                .def_readonly("_angular_pos", &PublicistIMUSensor::_angular_pos)
                .def_readonly("_angular_vel", &PublicistIMUSensor::_angular_vel)
                .def_readonly("_linear_accel", &PublicistIMUSensor::_linear_accel)

                .def("init", &sensor::IMU::init)
                .def("calculate", &sensor::IMU::calculate,
                    py::arg("t"))
                .def("type", &sensor::IMU::type)

                .def("angular_position", &sensor::IMU::angular_position)
                .def("angular_position_vec", &sensor::IMU::angular_position_vec)
                .def("angular_velocity", &sensor::IMU::angular_velocity)
                .def("linear_acceleration", &sensor::IMU::linear_acceleration)

                .def("attach_to_joint", static_cast<void (sensor::IMU::*)(dart::dynamics::Joint*, const Eigen::Isometry3d& tf)>(&sensor::ForceTorque::attach_to_joint),
                    py::arg("joint"),
                    py::arg("tf") = Eigen::Isometry3d::Identity());
        }
    } // namespace python
} // namespace robot_dart