#ifndef ROBOT_DART_ROBOT_HPP
#define ROBOT_DART_ROBOT_HPP

#include <unordered_map>

#include <dart/dynamics/MeshShape.hpp>
#include <dart/dynamics/Skeleton.hpp>

namespace robot_dart {
    namespace control {
        class RobotControl;
    }
    struct RobotDamage {
        RobotDamage() {}
        RobotDamage(const std::string& type, const std::string& data, void* extra = nullptr)
            : type(type), data(data), extra(extra)
        {
        }

        std::string type;
        std::string data;
        void* extra = nullptr;
    };

    class Robot : public std::enable_shared_from_this<Robot> {
    public:
        Robot(const std::string& model_file, const std::vector<std::pair<std::string, std::string>>& packages, const std::string& robot_name = "robot", bool is_urdf_string = false, bool cast_shadows = true, std::vector<RobotDamage> damages = {});
        Robot(const std::string& model_file, const std::string& robot_name = "robot", bool is_urdf_string = false, bool cast_shadows = true, std::vector<RobotDamage> damages = {});
        Robot(dart::dynamics::SkeletonPtr skeleton, const std::string& robot_name = "robot", bool cast_shadows = true, std::vector<RobotDamage> damages = {});

        std::shared_ptr<Robot> clone() const;
        std::shared_ptr<Robot> clone_ghost(const std::string& ghost_name = "ghost", const Eigen::Vector4d& ghost_color = {0.3, 0.3, 0.3, 0.7}) const;

        dart::dynamics::SkeletonPtr skeleton();
        dart::dynamics::BodyNode* body_node(const std::string& body_name);
        dart::dynamics::BodyNode* body_node(size_t body_index);

        dart::dynamics::Joint* joint(const std::string& joint_name);
        dart::dynamics::Joint* joint(size_t joint_index);

        dart::dynamics::DegreeOfFreedom* dof(const std::string& dof_name);
        dart::dynamics::DegreeOfFreedom* dof(size_t dof_index);

        std::vector<RobotDamage> damages() const;

        const std::string& name() const;
        // to use the same urdf somewhere else
        const std::string& model_filename() const { return _model_filename; }
        const std::vector<std::pair<std::string, std::string>>& model_packages() const { return _packages; }

        void update(double t);

        void reinit_controllers();

        size_t num_controllers() const;
        std::vector<std::shared_ptr<control::RobotControl>> controllers() const;
        std::vector<std::shared_ptr<control::RobotControl>> active_controllers() const;

        std::shared_ptr<control::RobotControl> controller(size_t index) const;

        void add_controller(
            const std::shared_ptr<control::RobotControl>& controller, double weight = 1.0);
        void remove_controller(const std::shared_ptr<control::RobotControl>& controller);
        void remove_controller(size_t index);
        void clear_controllers();

        void fix_to_world();
        // pose: Orientation-Position
        void free_from_world(const Eigen::Vector6d& pose = Eigen::Vector6d::Zero());

        bool fixed() const;
        bool free() const;

        void reset();

        void clear_external_forces();
        void clear_internal_forces();
        void reset_commands();

        // actuator type can be: torque, servo, velocity, passive, locked, mimic (only for completeness, use set_mimic to use this)
        // Be careful that actuator types are per joint and not per DoF
        void set_actuator_types(const std::string& type, const std::vector<std::string>& joint_names = {}, bool override_mimic = false, bool override_base = false);
        void set_actuator_type(const std::string& type, const std::string& joint_name, bool override_mimic = false, bool override_base = false);
        void set_mimic(const std::string& joint_name, const std::string& mimic_joint_name, double multiplier = 1., double offset = 0.);

        std::string actuator_type(const std::string& joint_name) const;
        std::vector<std::string> actuator_types(const std::vector<std::string>& joint_names = {}) const;

        void set_position_enforced(const std::vector<bool>& enforced, const std::vector<std::string>& dof_names = {});
        void set_position_enforced(bool enforced, const std::vector<std::string>& dof_names = {});

        std::vector<bool> position_enforced(const std::vector<std::string>& dof_names = {}) const;

        void set_damping_coeffs(const std::vector<double>& damps, const std::vector<std::string>& dof_names = {});
        void set_damping_coeffs(double damp, const std::vector<std::string>& dof_names = {});

        std::vector<double> damping_coeffs(const std::vector<std::string>& dof_names = {}) const;

        void set_coulomb_coeffs(const std::vector<double>& cfrictions, const std::vector<std::string>& dof_names = {});
        void set_coulomb_coeffs(double cfriction, const std::vector<std::string>& dof_names = {});

        std::vector<double> coulomb_coeffs(const std::vector<std::string>& dof_names = {}) const;

