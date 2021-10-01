#include <pybind11/pybind11.h>

#include <robot_dart/utils.hpp>

namespace py = pybind11;

namespace robot_dart {
    namespace python {
        void py_robot(py::module& m);
        void py_simu(py::module& m);
        void py_control(py::module& m);
        void py_utils(py::module& m);
        void py_sensors(py::module& m);
        void py_eigen(py::module& m);

#ifdef GRAPHIC
        void py_gui(py::module& m);
#endif
    } // namespace python
} // namespace robot_dart