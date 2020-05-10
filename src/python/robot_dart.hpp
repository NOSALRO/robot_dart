#include <pybind11/pybind11.h>

namespace py = pybind11;

namespace robot_dart {
    namespace python {
        void py_robot(py::module& m);
        void py_simu(py::module& m);
        void py_control(py::module& m);

#ifdef GRAPHIC
        void py_gui(py::module& m);
#endif
    } // namespace python
} // namespace robot_dart