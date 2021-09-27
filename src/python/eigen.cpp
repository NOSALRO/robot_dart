#include "robot_dart.hpp"

#include <Eigen/Dense>
#include <Eigen/Geometry>

#include <pybind11/eigen.h>

#include <dart/math/Geometry.hpp>

namespace robot_dart {
    namespace python {
        void py_eigen(py::module& module)
        {
            // The following is copied and adapted from DART: https://github.com/dartsim/dart/blob/c9186b042c406835984e76ae3b9bdd8579cd0312/python/dartpy/eigen_geometry_pybind.cpp#L131
            using T = double;

            // Do not return references to matrices (e.g. `Eigen::Ref<>`) so that we have
            // tighter control over validation.

            auto m = module.def_submodule("math");

            // Affine2d
            {
                using Class = Eigen::Transform<T, 2, Eigen::Affine>;
                ::pybind11::class_<Class> py_class(m, "Affine2");
                py_class
                    .def(::pybind11::init([]() {
                        return Class::Identity();
                    }))
                    .def_static("Identity", []() {
                        return Class::Identity();
                    })
                    .def(::pybind11::init([](const Eigen::Matrix<T, 3, 3>& matrix) {
                        Class out(matrix);
                        return out;
                    }),
                        ::pybind11::arg("matrix"))
                    .def(::pybind11::init([](const Eigen::Matrix<T, 2, 2>& rotation, const Eigen::Matrix<T, 2, 1>& translation) {
                        Class out = Class::Identity();
                        out.linear() = rotation;
                        out.translation() = translation;
                        return out;
                    }),
                        ::pybind11::arg("rotation"), ::pybind11::arg("translation"))
                    .def(::pybind11::init([](const Class& other) {
                        return other;
                    }),
                        ::pybind11::arg("other"))
                    .def("matrix", [](const Class* self) -> Eigen::Matrix<T, 3, 3> {
                        return self->matrix();
                    })
                    .def("set_matrix", [](Class* self, const Eigen::Matrix<T, 3, 3>& matrix) {
                        Class update(matrix);
                        *self = update;
                    })
                    .def("translation", [](const Class* self) -> Eigen::Matrix<T, 2, 1> {
                        return self->translation();
                    })
                    .def("set_translation", [](Class* self, const Eigen::Matrix<T, 2, 1>& translation) {
                        self->translation() = translation;
                    })
                    .def("rotation", [](const Class* self) -> Eigen::Matrix<T, 2, 2> {
                        return self->linear();
                    })
                    .def("set_rotation", [](Class* self, const Eigen::Matrix<T, 2, 2>& rotation) {
                        self->linear() = rotation;
                    })
                    .def("__str__", [](::pybind11::object self) {
                        return ::pybind11::str(self.attr("matrix")());
                    })
                    // Do not define operator `__mul__` until we have the Python3 `@`
                    // operator so that operations are similar to those of arrays.
                    .def(
                        "multiply", [](const Class& self, const Class& other) {
                            return self * other;
                        },
                        ::pybind11::arg("other"))
                    .def(
                        "multiply", [](const Class& self, const Eigen::Matrix<T, 2, 1>& position) {
                            return self * position;
                        },
                        ::pybind11::arg("position"))
                    .def("inverse", [](const Class* self) {
                        return self->inverse();
                    })
                    //============================
                    // Begin: added by robot_dart
                    //============================
                    .def(
                        "translate", [](Class* self, const Eigen::Matrix<T, 2, 1>& other) {
                            self->translate(other);
                        },
                        ::pybind11::arg("other"))
                    .def(
                        "pretranslate", [](Class* self, const Eigen::Matrix<T, 2, 1>& other) {
                            self->pretranslate(other);
                        },
                        ::pybind11::arg("other"))
                    .def(
                        "rotate", [](Class* self, const Eigen::Matrix<T, 2, 2>& other) {
                            self->rotate(other);
                        },
                        ::pybind11::arg("other"))
                    .def(
                        "prerotate", [](Class* self, const Eigen::Matrix<T, 2, 2>& other) {
                            self->prerotate(other);
                        },
                        ::pybind11::arg("other"))
                    .def(
                        "scale", [](Class* self, const Eigen::Matrix<T, 2, 1>& other) {
                            self->scale(other);
                        },
                        ::pybind11::arg("other"))
                    .def(
                        "prescale", [](Class* self, const Eigen::Matrix<T, 2, 1>& other) {
                            self->prescale(other);
                        },
                        ::pybind11::arg("other"))
                    .def(
                        "shear", [](Class* self, const T& sx, const T& sy) {
                            self->shear(sx, sy);
                        },
                        ::pybind11::arg("sx"), ::pybind11::arg("sy"))
                    // .def( // For some reason this does not compile
                    //     "preshear", [](Class* self, const T& sx, const T& sy) {
                    //         self->preshear(sx, sy);
                    //     },
                    //     ::pybind11::arg("sx"), ::pybind11::arg("sy"))
                    //==========================
                    // End: added by robot_dart
                    //==========================
                    ;
                ::pybind11::implicitly_convertible<Eigen::Matrix<T, 3, 3>, Class>();
            }

            // left-overs from DART math
            {
                m.def(
                    "logMap",
                    +[](const Eigen::Isometry3d& _tf) -> Eigen::Vector6d {
                        return dart::math::logMap(_tf);
                    },
                    ::py::arg("tf"));

                m.def(
                    "logMap",
                    +[](const Eigen::Matrix3d& _rot) -> Eigen::Vector3d {
                        return dart::math::logMap(_rot);
                    },
                    ::py::arg("rot"));
            }
        }
    } // namespace python
} // namespace robot_dart