#include "robot_dart.hpp"

PYBIND11_MODULE(RobotDART, m)
{
    using namespace robot_dart::python;
    // Load dartpy
    py::module::import("dartpy");
    // Load magnum math
    // py::module::import("magnum.math");

    m.doc() = "RobotDART: Python API of robot_dart";

    py_simu(m);
    py_robot(m);
    py_control(m);

#ifdef GRAPHIC
    py_gui(m);
#endif
}