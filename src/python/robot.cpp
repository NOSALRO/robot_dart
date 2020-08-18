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
                .def(py::init<const std::string&, const std::vector<std::pair<std::string, std::string>>&, const std::string&, bool, bool>(),
                    py::arg("model_file"),
                    py::arg("packages"),
                    py::arg("robot_name") = "robot",
                    py::arg("is_urdf_string") = false,
                    py::arg("cast_shadows") = true)
                .def(py::init<const std::string&, const std::string&, bool, bool>(),
                    py::arg("model_file"),
                    py::arg("robot_name") = "robot",
                    py::arg("is_urdf_string") = false,
                    py::arg("cast_shadows") = true)
                .def(py::init<dart::dynamics::SkeletonPtr, const std::string&, bool>(),
                    py::arg("skeleton"),
                    py::arg("robot_name") = "robot",
                    py::arg("cast_shadows") = true)

                .def("clone", &Robot::clone)
                .def("clone_ghost", &Robot::clone_ghost,
                    py::arg("ghost_name") = "ghost",
                    py::arg("ghost_color") = Eigen::Vector4d{0.3, 0.3, 0.3, 0.7})
                .def("skeleton", &Robot::skeleton)

                .def("name", &Robot::name)
                .def("model_filename", &Robot::model_filename)
                .def("model_packages", &Robot::model_packages)

                .def("update", &Robot::update)

                .def("reinit_controllers", &Robot::reinit_controllers)
                .def("num_controllers", &Robot::num_controllers)
                .def("controllers", &Robot::controllers)
                .def("active_controllers", &Robot::active_controllers)
                .def("controller", &Robot::controller)
                .def("add_controller", &Robot::add_controller, py::keep_alive<2, 1>(),
                    py::arg("controller"),
                    py::arg("weight") = 1.)
                .def("remove_controller", (void (Robot::*)(const std::shared_ptr<control::RobotControl>&)) & Robot::remove_controller)
                .def("remove_controller", (void (Robot::*)(size_t)) & Robot::remove_controller)
                .def("clear_controllers", &Robot::clear_controllers)

                .def("fix_to_world", &Robot::fix_to_world)
                .def("free_from_world", &Robot::free_from_world,
                    py::arg("pose") = Eigen::Vector6d::Zero())
                .def("fixed", &Robot::fixed)
                .def("free", &Robot::free)

                .def("reset", &Robot::reset)

                .def("set_actuator_types", &Robot::set_actuator_types,
                    py::arg("type"),
                    py::arg("joint_names") = std::vector<std::string>(),
                    py::arg("override_mimic") = false,
                    py::arg("override_base") = false)

                .def("set_actuator_type", &Robot::set_actuator_type,
                    py::arg("type"),
                    py::arg("joint_name"),
                    py::arg("override_mimic") = false,
                    py::arg("override_base") = false)

                .def("set_mimic", &Robot::set_mimic,
                    py::arg("joint_name"),
                    py::arg("mimic_joint_name"),
                    py::arg("multiplier") = 1.,
                    py::arg("offset") = 0.)

                .def("actuator_type", &Robot::actuator_type)

                .def("actuator_types", &Robot::actuator_types,
                    py::arg("joint_names") = std::vector<std::string>())

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

                .def("positions", &Robot::positions,
                    py::arg("dof_names") = std::vector<std::string>())
                .def("set_positions", &Robot::set_positions,
                    py::arg("positions"),
                    py::arg("dof_names") = std::vector<std::string>())

                .def("position_lower_limits", &Robot::position_lower_limits,
                    py::arg("dof_names") = std::vector<std::string>())
                .def("set_position_lower_limits", &Robot::set_position_lower_limits,
                    py::arg("positions"),
                    py::arg("dof_names") = std::vector<std::string>())
                .def("position_upper_limits", &Robot::position_upper_limits,
                    py::arg("dof_names") = std::vector<std::string>())
                .def("set_position_upper_limits", &Robot::set_position_upper_limits,
                    py::arg("positions"),
                    py::arg("dof_names") = std::vector<std::string>())

                .def("velocities", &Robot::velocities,
                    py::arg("dof_names") = std::vector<std::string>())
                .def("set_velocities", &Robot::set_velocities,
                    py::arg("velocities"),
                    py::arg("dof_names") = std::vector<std::string>())

                .def("velocity_lower_limits", &Robot::velocity_lower_limits,
                    py::arg("dof_names") = std::vector<std::string>())
                .def("set_velocity_lower_limits", &Robot::set_velocity_lower_limits,
                    py::arg("velocities"),
                    py::arg("dof_names") = std::vector<std::string>())
                .def("velocity_upper_limits", &Robot::velocity_upper_limits,
                    py::arg("dof_names") = std::vector<std::string>())
                .def("set_velocity_upper_limits", &Robot::set_velocity_upper_limits,
                    py::arg("velocities"),
                    py::arg("dof_names") = std::vector<std::string>())

                .def("accelerations", &Robot::accelerations,
                    py::arg("dof_names") = std::vector<std::string>())
                .def("set_accelerations", &Robot::set_accelerations,
                    py::arg("accelerations"),
                    py::arg("dof_names") = std::vector<std::string>())

                .def("acceleration_lower_limits", &Robot::acceleration_lower_limits,
                    py::arg("dof_names") = std::vector<std::string>())
                .def("set_acceleration_lower_limits", &Robot::set_acceleration_lower_limits,
                    py::arg("accelerations"),
                    py::arg("dof_names") = std::vector<std::string>())
                .def("acceleration_upper_limits", &Robot::acceleration_upper_limits,
                    py::arg("dof_names") = std::vector<std::string>())
                .def("set_acceleration_upper_limits", &Robot::set_acceleration_upper_limits,
                    py::arg("accelerations"),
                    py::arg("dof_names") = std::vector<std::string>())

                .def("forces", &Robot::forces,
                    py::arg("dof_names") = std::vector<std::string>())
                .def("set_forces", &Robot::set_forces,
                    py::arg("forces"),
                    py::arg("dof_names") = std::vector<std::string>())

                .def("force_lower_limits", &Robot::force_lower_limits,
                    py::arg("dof_names") = std::vector<std::string>())
                .def("set_force_lower_limits", &Robot::set_force_lower_limits,
                    py::arg("forces"),
                    py::arg("dof_names") = std::vector<std::string>())
                .def("force_upper_limits", &Robot::force_upper_limits,
                    py::arg("dof_names") = std::vector<std::string>())
                .def("set_force_upper_limits", &Robot::set_force_upper_limits,
                    py::arg("forces"),
                    py::arg("dof_names") = std::vector<std::string>())

                .def("commands", &Robot::commands,
                    py::arg("dof_names") = std::vector<std::string>())
                .def("set_commands", &Robot::set_commands,
                    py::arg("commands"),
                    py::arg("dof_names") = std::vector<std::string>())

                .def("force_torque", &Robot::force_torque)

                .def("set_external_force", (void (Robot::*)(const std::string&, const Eigen::Vector3d&, const Eigen::Vector3d&, bool, bool)) & Robot::set_external_force,
                    py::arg("body_name"),
                    py::arg("force"),
                    py::arg("offset") = Eigen::Vector3d::Zero(),
                    py::arg("force_local") = false,
                    py::arg("offset_local") = true)
                .def("set_external_force", (void (Robot::*)(size_t, const Eigen::Vector3d&, const Eigen::Vector3d&, bool, bool)) & Robot::set_external_force,
                    py::arg("body_index"),
                    py::arg("force"),
                    py::arg("offset") = Eigen::Vector3d::Zero(),
                    py::arg("force_local") = false,
                    py::arg("offset_local") = true)
                .def("add_external_force", (void (Robot::*)(const std::string&, const Eigen::Vector3d&, const Eigen::Vector3d&, bool, bool)) & Robot::add_external_force,
                    py::arg("body_name"),
                    py::arg("force"),
                    py::arg("offset") = Eigen::Vector3d::Zero(),
                    py::arg("force_local") = false,
                    py::arg("offset_local") = true)
                .def("add_external_force", (void (Robot::*)(size_t, const Eigen::Vector3d&, const Eigen::Vector3d&, bool, bool)) & Robot::add_external_force,
                    py::arg("body_index"),
                    py::arg("force"),
                    py::arg("offset") = Eigen::Vector3d::Zero(),
                    py::arg("force_local") = false,
                    py::arg("offset_local") = true)

                .def("set_external_torque", (void (Robot::*)(const std::string&, const Eigen::Vector3d&, bool)) & Robot::set_external_torque,
                    py::arg("body_name"),
                    py::arg("torque"),
                    py::arg("local") = false)
                .def("set_external_torque", (void (Robot::*)(size_t, const Eigen::Vector3d&, bool)) & Robot::set_external_torque,
                    py::arg("body_index"),
                    py::arg("torque"),
                    py::arg("local") = false)
                .def("add_external_torque", (void (Robot::*)(const std::string&, const Eigen::Vector3d&, bool)) & Robot::add_external_torque,
                    py::arg("body_name"),
                    py::arg("torque"),
                    py::arg("local") = false)
                .def("add_external_torque", (void (Robot::*)(size_t, const Eigen::Vector3d&, bool)) & Robot::add_external_torque,
                    py::arg("body_index"),
                    py::arg("torque"),
                    py::arg("local") = false)

                .def("clear_external_forces", &Robot::clear_external_forces)

                .def("external_forces", (Eigen::Vector6d(Robot::*)(const std::string& body_name) const) & Robot::external_forces)
                .def("external_forces", (Eigen::Vector6d(Robot::*)(size_t body_index) const) & Robot::external_forces)

                .def("body_pose", (Eigen::Isometry3d(Robot::*)(const std::string& body_name) const) & Robot::body_pose)
                .def("body_pose", (Eigen::Isometry3d(Robot::*)(size_t body_index) const) & Robot::body_pose)

                .def("body_pose_vec", (Eigen::Vector6d(Robot::*)(const std::string& body_name) const) & Robot::body_pose_vec)
                .def("body_pose_vec", (Eigen::Vector6d(Robot::*)(size_t body_index) const) & Robot::body_pose_vec)

                .def("body_velocity", (Eigen::Vector6d(Robot::*)(const std::string& body_name) const) & Robot::body_velocity)
                .def("body_velocity", (Eigen::Vector6d(Robot::*)(size_t body_index) const) & Robot::body_velocity)

                .def("body_acceleration", (Eigen::Vector6d(Robot::*)(const std::string& body_name) const) & Robot::body_acceleration)
                .def("body_acceleration", (Eigen::Vector6d(Robot::*)(size_t body_index) const) & Robot::body_acceleration)

                .def("body_names", &Robot::body_names)
                .def("body_name", &Robot::body_name)
                .def("set_body_name", &Robot::set_body_name)

                .def("body_mass", (double (Robot::*)(const std::string& body_name) const) & Robot::body_mass)
                .def("body_mass", (double (Robot::*)(size_t body_index) const) & Robot::body_mass)

                .def("set_body_mass", (void (Robot::*)(const std::string& body_name, double mass)) & Robot::set_body_mass)
                .def("set_body_mass", (void (Robot::*)(size_t body_index, double mass)) & Robot::set_body_mass)
                .def("add_body_mass", (void (Robot::*)(const std::string& body_name, double mass)) & Robot::add_body_mass)
                .def("add_body_mass", (void (Robot::*)(size_t body_index, double mass)) & Robot::add_body_mass)

                .def("jacobian", &Robot::jacobian,
                    py::arg("body_name"),
                    py::arg("dof_names") = std::vector<std::string>())
                .def("jacobian_deriv", &Robot::jacobian_deriv,
                    py::arg("body_name"),
                    py::arg("dof_names") = std::vector<std::string>())

                .def("com_jacobian", &Robot::com_jacobian,
                    py::arg("dof_names") = std::vector<std::string>())
                .def("com_jacobian_deriv", &Robot::com_jacobian_deriv,
                    py::arg("dof_names") = std::vector<std::string>())

                .def("mass_matrix", &Robot::mass_matrix,
                    py::arg("dof_names") = std::vector<std::string>())
                .def("aug_mass_matrix", &Robot::aug_mass_matrix,
                    py::arg("dof_names") = std::vector<std::string>())
                .def("inv_mass_matrix", &Robot::inv_mass_matrix,
                    py::arg("dof_names") = std::vector<std::string>())
                .def("inv_aug_mass_matrix", &Robot::inv_aug_mass_matrix,
                    py::arg("dof_names") = std::vector<std::string>())

                .def("coriolis_forces", &Robot::coriolis_forces,
                    py::arg("dof_names") = std::vector<std::string>())
                .def("gravity_forces", &Robot::gravity_forces,
                    py::arg("dof_names") = std::vector<std::string>())
                .def("coriolis_gravity_forces", &Robot::coriolis_gravity_forces,
                    py::arg("dof_names") = std::vector<std::string>())

                .def("vec_dof", &Robot::vec_dof,
                    py::arg("vec"),
                    py::arg("dof_names"))

                .def("update_joint_dof_maps", &Robot::update_joint_dof_maps)
                .def("dof_map", &Robot::dof_map)
                .def("joint_map", &Robot::joint_map)

                .def("dof_names", &Robot::dof_names,
                    py::arg("filter_mimics") = false,
                    py::arg("filter_locked") = false,
                    py::arg("filter_passive") = false)
                .def("mimic_dof_names", &Robot::mimic_dof_names)
                .def("locked_dof_names", &Robot::locked_dof_names)
                .def("passive_dof_names", &Robot::passive_dof_names)
                .def("dof_name", &Robot::dof_name)
                .def("dof_index", &Robot::dof_index)

                .def("joint_names", &Robot::joint_names)
                .def("joint_name", &Robot::joint_name)
                .def("set_joint_name", &Robot::set_joint_name)
                .def("joint_index", &Robot::joint_index)

                .def("set_color_mode", (void (Robot::*)(const std::string&)) & Robot::set_color_mode)
                .def("set_color_mode", (void (Robot::*)(const std::string&, const std::string&)) & Robot::set_color_mode)

                .def("set_cast_shadows", &Robot::set_cast_shadows,
                    py::arg("cast_shadows") = true)
                .def("cast_shadows", &Robot::cast_shadows)

                .def("set_ghost", &Robot::set_ghost,
                    py::arg("ghost") = true)
                .def("ghost", &Robot::ghost)

                .def("set_draw_axis", &Robot::set_draw_axis,
                    py::arg("body_name"),
                    py::arg("size") = 0.25,
                    py::arg("draw") = true)

                .def("remove_all_drawing_axis", &Robot::remove_all_drawing_axis)

                // .def("drawing_axes", &Robot::drawing_axes)

                .def_static("create_box", &Robot::create_box,
                    py::arg("dims"),
                    py::arg("pose") = Eigen::Vector6d::Zero(),
                    py::arg("type") = "free",
                    py::arg("mass") = 1.,
                    py::arg("color") = dart::Color::Red(1.0),
                    py::arg("box_name") = "box")
                .def_static("create_ellipsoid", &Robot::create_ellipsoid,
                    py::arg("dims"),
                    py::arg("pose") = Eigen::Vector6d::Zero(),
                    py::arg("type") = "free",
                    py::arg("mass") = 1.,
                    py::arg("color") = dart::Color::Red(1.0),
                    py::arg("ellipsoid_name") = "ellipsoid");
        }
    } // namespace python
} // namespace robot_dart