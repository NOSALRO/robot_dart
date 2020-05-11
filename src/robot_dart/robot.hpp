#ifndef ROBOT_DART_ROBOT_HPP
#define ROBOT_DART_ROBOT_HPP

#include <utility>

#include <dart/dynamics/MeshShape.hpp>
#include <dart/dynamics/Skeleton.hpp>

namespace robot_dart {
    namespace control {
        class RobotControl;
    }

    struct RobotDamage {
        RobotDamage() {}
        RobotDamage(const std::string& type, const std::string& data, void* extra = nullptr) : type(type), data(data), extra(extra) {}

        std::string type;
        std::string data;
        void* extra = nullptr;
    };

    class Robot : public std::enable_shared_from_this<Robot> {
    public:
        Robot(const std::string& model_file, const std::vector<std::pair<std::string, std::string>>& packages, const std::string& robot_name = "robot", bool is_urdf_string = false, std::vector<RobotDamage> damages = {});
        Robot(const std::string& model_file, const std::string& robot_name = "robot", bool is_urdf_string = false, std::vector<RobotDamage> damages = {});
        Robot(dart::dynamics::SkeletonPtr skeleton, const std::string& robot_name = "robot", std::vector<RobotDamage> damages = {});

        std::shared_ptr<Robot> clone() const;

        dart::dynamics::SkeletonPtr skeleton();

        std::vector<RobotDamage> damages() const;

        const std::string& name() const;

        void update(double t);

        void reinit_controllers();

        size_t num_controllers() const;
        std::vector<std::shared_ptr<control::RobotControl>> controllers() const;
        std::vector<std::shared_ptr<control::RobotControl>> active_controllers() const;

        std::shared_ptr<control::RobotControl> controller(size_t index) const;

        void add_controller(const std::shared_ptr<control::RobotControl>& controller, double weight = 1.0);
        void remove_controller(const std::shared_ptr<control::RobotControl>& controller);
        void remove_controller(size_t index);
        void clear_controllers();

        void fix_to_world();
        // pose: Orientation-Position
        void free_from_world(const Eigen::Vector6d& pose = Eigen::Vector6d::Zero());

        bool fixed() const;
        bool free() const;

        void set_actuator_type(size_t index, dart::dynamics::Joint::ActuatorType type, bool override_mimic = false);
        void set_actuator_types(const std::vector<dart::dynamics::Joint::ActuatorType>& types, bool override_mimic = false);
        void set_actuator_types(dart::dynamics::Joint::ActuatorType type, bool override_mimic = false);

        dart::dynamics::Joint::ActuatorType actuator_type(size_t index) const;
        std::vector<dart::dynamics::Joint::ActuatorType> actuator_types() const;

        void set_position_enforced(size_t dof, bool enforced);
        void set_position_enforced(const std::vector<bool>& enforced);
        void set_position_enforced(bool enforced);

        bool position_enforced(size_t dof) const;
        std::vector<bool> position_enforced() const;

        void set_damping_coeff(size_t dof, double damp);
        void set_damping_coeffs(const std::vector<double>& damps);
        void set_damping_coeffs(double damp);

        double damping_coeff(size_t dof) const;
        std::vector<double> damping_coeffs() const;

        void set_cfriction_coeff(size_t dof, double cfriction);
        void set_cfriction_coeffs(const std::vector<double>& cfrictions);
        void set_cfriction_coeffs(double cfriction);

        double cfriction_coeff(size_t dof) const;
        std::vector<double> cfriction_coeffs() const;

        Eigen::Isometry3d base_pose() const;
        // Set the pose of the robot base (changes the transformation of the parent joint of the root body)
        void set_base_pose(const Eigen::Isometry3d& tf);

        size_t num_dofs() const;
        size_t num_joints() const;
        size_t num_bodies() const;

        Eigen::Vector3d com() const;
        Eigen::Vector6d com_velocity() const;
        Eigen::Vector6d com_acceleration() const;

        Eigen::VectorXd positions() const;
        void set_positions(const Eigen::VectorXd& positions);

