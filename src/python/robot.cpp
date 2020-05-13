#include "robot_dart.hpp"

#include <pybind11/eigen.h>
#include <pybind11/stl.h>

#include <robot_dart/robot.hpp>

#include <robot_dart/control/robot_control.hpp>

namespace robot_dart {
    namespace python {
        void py_robot(py::module& m)
        {
            using namespace robot_dart;
            // Robot class
            py::class_<Robot, std::shared_ptr<Robot>>(m, "Robot")
                .def(py::init<const std::string&, const std::vector<std::pair<std::string, std::string>>&, const std::string&, bool>())
                .def(py::init<const std::string&, const std::string&, bool>())
                .def(py::init<dart::dynamics::SkeletonPtr, const std::string&>())

                .def("clone", &Robot::clone)
                .def("skeleton", &Robot::skeleton)

                .def("name", &Robot::name)

                .def("update", &Robot::update)

                .def("reinit_controllers", &Robot::reinit_controllers)
                .def("num_controllers", &Robot::num_controllers)
                .def("controllers", &Robot::controllers)
                .def("active_controllers", &Robot::active_controllers)
                .def("controller", &Robot::controller)
                .def("add_controller", &Robot::add_controller, py::keep_alive<2, 1>())
                .def("remove_controller", (void (Robot::*)(const std::shared_ptr<control::RobotControl>&)) & Robot::remove_controller)
                .def("remove_controller", (void (Robot::*)(size_t)) & Robot::remove_controller)
                .def("clear_controllers", &Robot::clear_controllers)

                .def("fix_to_world", &Robot::fix_to_world)
                .def("free_from_world", &Robot::free_from_world)
                .def("fixed", &Robot::fixed)
                .def("free", &Robot::free)

                .def("set_actuator_type", &Robot::set_actuator_type)
                .def("set_actuator_types", (void (Robot::*)(const std::vector<dart::dynamics::Joint::ActuatorType>&, bool)) & Robot::set_actuator_types)
                .def("set_actuator_types", (void (Robot::*)(dart::dynamics::Joint::ActuatorType, bool)) & Robot::set_actuator_types)

                .def("actuator_type", &Robot::actuator_type)
                .def("actuator_types", &Robot::actuator_types)

                .def("set_position_enforced", (void (Robot::*)(size_t, bool)) & Robot::set_position_enforced)
                .def("set_position_enforced", (void (Robot::*)(const std::vector<bool>&)) & Robot::set_position_enforced)
                .def("set_position_enforced", (void (Robot::*)(bool)) & Robot::set_position_enforced)

                .def("position_enforced", (bool (Robot::*)(size_t) const) & Robot::position_enforced)
                .def("position_enforced", (std::vector<bool>(Robot::*)() const) & Robot::position_enforced)

                .def("set_damping_coeff", &Robot::set_damping_coeff)
                .def("set_damping_coeffs", (void (Robot::*)(const std::vector<double>&)) & Robot::set_damping_coeffs)
                .def("set_damping_coeffs", (void (Robot::*)(double)) & Robot::set_damping_coeffs)

                .def("damping_coeff", &Robot::damping_coeff)
                .def("damping_coeffs", &Robot::damping_coeffs)

                .def("set_cfriction_coeff", &Robot::set_cfriction_coeff)
                .def("set_cfriction_coeffs", (void (Robot::*)(const std::vector<double>&)) & Robot::set_cfriction_coeffs)
                .def("set_cfriction_coeffs", (void (Robot::*)(double)) & Robot::set_cfriction_coeffs)

                .def("cfriction_coeff", &Robot::cfriction_coeff)
                .def("cfriction_coeffs", &Robot::cfriction_coeffs)

                .def("set_base_pose", &Robot::set_base_pose)

                .def("base_pose", &Robot::base_pose)
                .def("set_base_pose", &Robot::set_base_pose)

                .def("num_dofs", &Robot::num_dofs)
                .def("num_joints", &Robot::num_joints)
                .def("num_bodies", &Robot::num_bodies)

                .def("com", &Robot::com)
                .def("com_velocity", &Robot::com_velocity)
                .def("com_acceleration", &Robot::com_acceleration)

                .def("positions", &Robot::positions)
                .def("set_positions", &Robot::set_positions)

                .def("velocities", &Robot::velocities)
                .def("set_velocities", &Robot::set_velocities)

                .def("accelerations", &Robot::accelerations)
                .def("set_accelerations", &Robot::set_accelerations)

                .def("forces", &Robot::forces)
                .def("set_forces", &Robot::set_forces)

                .def("force_torque", &Robot::force_torque)

                .def("set_external_force", (void (Robot::*)(const std::string&, const Eigen::Vector3d&, const Eigen::Vector3d&, bool, bool)) & Robot::set_external_force)
                .def("set_external_force", (void (Robot::*)(size_t, const Eigen::Vector3d&, const Eigen::Vector3d&, bool, bool)) & Robot::set_external_force)
                .def("add_external_force", (void (Robot::*)(const std::string&, const Eigen::Vector3d&, const Eigen::Vector3d&, bool, bool)) & Robot::add_external_force)
                .def("add_external_force", (void (Robot::*)(size_t, const Eigen::Vector3d&, const Eigen::Vector3d&, bool, bool)) & Robot::add_external_force)

                .def("set_external_torque", (void (Robot::*)(const std::string&, const Eigen::Vector3d&, bool)) & Robot::set_external_torque)
                .def("set_external_torque", (void (Robot::*)(size_t, const Eigen::Vector3d&, bool)) & Robot::set_external_torque)
                .def("add_external_torque", (void (Robot::*)(const std::string&, const Eigen::Vector3d&, bool)) & Robot::add_external_torque)
                .def("add_external_torque", (void (Robot::*)(size_t, const Eigen::Vector3d&, bool)) & Robot::add_external_torque)

                .def("clear_external_forces", &Robot::clear_external_forces)

                .def("external_forces", (Eigen::Vector6d(Robot::*)(const std::string& body_name) const) & Robot::external_forces)
                .def("external_forces", (Eigen::Vector6d(Robot::*)(size_t body_index) const) & Robot::external_forces)

                .def("body_pose", (Eigen::Isometry3d(Robot::*)(const std::string& body_name) const) & Robot::body_pose)
                .def("body_pose", (Eigen::Isometry3d(Robot::*)(size_t body_index) const) & Robot::body_pose)

                .def("body_names", &Robot::body_names)
                .def("body_name", &Robot::body_name)
                .def("set_body_name", &Robot::set_body_name)

                .def("body_mass", (double (Robot::*)(const std::string& body_name) const) & Robot::body_mass)
                .def("body_mass", (double (Robot::*)(size_t body_index) const) & Robot::body_mass)

                .def("set_body_mass", (void (Robot::*)(const std::string& body_name, double mass)) & Robot::set_body_mass)
                .def("set_body_mass", (void (Robot::*)(size_t body_index, double mass)) & Robot::set_body_mass)
                .def("add_body_mass", (void (Robot::*)(const std::string& body_name, double mass)) & Robot::add_body_mass)
                .def("add_body_mass", (void (Robot::*)(size_t body_index, double mass)) & Robot::add_body_mass)

                .def("dof_names", &Robot::dof_names)
                .def("dof_name", &Robot::dof_name)
                .def("joint_names", &Robot::joint_names)
                .def("joint_name", &Robot::joint_name)
                .def("set_joint_name", &Robot::set_joint_name)

                .def("set_color_mode", (void (Robot::*)(dart::dynamics::MeshShape::ColorMode)) & Robot::set_color_mode)
                .def("set_color_mode", (void (Robot::*)(dart::dynamics::MeshShape::ColorMode, const std::string&)) & Robot::set_color_mode)

                .def_static("create_box", &Robot::create_box)
                .def_static("create_ellipsoid", &Robot::create_ellipsoid);
        }
    } // namespace python
} // namespace robot_dart