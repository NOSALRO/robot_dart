#include "robot_dart.hpp"

#include <pybind11/eigen.h>
#include <pybind11/operators.h>
#include <pybind11/stl.h>

#include <robot_dart/robot_dart_simu.hpp>

namespace robot_dart {
    namespace python {
        void py_simu(py::module& m)
        {
            using namespace robot_dart;
            using Descriptor = descriptor::BaseDescriptor;

            // Descriptor class
            class PyDescriptor : public Descriptor {
            public:
                using Descriptor::BaseDescriptor;

                /* Trampolines */
                void operator()() override
                {
                    PYBIND11_OVERLOAD_PURE_NAME(
                        void, /* return type */
                        descriptor::BaseDescriptor, /* parent class */
                        "__call__", /* name in python */
                        operator(), /* name in C++ */
                        /* arguments */
                    );
                }
            };

            class PublicistDescriptor : public Descriptor {
            public:
                using Descriptor::_desc_period;
                using Descriptor::_simu;
            };

            py::class_<Descriptor, PyDescriptor, std::shared_ptr<Descriptor>>(m, "Descriptor")
                .def(py::init<RobotDARTSimu*, size_t>())

                .def_readwrite("_desc_period", &PublicistDescriptor::_desc_period)
                .def_readonly("_simu", &PublicistDescriptor::_simu)

                .def("desc_dump", &Descriptor::desc_dump)
                .def("set_desc_dump", &Descriptor::set_desc_dump)

                .def("__call__", &Descriptor::operator());

            // RobotDARTSimu class
            py::class_<RobotDARTSimu>(m, "RobotDARTSimu")
                .def(py::init<double>())

                .def("run", &RobotDARTSimu::run,
                    py::arg("max_duration") = 5.,
                    py::arg("reset_commands") = false)
                .def("step_world", &RobotDARTSimu::step_world,
                    py::arg("reset_commands") = false)
                .def("step_robots", &RobotDARTSimu::step_robots,
                    py::arg("reset_commands") = false)

                .def("scheduler", (Scheduler & (RobotDARTSimu::*)(void)) & RobotDARTSimu::scheduler, py::return_value_policy::reference)
                .def("schedule", &RobotDARTSimu::schedule)

                .def("physics_freq", &RobotDARTSimu::physics_freq)

                .def("control_freq", &RobotDARTSimu::control_freq)
                .def("set_control_freq", &RobotDARTSimu::set_control_freq)

                .def("graphics_freq", &RobotDARTSimu::graphics_freq)
                .def("set_graphics_freq", &RobotDARTSimu::set_graphics_freq)

                .def("graphics", &RobotDARTSimu::graphics)
                .def("set_graphics", &RobotDARTSimu::set_graphics, py::keep_alive<2, 1>())

                .def("world", &RobotDARTSimu::world)

                .def("add_descriptor", (void (RobotDARTSimu::*)(const std::shared_ptr<descriptor::BaseDescriptor>&)) & RobotDARTSimu::add_descriptor, py::keep_alive<2, 1>())
                .def("descriptors", &RobotDARTSimu::descriptors)
                .def("descriptor", &RobotDARTSimu::descriptor)

                .def("remove_descriptor", (void (RobotDARTSimu::*)(const std::shared_ptr<descriptor::BaseDescriptor>&)) & RobotDARTSimu::remove_descriptor)
                .def("remove_descriptor", (void (RobotDARTSimu::*)(size_t)) & RobotDARTSimu::remove_descriptor)
                .def("clear_descriptors", &RobotDARTSimu::clear_descriptors)

                .def("add_camera", &RobotDARTSimu::add_camera, py::keep_alive<2, 1>())
                .def("cameras", &RobotDARTSimu::cameras)
                .def("camera", &RobotDARTSimu::camera)

                .def("remove_camera", (void (RobotDARTSimu::*)(const std::shared_ptr<gui::Base>&)) & RobotDARTSimu::remove_camera)
                .def("remove_camera", (void (RobotDARTSimu::*)(size_t)) & RobotDARTSimu::remove_camera)
                .def("clear_cameras", &RobotDARTSimu::clear_cameras)

                .def("step", &RobotDARTSimu::step)
                .def("set_step", &RobotDARTSimu::set_step)

                .def("stop_sim", &RobotDARTSimu::stop_sim)
                .def("halted_sim", &RobotDARTSimu::halted_sim)

                .def("num_robots", &RobotDARTSimu::num_robots)
                .def("robots", &RobotDARTSimu::robots)
                .def("robot", &RobotDARTSimu::robot)
                .def("robot_index", &RobotDARTSimu::robot_index)

                .def("add_robot", &RobotDARTSimu::add_robot, py::keep_alive<2, 1>())
                .def("add_visual_robot", &RobotDARTSimu::add_visual_robot, py::keep_alive<2, 1>())
                .def("remove_robot", (void (RobotDARTSimu::*)(const std::shared_ptr<Robot>&)) & RobotDARTSimu::remove_robot)
                .def("remove_robot", (void (RobotDARTSimu::*)(size_t)) & RobotDARTSimu::remove_robot)
                .def("clear_robots", &RobotDARTSimu::clear_robots)

                .def("add_floor", &RobotDARTSimu::add_floor)
                .def("add_checkerboard_floor", &RobotDARTSimu::add_checkerboard_floor)

                .def("set_collision_detector", &RobotDARTSimu::set_collision_detector)
                .def("collision_detector", &RobotDARTSimu::collision_detector)

                .def("set_collision_mask", (void (RobotDARTSimu::*)(size_t, uint16_t)) & RobotDARTSimu::set_collision_mask)
                .def("set_collision_mask", (void (RobotDARTSimu::*)(size_t, const std::string&, uint16_t)) & RobotDARTSimu::set_collision_mask)
                .def("set_collision_mask", (void (RobotDARTSimu::*)(size_t, size_t, uint16_t)) & RobotDARTSimu::set_collision_mask)

                .def("collision_mask", (uint16_t(RobotDARTSimu::*)(size_t, const std::string&)) & RobotDARTSimu::collision_mask)
                .def("collision_mask", (uint16_t(RobotDARTSimu::*)(size_t, size_t)) & RobotDARTSimu::collision_mask)

                .def("remove_collision_mask", (void (RobotDARTSimu::*)(size_t)) & RobotDARTSimu::remove_collision_mask)
                .def("remove_collision_mask", (void (RobotDARTSimu::*)(size_t, const std::string&)) & RobotDARTSimu::remove_collision_mask)
                .def("remove_collision_mask", (void (RobotDARTSimu::*)(size_t, size_t)) & RobotDARTSimu::remove_collision_mask)

                .def("remove_all_collision_masks", &RobotDARTSimu::remove_all_collision_masks);
        }
    } // namespace python
} // namespace robot_dart