#include "robot_dart.hpp"

#include <robot_dart/scheduler.hpp>

namespace robot_dart {
    namespace python {
        void py_utils(py::module& m)
        {
            using namespace robot_dart;

            py::class_<Scheduler>(m, "Scheduler")
                .def(py::init<double, bool>())

                .def("__call__", &Scheduler::operator())

                .def("sync", &Scheduler::sync)
                .def("reset", &Scheduler::reset)

                .def("time", &Scheduler::time)
                .def("next_time", &Scheduler::next_time)
                .def("dt", &Scheduler::dt);
        }
    } // namespace python
} // namespace robot_dart