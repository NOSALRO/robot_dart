#include "robot_dart.hpp"

#include <robot_dart/scheduler.hpp>

namespace robot_dart {
    namespace python {
        void py_utils(py::module& m)
        {
            using namespace robot_dart;

            py::class_<Scheduler>(m, "Scheduler")
                .def(py::init<double, bool>(),
                    py::arg("dt"),
                    py::arg("sync") = false)

                .def("__call__", &Scheduler::operator(),
                    py::arg("frequency"))
                .def("schedule", &Scheduler::schedule,
                    py::arg("frequency"))

                .def("step", &Scheduler::step)

                .def("reset", &Scheduler::reset,
                    py::arg("dt"),
                    py::arg("sync") = false,
                    py::arg("current_time") = 0.,
                    py::arg("real_time") = 0.)

                .def("set_sync", &Scheduler::set_sync)
                .def("sync", &Scheduler::sync)

                .def("current_time", &Scheduler::current_time)
                .def("next_time", &Scheduler::next_time)
                .def("real_time", &Scheduler::real_time)
                .def("real_time_factor", &Scheduler::real_time_factor)
                .def("it_duration", &Scheduler::it_duration)
                .def("dt", &Scheduler::dt);
        }
    } // namespace python
} // namespace robot_dart