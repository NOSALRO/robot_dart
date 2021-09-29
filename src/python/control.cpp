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
                using RobotControl::_check_free;
                using RobotControl::_controllable_dofs;
                using RobotControl::_ctrl;
                using RobotControl::_robot;
            };

            // RobotControl class
            py::class_<RobotControl, PyRobotControl, std::shared_ptr<RobotControl>>(m, "RobotControl")
                .def(py::init<>())
                .def(py::init<const Eigen::VectorXd&, bool>(),
                    py::arg("ctrl"),
                    py::arg("full_control") = false)
                .def(py::init<const Eigen::VectorXd&, const std::vector<std::string>&>(),
                    py::arg("ctrl"),
                    py::arg("controllable_dofs"))

                .def_readwrite("_active", &PublicistRobotControl::_active)
                .def_readwrite("_check_free", &PublicistRobotControl::_check_free)
                .def_readonly("_controllable_dofs", &PublicistRobotControl::_controllable_dofs)
                .def_readwrite("_ctrl", &PublicistRobotControl::_ctrl)
                .def_readonly("_robot", &PublicistRobotControl::_robot)

                .def("set_parameters", &RobotControl::set_parameters)
                .def("parameters", &RobotControl::parameters)

                .def("init", &RobotControl::init)

                .def("set_robot", &RobotControl::set_robot)
                .def("robot", &RobotControl::robot)

                .def("activate", &RobotControl::activate)
                .def("active", &RobotControl::active)

                .def("controllable_dofs", &RobotControl::controllable_dofs)

                .def("weight", &RobotControl::weight)
                .def("set_weight", &RobotControl::set_weight)

                // virtual void configure() = 0;
                .def("configure", &RobotControl::configure)

                // virtual Eigen::VectorXd calculate(double t) = 0;
                .def("calculate", &RobotControl::calculate)
                // virtual std::shared_ptr<RobotControl> clone() const = 0;
                .def("clone", &RobotControl::clone);

            // PDControl class
            py::class_<PDControl, RobotControl, std::shared_ptr<PDControl>>(m, "PDControl")
                .def(py::init<>())
                .def(py::init<const Eigen::VectorXd&, bool, bool>(),
                    py::arg("ctrl"),
                    py::arg("full_control") = false,
                    py::arg("use_angular_errors") = true)
                .def(py::init<const Eigen::VectorXd&, const std::vector<std::string>&, bool>(),
                    py::arg("ctrl"),
                    py::arg("controllable_dofs"),
                    py::arg("use_angular_errors") = true)

                .def("configure", &PDControl::configure)
                .def("calculate", &PDControl::calculate)

                .def("set_pd", (void (PDControl::*)(double, double)) & PDControl::set_pd)
                .def("set_pd", (void (PDControl::*)(const Eigen::VectorXd&, const Eigen::VectorXd&)) & PDControl::set_pd)

                .def("pd", &PDControl::pd)

                .def("using_angular_errors", &PDControl::using_angular_errors)
                .def("set_use_angular_errors", &PDControl::set_use_angular_errors,
                    py::arg("enable") = true)

                .def("clone", &PDControl::clone);

            // SimpleControl class
            py::class_<SimpleControl, RobotControl, std::shared_ptr<SimpleControl>>(m, "SimpleControl")
                .def(py::init<>())
                .def(py::init<const Eigen::VectorXd&, bool>(),
                    py::arg("ctrl"),
                    py::arg("full_control") = false)
                .def(py::init<const Eigen::VectorXd&, const std::vector<std::string>&>(),
                    py::arg("ctrl"),
                    py::arg("controllable_dofs"))

                .def("configure", &SimpleControl::configure)
                .def("calculate", &SimpleControl::calculate)

                .def("clone", &SimpleControl::clone);
        }
    } // namespace python
} // namespace robot_dart