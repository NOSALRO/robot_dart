#ifndef ROBOT_DART_SENSOR_SENSOR_HPP
#define ROBOT_DART_SENSOR_SENSOR_HPP

#include <Eigen/Core>
#include <Eigen/Geometry>

#include <memory>
#include <vector>

namespace dart {
    namespace dynamics {
        class BodyNode;
    }
} // namespace dart

namespace robot_dart {
    class RobotDARTSimu;

    namespace sensor {
        class Sensor {
        public:
            Sensor(RobotDARTSimu* simu, size_t freq = 40);
            virtual ~Sensor() {}

            void activate(bool enable = true);
            bool active() const;

            size_t frequency() const;
            void set_frequency(size_t freq);

            void set_pose(const Eigen::Isometry3d& tf);
            const Eigen::Isometry3d& pose() const;

            void refresh(double t);

            virtual void init() = 0;
            // TO-DO: Maybe make this const?
            virtual void calculate(double t) = 0;

            virtual std::string type() const = 0;

            virtual void attach_to(const std::string& body_name, const Eigen::Isometry3d& tf = Eigen::Isometry3d::Identity());

        protected:
            RobotDARTSimu* _simu;
            bool _active;
            size_t _frequency;

            Eigen::Isometry3d _world_pose;

            bool _attaching = false, _attached = false;
            std::string _attach_to;
            Eigen::Isometry3d _attached_tf;
            dart::dynamics::BodyNode* _body_attached;
        };
    } // namespace sensor
} // namespace robot_dart

#endif