        Eigen::VectorXd velocities() const;
        void set_velocities(const Eigen::VectorXd& velocities);

        Eigen::VectorXd accelerations() const;
        void set_accelerations(const Eigen::VectorXd& accelerations);

        Eigen::VectorXd forces() const;
        void set_forces(const Eigen::VectorXd& forces);

        std::pair<Eigen::Vector6d, Eigen::Vector6d> force_torque(size_t joint_index) const;

        void set_external_force(const std::string& body_name, const Eigen::Vector3d& force, const Eigen::Vector3d& offset = Eigen::Vector3d::Zero(), bool force_local = false, bool offset_local = true);
        void set_external_force(size_t body_index, const Eigen::Vector3d& force, const Eigen::Vector3d& offset = Eigen::Vector3d::Zero(), bool force_local = false, bool offset_local = true);
        void add_external_force(const std::string& body_name, const Eigen::Vector3d& force, const Eigen::Vector3d& offset = Eigen::Vector3d::Zero(), bool force_local = false, bool offset_local = true);
        void add_external_force(size_t body_index, const Eigen::Vector3d& force, const Eigen::Vector3d& offset = Eigen::Vector3d::Zero(), bool force_local = false, bool offset_local = true);

        void set_external_torque(const std::string& body_name, const Eigen::Vector3d& torque, bool local = false);
        void set_external_torque(size_t body_index, const Eigen::Vector3d& torque, bool local = false);
        void add_external_torque(const std::string& body_name, const Eigen::Vector3d& torque, bool local = false);
        void add_external_torque(size_t body_index, const Eigen::Vector3d& torque, bool local = false);

        void clear_external_forces();

        Eigen::Vector6d external_forces(const std::string& body_name) const;
        Eigen::Vector6d external_forces(size_t body_index) const;

        Eigen::Isometry3d body_pose(const std::string& body_name) const;
        Eigen::Isometry3d body_pose(size_t body_index) const;

        std::string body_name(size_t body_index) const;
        void set_body_name(size_t body_index, const std::string& body_name);

        double body_mass(const std::string& body_name) const;
        double body_mass(size_t body_index) const;
        void set_body_mass(const std::string& body_name, double mass);
        void set_body_mass(size_t body_index, double mass);

        std::string dof_name(size_t dof_index) const;
        std::string joint_name(size_t joint_index) const;
        void set_joint_name(size_t joint_index, const std::string& joint_name);

        void set_color_mode(dart::dynamics::MeshShape::ColorMode color_mode);
        void set_color_mode(dart::dynamics::MeshShape::ColorMode color_mode, const std::string& body_name);

        // helper functions
        // pose: Orientation-Position
        static std::shared_ptr<Robot> create_box(const Eigen::Vector3d& dims, const Eigen::Vector6d& pose = Eigen::Vector6d::Zero(), const std::string& type = "free", double mass = 1.0, const Eigen::Vector4d& color = dart::Color::Red(1.0), const std::string& box_name = "box");

        static std::shared_ptr<Robot> create_ellipsoid(const Eigen::Vector3d& dims, const Eigen::Vector6d& pose = Eigen::Vector6d::Zero(), const std::string& type = "free", double mass = 1.0, const Eigen::Vector4d& color = dart::Color::Red(1.0), const std::string& ellipsoid_name = "ellipsoid");

    protected:
        dart::dynamics::SkeletonPtr _load_model(const std::string& filename, const std::vector<std::pair<std::string, std::string>>& packages = std::vector<std::pair<std::string, std::string>>(), bool is_urdf_string = false);

        void _set_damages(const std::vector<RobotDamage>& damages);
        void _set_color_mode(dart::dynamics::MeshShape::ColorMode color_mode, dart::dynamics::SkeletonPtr skel);
        void _set_color_mode(dart::dynamics::MeshShape::ColorMode color_mode, dart::dynamics::ShapeNode* sn);

        std::string _robot_name;
        dart::dynamics::SkeletonPtr _skeleton;
        std::vector<RobotDamage> _damages;
        std::vector<std::shared_ptr<control::RobotControl>> _controllers;
    };
} // namespace robot_dart

#endif
