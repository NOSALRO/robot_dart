#ifndef ROBOT_DART_ROBOT_HPP
#define ROBOT_DART_ROBOT_HPP

#include <dart/dynamics/Skeleton.hpp>

namespace robot_dart {

    class RobotControl;

    struct RobotDamage {
        RobotDamage() {}
        RobotDamage(const std::string& type, const std::string& data, void* extra = nullptr) : type(type), data(data), extra(extra) {}

        std::string type;
        std::string data;
        void* extra = nullptr;
    };

    class Robot : public std::enable_shared_from_this<Robot> {
    public:
        Robot();
        Robot(std::string model_file, std::vector<RobotDamage> damages = {}, std::string robot_name = "robot", bool absolute_path = false);
        Robot(dart::dynamics::SkeletonPtr skeleton, std::vector<RobotDamage> damages = {}, std::string robot_name = "robot");

        std::shared_ptr<Robot> clone() const;

        dart::dynamics::SkeletonPtr skeleton();

        std::vector<RobotDamage> damages() const;

        std::string name() const;

        void update(double t);

        void reinitControllers();

        std::vector<std::shared_ptr<RobotControl>> controllers() const;
        std::vector<std::shared_ptr<RobotControl>> activeControllers() const;

        void add_controller(const std::shared_ptr<RobotControl>& controller, double weight = 1.0);

        std::shared_ptr<RobotControl> controller(size_t index) const;

        // TO-DO: Add remove_controller

        void fix_to_world();
        // pose: Orientation-Position
        void free_from_world(const Eigen::Vector6d& pose = Eigen::Vector6d::Zero());

        bool fixed() const;
        bool free() const;

        void set_actuator_types(const std::vector<dart::dynamics::Joint::ActuatorType>& types);
        void set_actuator_types(dart::dynamics::Joint::ActuatorType type);

        void set_position_enforced(const std::vector<bool>& enforced);
        void set_position_enforced(bool enforced);

        void set_damping_coeff(const std::vector<double>& damps);
        void set_damping_coeff(double damp);

        Eigen::Vector3d body_pos(std::string body_name) const;
        Eigen::Matrix3d body_rot(std::string body_name) const;
        Eigen::Isometry3d body_trans(std::string body_name) const;

    protected:
        dart::dynamics::SkeletonPtr _load_model(std::string model_file, bool absolute_path);

        void _set_damages(const std::vector<RobotDamage>& damages);

        std::string _robot_name;
        dart::dynamics::SkeletonPtr _skeleton;
        std::vector<RobotDamage> _damages;
        std::vector<std::shared_ptr<RobotControl>> _controllers;
    };
} // namespace robot_dart

#endif