        void set_spring_stiffnesses(const std::vector<double>& stiffnesses, const std::vector<std::string>& dof_names = {});
        void set_spring_stiffnesses(double stiffness, const std::vector<std::string>& dof_names = {});

        std::vector<double> spring_stiffnesses(const std::vector<std::string>& dof_names = {}) const;

        // the friction direction is in local frame
        void set_friction_dir(const std::string& body_name, const Eigen::Vector3d& direction);
        void set_friction_dir(size_t body_index, const Eigen::Vector3d& direction);
        Eigen::Vector3d friction_dir(const std::string& body_name);
        Eigen::Vector3d friction_dir(size_t body_index);

        void set_friction_coeff(const std::string& body_name, double value);
        void set_friction_coeff(size_t body_index, double value);
        void set_friction_coeffs(double value);
        double friction_coeff(const std::string& body_name);
        double friction_coeff(size_t body_index);

        void set_secondary_friction_coeff(const std::string& body_name, double value);
        void set_secondary_friction_coeff(size_t body_index, double value);
        void set_secondary_friction_coeffs(double value);
        double secondary_friction_coeff(const std::string& body_name);
        double secondary_friction_coeff(size_t body_index);

        void set_restitution_coeff(const std::string& body_name, double value);
        void set_restitution_coeff(size_t body_index, double value);
        void set_restitution_coeffs(double value);
        double restitution_coeff(const std::string& body_name);
        double restitution_coeff(size_t body_index);

        Eigen::Isometry3d base_pose() const;
        Eigen::Vector6d base_pose_vec() const;
        void set_base_pose(const Eigen::Isometry3d& tf);
        void set_base_pose(const Eigen::Vector6d& pose);

        size_t num_dofs() const;
        size_t num_joints() const;
        size_t num_bodies() const;

        Eigen::Vector3d com() const;
        Eigen::Vector6d com_velocity() const;
        Eigen::Vector6d com_acceleration() const;

        Eigen::VectorXd positions(const std::vector<std::string>& dof_names = {}) const;
        void set_positions(const Eigen::VectorXd& positions, const std::vector<std::string>& dof_names = {});

        Eigen::VectorXd position_lower_limits(const std::vector<std::string>& dof_names = {}) const;
        void set_position_lower_limits(const Eigen::VectorXd& positions, const std::vector<std::string>& dof_names = {});
        Eigen::VectorXd position_upper_limits(const std::vector<std::string>& dof_names = {}) const;
        void set_position_upper_limits(const Eigen::VectorXd& positions, const std::vector<std::string>& dof_names = {});

        Eigen::VectorXd velocities(const std::vector<std::string>& dof_names = {}) const;
        void set_velocities(const Eigen::VectorXd& velocities, const std::vector<std::string>& dof_names = {});

        Eigen::VectorXd velocity_lower_limits(const std::vector<std::string>& dof_names = {}) const;
        void set_velocity_lower_limits(const Eigen::VectorXd& velocities, const std::vector<std::string>& dof_names = {});
        Eigen::VectorXd velocity_upper_limits(const std::vector<std::string>& dof_names = {}) const;
        void set_velocity_upper_limits(const Eigen::VectorXd& velocities, const std::vector<std::string>& dof_names = {});

        Eigen::VectorXd accelerations(const std::vector<std::string>& dof_names = {}) const;
        void set_accelerations(const Eigen::VectorXd& accelerations, const std::vector<std::string>& dof_names = {});

        Eigen::VectorXd acceleration_lower_limits(const std::vector<std::string>& dof_names = {}) const;
        void set_acceleration_lower_limits(const Eigen::VectorXd& accelerations, const std::vector<std::string>& dof_names = {});
        Eigen::VectorXd acceleration_upper_limits(const std::vector<std::string>& dof_names = {}) const;
        void set_acceleration_upper_limits(const Eigen::VectorXd& accelerations, const std::vector<std::string>& dof_names = {});

        Eigen::VectorXd forces(const std::vector<std::string>& dof_names = {}) const;
        void set_forces(const Eigen::VectorXd& forces, const std::vector<std::string>& dof_names = {});

        Eigen::VectorXd force_lower_limits(const std::vector<std::string>& dof_names = {}) const;
        void set_force_lower_limits(const Eigen::VectorXd& forces, const std::vector<std::string>& dof_names = {});
        Eigen::VectorXd force_upper_limits(const std::vector<std::string>& dof_names = {}) const;
        void set_force_upper_limits(const Eigen::VectorXd& forces, const std::vector<std::string>& dof_names = {});

        Eigen::VectorXd commands(const std::vector<std::string>& dof_names = {}) const;
        void set_commands(const Eigen::VectorXd& commands, const std::vector<std::string>& dof_names = {});

