#include "robot_dart.hpp"
#include "utils_headers_dart.hpp"
#include "utils_headers_pybind11.hpp"

#include <robot_dart/robot.hpp>
#include <robot_dart/robot_dart_simu.hpp>

// all robots
#include <robot_dart/robots/a1.hpp>
#include <robot_dart/robots/arm.hpp>
#include <robot_dart/robots/franka.hpp>
#include <robot_dart/robots/hexapod.hpp>
#include <robot_dart/robots/icub.hpp>
#include <robot_dart/robots/iiwa.hpp>
#include <robot_dart/robots/pendulum.hpp>
#include <robot_dart/robots/talos.hpp>
#include <robot_dart/robots/tiago.hpp>
#include <robot_dart/robots/ur3e.hpp>
#include <robot_dart/robots/vx300.hpp>

#include <robot_dart/control/robot_control.hpp>

namespace robot_dart {
    namespace python {
        void py_robot(py::module& m)
        {
            using namespace robot_dart;

            // PyRobot
            class PyRobot : public Robot {
            public:
                using Robot::Robot;

                /* Trampolines */
                void _post_addition(RobotDARTSimu* simu) override
                {
                    PYBIND11_OVERLOAD(
                        void, /* Return type */
                        Robot, /* Parent class */
                        _post_addition, /* Name of function in C++ (must match Python name) */
                        simu);
                }

                void _post_removal(RobotDARTSimu* simu) override
                {
                    PYBIND11_OVERLOAD(
                        void, /* Return type */
                        Robot, /* Parent class */
                        _post_removal, /* Name of function in C++ (must match Python name) */
                        simu);
                }
            };

            // Robot class
            py::class_<Robot, PyRobot, std::shared_ptr<Robot>>(m, "Robot")
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

                .def("body_node", static_cast<dart::dynamics::BodyNode* (Robot::*)(const std::string&)>(&Robot::body_node), py::return_value_policy::reference,
                    py::arg("body_name"))

                .def("body_node", static_cast<dart::dynamics::BodyNode* (Robot::*)(size_t)>(&Robot::body_node), py::return_value_policy::reference,
                    py::arg("body_index"))

                .def("joint", static_cast<dart::dynamics::Joint* (Robot::*)(const std::string&)>(&Robot::joint), py::return_value_policy::reference,
                    py::arg("joint_name"))

                .def("joint", static_cast<dart::dynamics::Joint* (Robot::*)(size_t)>(&Robot::joint), py::return_value_policy::reference,
                    py::arg("joint_index"))

                .def("dof", static_cast<dart::dynamics::DegreeOfFreedom* (Robot::*)(const std::string&)>(&Robot::dof), py::return_value_policy::reference,
                    py::arg("dof_name"))

                .def("dof", static_cast<dart::dynamics::DegreeOfFreedom* (Robot::*)(size_t)>(&Robot::dof), py::return_value_policy::reference,
                    py::arg("dof_index"))

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
                .def("remove_controller", static_cast<void (Robot::*)(const std::shared_ptr<control::RobotControl>&)>(&Robot::remove_controller))
                .def("remove_controller", static_cast<void (Robot::*)(size_t)>(&Robot::remove_controller))
                .def("clear_controllers", &Robot::clear_controllers)

                .def("fix_to_world", &Robot::fix_to_world)
                .def("free_from_world", &Robot::free_from_world,
                    py::arg("pose") = Eigen::Vector6d::Zero())
                .def("fixed", &Robot::fixed)
                .def("free", &Robot::free)

                .def("reset", &Robot::reset)
                .def("clear_external_forces", &Robot::clear_external_forces)
                .def("clear_internal_forces", &Robot::clear_internal_forces)
                .def("reset_commands", &Robot::reset_commands)

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

                .def("set_position_enforced", static_cast<void (Robot::*)(const std::vector<bool>&, const std::vector<std::string>&)>(&Robot::set_position_enforced),
                    py::arg("enforced"),
                    py::arg("dof_names") = std::vector<std::string>())
                .def("set_position_enforced", static_cast<void (Robot::*)(bool, const std::vector<std::string>&)>(&Robot::set_position_enforced),
                    py::arg("enforced"),
                    py::arg("dof_names") = std::vector<std::string>())

                .def("force_position_bounds", &Robot::force_position_bounds)

                .def("position_enforced", &Robot::position_enforced,
                    py::arg("dof_names") = std::vector<std::string>())

                .def("set_damping_coeffs", static_cast<void (Robot::*)(const std::vector<double>&, const std::vector<std::string>&)>(&Robot::set_damping_coeffs),
                    py::arg("damps"),
                    py::arg("dof_names") = std::vector<std::string>())
                .def("set_damping_coeffs", static_cast<void (Robot::*)(double, const std::vector<std::string>&)>(&Robot::set_damping_coeffs),
                    py::arg("damps"),
                    py::arg("dof_names") = std::vector<std::string>())

                .def("damping_coeffs", &Robot::damping_coeffs,
                    py::arg("dof_names") = std::vector<std::string>())

                .def("set_coulomb_coeffs", static_cast<void (Robot::*)(const std::vector<double>&, const std::vector<std::string>&)>(&Robot::set_coulomb_coeffs),
                    py::arg("cfrictions"),
                    py::arg("dof_names") = std::vector<std::string>())
                .def("set_coulomb_coeffs", static_cast<void (Robot::*)(double, const std::vector<std::string>&)>(&Robot::set_coulomb_coeffs),
                    py::arg("cfrictions"),
                    py::arg("dof_names") = std::vector<std::string>())

                .def("coulomb_coeffs", &Robot::coulomb_coeffs,
                    py::arg("dof_names") = std::vector<std::string>())

                .def("set_spring_stiffnesses", static_cast<void (Robot::*)(const std::vector<double>&, const std::vector<std::string>&)>(&Robot::set_spring_stiffnesses),
                    py::arg("stiffnesses"),
                    py::arg("dof_names") = std::vector<std::string>())
                .def("set_spring_stiffnesses", static_cast<void (Robot::*)(double, const std::vector<std::string>&)>(&Robot::set_spring_stiffnesses),
                    py::arg("stiffnesses"),
                    py::arg("dof_names") = std::vector<std::string>())

                .def("spring_stiffnesses", &Robot::spring_stiffnesses,
                    py::arg("dof_names") = std::vector<std::string>())

                .def("set_friction_dir", static_cast<void (Robot::*)(const std::string&, const Eigen::Vector3d&)>(&Robot::set_friction_dir),
                    py::arg("body_name"),
                    py::arg("direction"))
                .def("set_friction_dir", static_cast<void (Robot::*)(size_t, const Eigen::Vector3d&)>(&Robot::set_friction_dir),
                    py::arg("body_index"),
                    py::arg("direction"))

                .def("friction_dir", static_cast<Eigen::Vector3d (Robot::*)(const std::string&)>(&Robot::friction_dir),
                    py::arg("body_name"))
                .def("friction_dir", static_cast<Eigen::Vector3d (Robot::*)(size_t)>(&Robot::friction_dir),
                    py::arg("body_index"))

                .def("set_friction_coeff", static_cast<void (Robot::*)(const std::string&, double)>(&Robot::set_friction_coeff),
                    py::arg("body_name"),
                    py::arg("value"))
                .def("set_friction_coeff", static_cast<void (Robot::*)(size_t, double)>(&Robot::set_friction_coeff),
                    py::arg("body_index"),
                    py::arg("value"))
                .def("set_friction_coeffs", &Robot::set_friction_coeffs,
                    py::arg("value"))

                .def("friction_coeff", static_cast<double (Robot::*)(const std::string&)>(&Robot::friction_coeff),
                    py::arg("body_name"))
                .def("friction_coeff", static_cast<double (Robot::*)(size_t)>(&Robot::friction_coeff),
                    py::arg("body_index"))

                .def("set_secondary_friction_coeff", static_cast<void (Robot::*)(const std::string&, double)>(&Robot::set_secondary_friction_coeff),
                    py::arg("body_name"),
                    py::arg("value"))
                .def("set_secondary_friction_coeff", static_cast<void (Robot::*)(size_t, double)>(&Robot::set_secondary_friction_coeff),
                    py::arg("body_index"),
                    py::arg("value"))
                .def("set_secondary_friction_coeffs", &Robot::set_secondary_friction_coeffs,
                    py::arg("value"))

                .def("secondary_friction_coeff", static_cast<double (Robot::*)(const std::string&)>(&Robot::secondary_friction_coeff),
                    py::arg("body_name"))
                .def("secondary_friction_coeff", static_cast<double (Robot::*)(size_t)>(&Robot::secondary_friction_coeff),
                    py::arg("body_index"))

                .def("set_restitution_coeff", static_cast<void (Robot::*)(const std::string&, double)>(&Robot::set_restitution_coeff),
                    py::arg("body_name"),
                    py::arg("value"))
                .def("set_restitution_coeff", static_cast<void (Robot::*)(size_t, double)>(&Robot::set_restitution_coeff),
                    py::arg("body_index"),
                    py::arg("value"))
                .def("set_restitution_coeffs", &Robot::set_restitution_coeffs,
                    py::arg("value"))

                .def("restitution_coeff", static_cast<double (Robot::*)(const std::string&)>(&Robot::restitution_coeff),
                    py::arg("body_name"))
                .def("restitution_coeff", static_cast<double (Robot::*)(size_t)>(&Robot::restitution_coeff),
                    py::arg("body_index"))

                .def("base_pose", &Robot::base_pose)
                .def("base_pose_vec", &Robot::base_pose_vec)

                .def("set_base_pose", static_cast<void (Robot::*)(const Eigen::Isometry3d&)>(&Robot::set_base_pose),
                    py::arg("tf"))
                .def("set_base_pose", static_cast<void (Robot::*)(const Eigen::Vector6d&)>(&Robot::set_base_pose),
                    py::arg("pose"))

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

                .def("set_external_force", static_cast<void (Robot::*)(const std::string&, const Eigen::Vector3d&, const Eigen::Vector3d&, bool, bool)>(&Robot::set_external_force),
                    py::arg("body_name"),
                    py::arg("force"),
                    py::arg("offset") = Eigen::Vector3d::Zero(),
                    py::arg("force_local") = false,
                    py::arg("offset_local") = true)
                .def("set_external_force", static_cast<void (Robot::*)(size_t, const Eigen::Vector3d&, const Eigen::Vector3d&, bool, bool)>(&Robot::set_external_force),
                    py::arg("body_index"),
                    py::arg("force"),
                    py::arg("offset") = Eigen::Vector3d::Zero(),
                    py::arg("force_local") = false,
                    py::arg("offset_local") = true)
                .def("add_external_force", static_cast<void (Robot::*)(const std::string&, const Eigen::Vector3d&, const Eigen::Vector3d&, bool, bool)>(&Robot::add_external_force),
                    py::arg("body_name"),
                    py::arg("force"),
                    py::arg("offset") = Eigen::Vector3d::Zero(),
                    py::arg("force_local") = false,
                    py::arg("offset_local") = true)
                .def("add_external_force", static_cast<void (Robot::*)(size_t, const Eigen::Vector3d&, const Eigen::Vector3d&, bool, bool)>(&Robot::add_external_force),
                    py::arg("body_index"),
                    py::arg("force"),
                    py::arg("offset") = Eigen::Vector3d::Zero(),
                    py::arg("force_local") = false,
                    py::arg("offset_local") = true)

                .def("set_external_torque", static_cast<void (Robot::*)(const std::string&, const Eigen::Vector3d&, bool)>(&Robot::set_external_torque),
                    py::arg("body_name"),
                    py::arg("torque"),
                    py::arg("local") = false)
                .def("set_external_torque", static_cast<void (Robot::*)(size_t, const Eigen::Vector3d&, bool)>(&Robot::set_external_torque),
                    py::arg("body_index"),
                    py::arg("torque"),
                    py::arg("local") = false)
                .def("add_external_torque", static_cast<void (Robot::*)(const std::string&, const Eigen::Vector3d&, bool)>(&Robot::add_external_torque),
                    py::arg("body_name"),
                    py::arg("torque"),
                    py::arg("local") = false)
                .def("add_external_torque", static_cast<void (Robot::*)(size_t, const Eigen::Vector3d&, bool)>(&Robot::add_external_torque),
                    py::arg("body_index"),
                    py::arg("torque"),
                    py::arg("local") = false)

                .def("external_forces", static_cast<Eigen::Vector6d (Robot::*)(const std::string& body_name) const>(&Robot::external_forces))
                .def("external_forces", static_cast<Eigen::Vector6d (Robot::*)(size_t body_index) const>(&Robot::external_forces))

                .def("body_pose", static_cast<Eigen::Isometry3d (Robot::*)(const std::string& body_name) const>(&Robot::body_pose))
                .def("body_pose", static_cast<Eigen::Isometry3d (Robot::*)(size_t body_index) const>(&Robot::body_pose))

                .def("body_pose_vec", static_cast<Eigen::Vector6d (Robot::*)(const std::string& body_name) const>(&Robot::body_pose_vec))
                .def("body_pose_vec", static_cast<Eigen::Vector6d (Robot::*)(size_t body_index) const>(&Robot::body_pose_vec))

                .def("body_velocity", static_cast<Eigen::Vector6d (Robot::*)(const std::string& body_name) const>(&Robot::body_velocity))
                .def("body_velocity", static_cast<Eigen::Vector6d (Robot::*)(size_t body_index) const>(&Robot::body_velocity))

                .def("body_acceleration", static_cast<Eigen::Vector6d (Robot::*)(const std::string& body_name) const>(&Robot::body_acceleration))
                .def("body_acceleration", static_cast<Eigen::Vector6d (Robot::*)(size_t body_index) const>(&Robot::body_acceleration))

                .def("body_names", &Robot::body_names)
                .def("body_name", &Robot::body_name)
                .def("set_body_name", &Robot::set_body_name)

                .def("body_mass", static_cast<double (Robot::*)(const std::string& body_name) const>(&Robot::body_mass))
                .def("body_mass", static_cast<double (Robot::*)(size_t body_index) const>(&Robot::body_mass))

                .def("set_body_mass", static_cast<void (Robot::*)(const std::string& body_name, double mass)>(&Robot::set_body_mass))
                .def("set_body_mass", static_cast<void (Robot::*)(size_t body_index, double mass)>(&Robot::set_body_mass))
                .def("add_body_mass", static_cast<void (Robot::*)(const std::string& body_name, double mass)>(&Robot::add_body_mass))
                .def("add_body_mass", static_cast<void (Robot::*)(size_t body_index, double mass)>(&Robot::add_body_mass))

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

                .def("set_color_mode", static_cast<void (Robot::*)(const std::string&)>(&Robot::set_color_mode))
                .def("set_color_mode", static_cast<void (Robot::*)(const std::string&, const std::string&)>(&Robot::set_color_mode))

                .def("set_self_collision", &Robot::set_self_collision,
                    py::arg("enable_self_collisions") = true,
                    py::arg("enable_adjacent_collisions") = false)
                .def("self_colliding", &Robot::self_colliding)
                .def("adjacent_colliding", &Robot::adjacent_colliding)

                .def("set_cast_shadows", &Robot::set_cast_shadows,
                    py::arg("cast_shadows") = true)
                .def("cast_shadows", &Robot::cast_shadows)

                .def("set_ghost", &Robot::set_ghost,
                    py::arg("ghost") = true)
                .def("ghost", &Robot::ghost)

                .def("set_draw_axis", &Robot::set_draw_axis,
                    py::arg("body_name"),
                    py::arg("size") = 0.25)

                .def("remove_all_drawing_axis", &Robot::remove_all_drawing_axis)

                // .def("drawing_axes", &Robot::drawing_axes)

                .def_static("create_box", static_cast<std::shared_ptr<Robot> (*)(const Eigen::Vector3d&, const Eigen::Vector6d&, const std::string&, double, const Eigen::Vector4d&, const std::string&)>(&Robot::create_box),
                    py::arg("dims"),
                    py::arg("pose") = Eigen::Vector6d::Zero(),
                    py::arg("type") = "free",
                    py::arg("mass") = 1.,
                    py::arg("color") = dart::Color::Red(1.0),
                    py::arg("box_name") = "box")
                .def_static("create_box", static_cast<std::shared_ptr<Robot> (*)(const Eigen::Vector3d&, const Eigen::Isometry3d&, const std::string&, double, const Eigen::Vector4d&, const std::string&)>(&Robot::create_box),
                    py::arg("dims"),
                    py::arg("tf"),
                    py::arg("type") = "free",
                    py::arg("mass") = 1.,
                    py::arg("color") = dart::Color::Red(1.0),
                    py::arg("box_name") = "box")

                .def_static("create_ellipsoid", static_cast<std::shared_ptr<Robot> (*)(const Eigen::Vector3d&, const Eigen::Vector6d&, const std::string&, double, const Eigen::Vector4d&, const std::string&)>(&Robot::create_ellipsoid),
                    py::arg("dims"),
                    py::arg("pose") = Eigen::Vector6d::Zero(),
                    py::arg("type") = "free",
                    py::arg("mass") = 1.,
                    py::arg("color") = dart::Color::Red(1.0),
                    py::arg("ellipsoid_name") = "ellipsoid")
                .def_static("create_ellipsoid", static_cast<std::shared_ptr<Robot> (*)(const Eigen::Vector3d&, const Eigen::Isometry3d&, const std::string&, double, const Eigen::Vector4d&, const std::string&)>(&Robot::create_ellipsoid),
                    py::arg("dims"),
                    py::arg("tf"),
                    py::arg("type") = "free",
                    py::arg("mass") = 1.,
                    py::arg("color") = dart::Color::Red(1.0),
                    py::arg("ellipsoid_name") = "ellipsoid");

            // Robot classes
            using namespace robot_dart::robots;
            py::class_<A1, Robot, std::shared_ptr<A1>>(m, "A1")
                .def(py::init<size_t, const std::string&, const std::vector<std::pair<std::string, std::string>>&>(),
                    py::arg("frequency") = 1000,
                    py::arg("urdf") = "unitree_a1/a1.urdf",
                    py::arg("packages") = std::vector<std::pair<std::string, std::string>>({{"a1_description", "unitree_a1/a1_description"}}))

                .def("imu", &A1::imu, py::return_value_policy::reference);

            py::class_<Arm, Robot, std::shared_ptr<Arm>>(m, "Arm")
                .def(py::init<const std::string&>(),
                    py::arg("urdf") = "arm.urdf");

            py::class_<Franka, Robot, std::shared_ptr<Franka>>(m, "Franka")
                .def(py::init<size_t, const std::string&, const std::vector<std::pair<std::string, std::string>>&>(),
                    py::arg("frequency") = 1000,
                    py::arg("urdf") = "franka/franka.urdf",
                    py::arg("packages") = std::vector<std::pair<std::string, std::string>>({{"franka_description", "franka/franka_description"}}))
                .def("ft_wrist", &Franka::ft_wrist, py::return_value_policy::reference);

            py::class_<Hexapod, Robot, std::shared_ptr<Hexapod>>(m, "Hexapod")
                .def(py::init<const std::string&>(),
                    py::arg("urdf") = "pexod.urdf");

            py::class_<Iiwa, Robot, std::shared_ptr<Iiwa>>(m, "Iiwa")
                .def(py::init<size_t, const std::string&, const std::vector<std::pair<std::string, std::string>>&>(),
                    py::arg("frequency") = 1000,
                    py::arg("urdf") = "iiwa/iiwa.urdf",
                    py::arg("packages") = std::vector<std::pair<std::string, std::string>>({{"iiwa_description", "iiwa/iiwa_description"}}))
                .def("ft_wrist", &Iiwa::ft_wrist, py::return_value_policy::reference);

            py::class_<Tiago, Robot, std::shared_ptr<Tiago>>(m, "Tiago")
                .def(py::init<size_t, const std::string&, const std::vector<std::pair<std::string, std::string>>&>(),
                    py::arg("frequency") = 1000,
                    py::arg("urdf") = "tiago/tiago_steel.urdf",
                    py::arg("packages") = std::vector<std::pair<std::string, std::string>>({{"tiago_description", "tiago/tiago_description"}}))
                .def("ft_wrist", &Tiago::ft_wrist, py::return_value_policy::reference)
                .def("caster_joints", &Tiago::caster_joints)
                .def("set_actuator_types", &Tiago::set_actuator_types,
                    py::arg("type"),
                    py::arg("joint_names") = std::vector<std::string>(),
                    py::arg("override_mimic") = false,
                    py::arg("override_base") = false,
                    py::arg("override_caster") = false)
                .def("set_actuator_type", &Tiago::set_actuator_type,
                    py::arg("type"),
                    py::arg("joint_name"),
                    py::arg("override_mimic") = false,
                    py::arg("override_base") = false,
                    py::arg("override_caster") = false);

            py::class_<ICub, Robot, std::shared_ptr<ICub>>(m, "ICub")
                .def(py::init<size_t, const std::string&, const std::vector<std::pair<std::string, std::string>>&>(),
                    py::arg("frequency") = 1000,
                    py::arg("urdf") = "icub/icub.urdf",
                    py::arg("packages") = std::vector<std::pair<std::string, std::string>>({{"icub_description", "icub/icub_description"}}))

                .def("imu", &ICub::imu, py::return_value_policy::reference)
                .def("ft_foot_left", &ICub::ft_foot_left, py::return_value_policy::reference)
                .def("ft_foot_right", &ICub::ft_foot_right, py::return_value_policy::reference);

            py::class_<Talos, Robot, std::shared_ptr<Talos>>(m, "Talos")
                .def(py::init<size_t, const std::string&, const std::vector<std::pair<std::string, std::string>>&>(),
                    py::arg("frequency") = 1000,
                    py::arg("urdf") = "talos/talos.urdf",
                    py::arg("packages") = std::vector<std::pair<std::string, std::string>>({{"talos_description", "talos/talos_description"}}))

                .def("imu", &Talos::imu, py::return_value_policy::reference)
                .def("ft_foot_left", &Talos::ft_foot_left, py::return_value_policy::reference)
                .def("ft_foot_right", &Talos::ft_foot_right, py::return_value_policy::reference)
                .def("ft_wrist_left", &Talos::ft_foot_left, py::return_value_policy::reference)
                .def("ft_wrist_right", &Talos::ft_foot_right, py::return_value_policy::reference)

                .def("torques", &Talos::torques);
            py::class_<TalosFastCollision, Talos, std::shared_ptr<TalosFastCollision>>(m, "TalosFastCollision")
                .def(py::init<size_t, const std::string&, const std::vector<std::pair<std::string, std::string>>&>(),
                    py::arg("frequency") = 1000,
                    py::arg("urdf") = "talos/talos_fast_collision.urdf",
                    py::arg("packages") = std::vector<std::pair<std::string, std::string>>({{"talos_description", "talos/talos_description"}}))

                .def("imu", &TalosFastCollision::imu, py::return_value_policy::reference)
                .def("ft_foot_left", &TalosFastCollision::ft_foot_left, py::return_value_policy::reference)
                .def("ft_foot_right", &TalosFastCollision::ft_foot_right, py::return_value_policy::reference)
                .def("ft_wrist_left", &TalosFastCollision::ft_foot_left, py::return_value_policy::reference)
                .def("ft_wrist_right", &TalosFastCollision::ft_foot_right, py::return_value_policy::reference)

                .def("torques", &TalosFastCollision::torques);
            py::class_<TalosLight, Talos, std::shared_ptr<TalosLight>>(m, "TalosLight")
                .def(py::init<size_t, const std::string&, const std::vector<std::pair<std::string, std::string>>&>(),
                    py::arg("frequency") = 1000,
                    py::arg("urdf") = "talos/talos_fast.urdf",
                    py::arg("packages") = std::vector<std::pair<std::string, std::string>>({{"talos_description", "talos/talos_description"}}));

            py::class_<Ur3e, Robot, std::shared_ptr<Ur3e>>(m, "Ur3e")
                .def(py::init<size_t, const std::string&, const std::vector<std::pair<std::string, std::string>>&>(),
                    py::arg("frequency") = 1000,
                    py::arg("urdf") = "ur3e/ur3e.urdf",
                    py::arg("packages") = std::vector<std::pair<std::string, std::string>>({{"ur3e_description", "ur3e/ur3e_description"}}))
                .def("ft_wrist", &Ur3e::ft_wrist, py::return_value_policy::reference);

            py::class_<Ur3eHand, Ur3e, std::shared_ptr<Ur3eHand>>(m, "Ur3eHand")
                .def(py::init<size_t, const std::string&, const std::vector<std::pair<std::string, std::string>>&>(),
                    py::arg("frequency") = 1000,
                    py::arg("urdf") = "ur3e/ur3e_with_schunk_hand.urdf",
                    py::arg("packages") = std::vector<std::pair<std::string, std::string>>({{"ur3e_description", "ur3e/ur3e_description"}}));

            py::class_<Vx300, Robot, std::shared_ptr<Vx300>>(m, "Vx300")
                .def(py::init<const std::string&, const std::vector<std::pair<std::string, std::string>>&>(),
                    py::arg("urdf") = "vx300/vx300.urdf",
                    py::arg("packages") = std::vector<std::pair<std::string, std::string>>({{"interbotix_xsarm_descriptions", "vx300"}}));
        }
    } // namespace python
} // namespace robot_dart
