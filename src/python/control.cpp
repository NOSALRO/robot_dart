#include "robot_dart.hpp"

#include <pybind11/eigen.h>
#include <pybind11/functional.h>
#include <pybind11/stl.h>

#include <robot_dart/robot.hpp>
#include <robot_dart/robot_dart_simu.hpp>

#include <robot_dart/control/pd_control.hpp>
#include <robot_dart/control/robot_control.hpp>
#include <robot_dart/control/simple_control.hpp>

namespace robot_dart {
    namespace python {
        void py_control(py::module& m)
        {
            using namespace robot_dart;
            using namespace robot_dart::control;

            class PyRobotControl : public RobotControl {
            public:
                using RobotControl::RobotControl;

                /* Trampolines */
                void configure() override
                {
                    PYBIND11_OVERLOAD_PURE(
                        void, /* Return type */
                        RobotControl, /* Parent class */
                        configure, /* Name of function in C++ (must match Python name) */
                    );
                }

                Eigen::VectorXd calculate(double t) override
                {
                    PYBIND11_OVERLOAD_PURE(
                        Eigen::VectorXd, /* Return type */
                        RobotControl, /* Parent class */
                        calculate, /* Name of function in C++ (must match Python name) */
                        t);
                }

                std::shared_ptr<RobotControl> clone() const override
                {
                    PYBIND11_OVERLOAD_PURE(
                        std::shared_ptr<RobotControl>, /* Return type */
                        RobotControl, /* Parent class */
                        clone, /* Name of function in C++ (must match Python name) */
                    );
                }
            };

            class PublicistRobotControl : public RobotControl {
            public:
                using RobotControl::_active;
                using RobotControl::_ctrl;
                using RobotControl::_full_control;
                using RobotControl::_robot;
            };

            // RobotControl class
            py::class_<RobotControl, PyRobotControl, std::shared_ptr<RobotControl>>(m, "RobotControl")
                .def(py::init<const Eigen::VectorXd&, bool>())

                .def_readwrite("_active", &PublicistRobotControl::_active)
                .def_readwrite("_ctrl", &PublicistRobotControl::_ctrl)
                .def_readwrite("_full_control", &PublicistRobotControl::_full_control)
                .def_readonly("_robot", &PublicistRobotControl::_robot)

                .def("set_parameters", &RobotControl::set_parameters)
                .def("parameters", &RobotControl::parameters)

                .def("init", &RobotControl::init)

                .def("set_robot", &RobotControl::set_robot)
                .def("robot", &RobotControl::robot)

                .def("activate", &RobotControl::activate)
                .def("active", &RobotControl::active)

                .def("fully_controlled", &RobotControl::fully_controlled)
                .def("set_full_control", &RobotControl::set_full_control)

                .def("weight", &RobotControl::weight)
                .def("set_weight", &RobotControl::set_weight)

                // virtual void configure() = 0;
                .def("configure", &RobotControl::configure)

                .def("commands", &RobotControl::commands)

                // virtual Eigen::VectorXd calculate(double t) = 0;
                .def("calculate", &RobotControl::calculate)
                // virtual std::shared_ptr<RobotControl> clone() const = 0;
                .def("clone", &RobotControl::clone)

                .def("get_positions", &RobotControl::get_positions)
                .def("set_positions", &RobotControl::set_positions)

                .def("get_velocities", &RobotControl::get_velocities)
                .def("set_velocities", &RobotControl::set_velocities)

                .def("get_accelerations", &RobotControl::get_accelerations)
                .def("set_accelerations", &RobotControl::set_accelerations)

                .def("get_forces", &RobotControl::get_forces)
                .def("set_forces", &RobotControl::set_forces);

            // PDControl class
            py::class_<PDControl, RobotControl, std::shared_ptr<PDControl>>(m, "PDControl")
                .def(py::init<const Eigen::VectorXd&, bool>())

                .def("configure", &PDControl::configure)
                .def("calculate", &PDControl::calculate)

                .def("set_pd", (void (PDControl::*)(double, double)) & PDControl::set_pd)
                .def("set_pd", (void (PDControl::*)(const Eigen::VectorXd&, const Eigen::VectorXd&)) & PDControl::set_pd)

                .def("pd", &PDControl::pd)

                .def("clone", &PDControl::clone);

            // SimpleControl class
            py::class_<SimpleControl, RobotControl, std::shared_ptr<SimpleControl>>(m, "SimpleControl")
                .def(py::init<const Eigen::VectorXd&, bool>())

                .def("configure", &SimpleControl::configure)
                .def("calculate", &SimpleControl::calculate)

                .def("clone", &SimpleControl::clone);
        }
    } // namespace python
} // namespace robot_dart