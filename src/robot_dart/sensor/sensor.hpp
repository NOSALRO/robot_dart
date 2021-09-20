#ifndef ROBOT_DART_SENSOR_SENSOR_HPP
#define ROBOT_DART_SENSOR_SENSOR_HPP

#include <robot_dart/robot.hpp>
#include <robot_dart/utils.hpp>

#include <Eigen/Core>
#include <Eigen/Geometry>

#include <memory>
#include <vector>

namespace dart {
    namespace dynamics {
        class BodyNode;
        class Joint;
    } // namespace dynamics
} // namespace dart

namespace robot_dart {
    class RobotDARTSimu;

    namespace sensor {
        class Sensor {
        public:
            Sensor(size_t freq = 40);
            virtual ~Sensor() {}

            void activate(bool enable = true);
            bool active() const;

            void set_simu(RobotDARTSimu* simu);
            const RobotDARTSimu* simu() const;

            size_t frequency() const;
            void set_frequency(size_t freq);

            void set_pose(const Eigen::Isometry3d& tf);
            const Eigen::Isometry3d& pose() const;

            void refresh(double t);

            virtual void init() = 0;
            // TO-DO: Maybe make this const?
            virtual void calculate(double t) = 0;

            virtual std::string type() const = 0;

            virtual void attach_to_body(dart::dynamics::BodyNode* body, const Eigen::Isometry3d& tf = Eigen::Isometry3d::Identity());
            void attach_to_body(const std::shared_ptr<Robot>& robot, const std::string& body_name, const Eigen::Isometry3d& tf = Eigen::Isometry3d::Identity()) { attach_to_body(robot->body_node(body_name), tf); }

            virtual void attach_to_joint(dart::dynamics::Joint* joint, const Eigen::Isometry3d& tf = Eigen::Isometry3d::Identity());
            void attach_to_joint(const std::shared_ptr<Robot>& robot, const std::string& joint_name, const Eigen::Isometry3d& tf = Eigen::Isometry3d::Identity()) { attach_to_joint(robot->joint(joint_name), tf); }

            void detach();
        protected:
            RobotDARTSimu* _simu = nullptr;
            bool _active;
            size_t _frequency;

            Eigen::Isometry3d _world_pose;

            bool _attaching_to_body = false, _attached_to_body = false;
            bool _attaching_to_joint = false, _attached_to_joint = false;
            Eigen::Isometry3d _attached_tf;
            dart::dynamics::BodyNode* _body_attached;
            dart::dynamics::Joint* _joint_attached;
        };
    } // namespace sensor
} // namespace robot_dart

#endif