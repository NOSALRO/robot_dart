#include "robot_dart.hpp"

#include <pybind11/eigen.h>
#include <pybind11/operators.h>
#include <pybind11/stl.h>

#include <robot_dart/robot_dart_simu.hpp>
#include <robot_dart/sensor/sensor.hpp>

namespace robot_dart {
    namespace python {
        void py_sensors(py::module& m)
        {
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
                        init,
                        /* arguments */
                    );
                }

                void calculate(double t) override
                {
                    PYBIND11_OVERLOAD_PURE(
                        void, /* return type */
                        Sensor, /* parent class */
                        calculate,
                        t
                        /* arguments */
                    );
                }

                std::string type() const override
                {
                    PYBIND11_OVERLOAD_PURE(
                        std::string, /* return type */
                        Sensor, /* parent class */
                        type,
                        /* arguments */
                    );
                }
            };

            class PublicistSensor : public Sensor {
            public:
                using Sensor::_active;
                using Sensor::_frequency;
                using Sensor::_simu;
            };

            py::class_<Sensor, PySensor, std::shared_ptr<Sensor>>(m, "Sensor")
                .def(py::init<RobotDARTSimu*, size_t>(),
                    py::arg("simu"),
                    py::arg("freq") = 40)

                .def_readwrite("_active", &PublicistSensor::_active)
                .def_readwrite("_frequency", &PublicistSensor::_frequency)
                .def_readonly("_simu", &PublicistSensor::_simu)

                .def("activate", &Sensor::activate,
                    py::arg("enable") = true)
                .def("active", &Sensor::active)

                .def("frequency", &Sensor::frequency)
                .def("set_frequency", &Sensor::set_frequency,
                    py::arg("freq"))

                .def("set_pose", &Sensor::set_pose,
                    py::arg("tf"))
                .def("pose", &Sensor::pose)

                .def("refresh", &Sensor::refresh,
                    py::arg("t"))

                .def("init", &Sensor::init)
                .def("calculate", &Sensor::calculate)
                .def("type", &Sensor::calculate);
        }
    } // namespace python
} // namespace robot_dart