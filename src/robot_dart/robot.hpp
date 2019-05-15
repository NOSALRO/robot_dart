#ifndef ROBOT_DART_ROBOT_HPP
#define ROBOT_DART_ROBOT_HPP

#include <utility>

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

        void set_actuator_type(size_t index, dart::dynamics::Joint::ActuatorType type);
        void set_actuator_types(const std::vector<dart::dynamics::Joint::ActuatorType>& types);
        void set_actuator_types(dart::dynamics::Joint::ActuatorType type);

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

        Eigen::Vector3d body_pos(const std::string& body_name) const;
        Eigen::Matrix3d body_rot(const std::string& body_name) const;
        Eigen::Isometry3d body_trans(const std::string& body_name) const;

        // helper functions
        // pose: Orientation-Position
        static std::shared_ptr<Robot> create_box(const Eigen::Vector3d& dims, const Eigen::Vector6d& pose = Eigen::Vector6d::Zero(), const std::string& type = "free", double mass = 1.0, const Eigen::Vector4d& color = dart::Color::Red(1.0), const std::string& box_name = "box");

        static std::shared_ptr<Robot> create_ellipsoid(const Eigen::Vector3d& dims, const Eigen::Vector6d& pose = Eigen::Vector6d::Zero(), const std::string& type = "free", double mass = 1.0, const Eigen::Vector4d& color = dart::Color::Red(1.0), const std::string& ellipsoid_name = "ellipsoid");

    protected:
        dart::dynamics::SkeletonPtr _load_model(const std::string& filename, const std::vector<std::pair<std::string, std::string>>& packages = std::vector<std::pair<std::string, std::string>>(), bool is_urdf_string = false);

        void _set_damages(const std::vector<RobotDamage>& damages);

        std::string _robot_name;
        dart::dynamics::SkeletonPtr _skeleton;
        std::vector<RobotDamage> _damages;
        std::vector<std::shared_ptr<control::RobotControl>> _controllers;
    };
} // namespace robot_dart

#endif