        std::pair<Eigen::Vector6d, Eigen::Vector6d> force_torque(size_t joint_index) const;

        void set_external_force(const std::string& body_name, const Eigen::Vector3d& force, const Eigen::Vector3d& offset = Eigen::Vector3d::Zero(), bool force_local = false, bool offset_local = true);
        void set_external_force(size_t body_index, const Eigen::Vector3d& force, const Eigen::Vector3d& offset = Eigen::Vector3d::Zero(), bool force_local = false, bool offset_local = true);
        void add_external_force(const std::string& body_name, const Eigen::Vector3d& force, const Eigen::Vector3d& offset = Eigen::Vector3d::Zero(), bool force_local = false, bool offset_local = true);
        void add_external_force(size_t body_index, const Eigen::Vector3d& force, const Eigen::Vector3d& offset = Eigen::Vector3d::Zero(), bool force_local = false, bool offset_local = true);

        void set_external_torque(const std::string& body_name, const Eigen::Vector3d& torque, bool local = false);
        void set_external_torque(size_t body_index, const Eigen::Vector3d& torque, bool local = false);
        void add_external_torque(const std::string& body_name, const Eigen::Vector3d& torque, bool local = false);
        void add_external_torque(size_t body_index, const Eigen::Vector3d& torque, bool local = false);

        Eigen::Vector6d external_forces(const std::string& body_name) const;
        Eigen::Vector6d external_forces(size_t body_index) const;

        Eigen::Isometry3d body_pose(const std::string& body_name) const;
        Eigen::Isometry3d body_pose(size_t body_index) const;

        Eigen::Vector6d body_pose_vec(const std::string& body_name) const;
        Eigen::Vector6d body_pose_vec(size_t body_index) const;

        Eigen::Vector6d body_velocity(const std::string& body_name) const;
        Eigen::Vector6d body_velocity(size_t body_index) const;

        Eigen::Vector6d body_acceleration(const std::string& body_name) const;
        Eigen::Vector6d body_acceleration(size_t body_index) const;

        std::vector<std::string> body_names() const;
        std::string body_name(size_t body_index) const;
        void set_body_name(size_t body_index, const std::string& body_name);
        size_t body_index(const std::string& body_name) const;

        double body_mass(const std::string& body_name) const;
        double body_mass(size_t body_index) const;
        void set_body_mass(const std::string& body_name, double mass);
        void set_body_mass(size_t body_index, double mass);
        void add_body_mass(const std::string& body_name, double mass);
        void add_body_mass(size_t body_index, double mass);

        Eigen::MatrixXd jacobian(const std::string& body_name, const std::vector<std::string>& dof_names = {}) const;
        Eigen::MatrixXd jacobian_deriv(const std::string& body_name, const std::vector<std::string>& dof_names = {}) const;

        Eigen::MatrixXd com_jacobian(const std::vector<std::string>& dof_names = {}) const;
        Eigen::MatrixXd com_jacobian_deriv(const std::vector<std::string>& dof_names = {}) const;

        Eigen::MatrixXd mass_matrix(const std::vector<std::string>& dof_names = {}) const;
        Eigen::MatrixXd aug_mass_matrix(const std::vector<std::string>& dof_names = {}) const;
        Eigen::MatrixXd inv_mass_matrix(const std::vector<std::string>& dof_names = {}) const;
        Eigen::MatrixXd inv_aug_mass_matrix(const std::vector<std::string>& dof_names = {}) const;

        Eigen::VectorXd coriolis_forces(const std::vector<std::string>& dof_names = {}) const;
        Eigen::VectorXd gravity_forces(const std::vector<std::string>& dof_names = {}) const;
        Eigen::VectorXd coriolis_gravity_forces(const std::vector<std::string>& dof_names = {}) const;

        // Get only the part of vector for DOFs in dof_names
        Eigen::VectorXd vec_dof(const Eigen::VectorXd& vec, const std::vector<std::string>& dof_names) const;

        void update_joint_dof_maps();
        const std::unordered_map<std::string, size_t>& dof_map() const;
        const std::unordered_map<std::string, size_t>& joint_map() const;

        std::vector<std::string> dof_names(bool filter_mimics = false, bool filter_locked = false, bool filter_passive = false) const;
        std::vector<std::string> mimic_dof_names() const;
        std::vector<std::string> locked_dof_names() const;
        std::vector<std::string> passive_dof_names() const;
        std::string dof_name(size_t dof_index) const;
        size_t dof_index(const std::string& dof_name) const;

        std::vector<std::string> joint_names() const;
        std::string joint_name(size_t joint_index) const;
        void set_joint_name(size_t joint_index, const std::string& joint_name);
        size_t joint_index(const std::string& joint_name) const;

        // MATERIAL_COLOR, COLOR_INDEX, SHAPE_COLOR
        // This applies only to MeshShapes. Color mode can be: "material", "assimp", or "aspect"
        // "material" -> uses the color of the material in the mesh file
        // "assimp" -> uses the color specified by aiMesh::mColor
        // "aspect" -> uses the color defined in the VisualAspect (if not changed, this is what read from the URDF)
        void set_color_mode(const std::string& color_mode);
        void set_color_mode(const std::string& color_mode, const std::string& body_name);

        void set_self_collision(bool enable_self_collisions = true, bool enable_adjacent_collisions = false);
        bool self_colliding() const;
        // This returns true if self colliding AND adjacent checks are on
        bool adjacent_colliding() const;

        // GUI options
        void set_cast_shadows(bool cast_shadows = true);
        bool cast_shadows() const;

        void set_ghost(bool ghost = true);
        bool ghost() const;

        void set_draw_axis(const std::string& body_name, double size = 0.25);
        void remove_all_drawing_axis();
        const std::vector<std::pair<dart::dynamics::BodyNode*, double>>& drawing_axes() const;

        // helper functions
        static std::shared_ptr<Robot> create_box(const Eigen::Vector3d& dims,
            const Eigen::Isometry3d& tf, const std::string& type = "free",
            double mass = 1.0, const Eigen::Vector4d& color = dart::Color::Red(1.0),
            const std::string& box_name = "box");
        // pose: 6D log_map
        static std::shared_ptr<Robot> create_box(const Eigen::Vector3d& dims,
            const Eigen::Vector6d& pose = Eigen::Vector6d::Zero(), const std::string& type = "free",
            double mass = 1.0, const Eigen::Vector4d& color = dart::Color::Red(1.0),
            const std::string& box_name = "box");

        static std::shared_ptr<Robot> create_ellipsoid(const Eigen::Vector3d& dims,
            const Eigen::Isometry3d& tf, const std::string& type = "free",
            double mass = 1.0, const Eigen::Vector4d& color = dart::Color::Red(1.0),
            const std::string& ellipsoid_name = "ellipsoid");
        // pose: 6D log_map
        static std::shared_ptr<Robot> create_ellipsoid(const Eigen::Vector3d& dims,
            const Eigen::Vector6d& pose = Eigen::Vector6d::Zero(), const std::string& type = "free",
            double mass = 1.0, const Eigen::Vector4d& color = dart::Color::Red(1.0),
            const std::string& ellipsoid_name = "ellipsoid");

    protected:
        std::string _get_path(const std::string& filename) const;
        dart::dynamics::SkeletonPtr _load_model(const std::string& filename, const std::vector<std::pair<std::string, std::string>>& packages = std::vector<std::pair<std::string, std::string>>(), bool is_urdf_string = false);

        void _set_damages(const std::vector<RobotDamage>& damages);
        void _set_color_mode(dart::dynamics::MeshShape::ColorMode color_mode, dart::dynamics::SkeletonPtr skel);
        void _set_color_mode(dart::dynamics::MeshShape::ColorMode color_mode, dart::dynamics::ShapeNode* sn);
        void _set_actuator_type(size_t joint_index, dart::dynamics::Joint::ActuatorType type, bool override_mimic = false, bool override_base = false);
        void _set_actuator_types(const std::vector<dart::dynamics::Joint::ActuatorType>& types, bool override_mimic = false, bool override_base = false);
        void _set_actuator_types(dart::dynamics::Joint::ActuatorType type, bool override_mimic = false, bool override_base = false);

        dart::dynamics::Joint::ActuatorType _actuator_type(size_t joint_index) const;
        std::vector<dart::dynamics::Joint::ActuatorType> _actuator_types() const;

        Eigen::MatrixXd _jacobian(const Eigen::MatrixXd& full_jacobian, const std::vector<std::string>& dof_names) const;
        Eigen::MatrixXd _mass_matrix(const Eigen::MatrixXd& full_mass_matrix, const std::vector<std::string>& dof_names) const;

        std::string _robot_name;
        std::string _model_filename;
        std::vector<std::pair<std::string, std::string>> _packages;
        dart::dynamics::SkeletonPtr _skeleton;
        std::vector<RobotDamage> _damages;
        std::vector<std::shared_ptr<control::RobotControl>> _controllers;
        std::unordered_map<std::string, size_t> _dof_map, _joint_map;
        bool _cast_shadows;
        bool _is_ghost;
        std::vector<std::pair<dart::dynamics::BodyNode*, double>> _axis_shapes;
    };
} // namespace robot_dart

#endif