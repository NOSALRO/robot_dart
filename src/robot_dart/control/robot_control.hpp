#ifndef ROBOT_DART_CONTROL_ROBOT_CONTROL
#define ROBOT_DART_CONTROL_ROBOT_CONTROL

#include <Eigen/Core>

#include <memory>
#include <vector>

#include <dart/config.hpp>

namespace robot_dart {
    class Robot;

    namespace control {

        class RobotControl {
        public:
            RobotControl();
            RobotControl(const std::vector<double>& ctrl, bool full_control = false);

            void set_parameters(const std::vector<double>& ctrl);
            std::vector<double> parameters() const;

            void init();

            void set_robot(const std::shared_ptr<Robot>& robot);
            std::shared_ptr<Robot> robot() const;

            void activate(bool enable = true);
            bool active() const;

            bool fully_controlled() const;
            void set_full_control(bool enable = true);

            double weight() const;
            void set_weight(double weight);

            virtual void configure() = 0;
            // TO-DO: Maybe make these 2 const?
            Eigen::VectorXd commands(double t);
            virtual Eigen::VectorXd calculate(double t) = 0;
            virtual std::shared_ptr<RobotControl> clone() const = 0;

            Eigen::VectorXd get_positions() const;
            void set_positions(const Eigen::VectorXd& positions);

            Eigen::VectorXd get_velocities() const;
            void set_velocities(const Eigen::VectorXd& velocities);

            Eigen::VectorXd get_accelerations() const;
            void set_accelerations(const Eigen::VectorXd& accelerations);

            Eigen::VectorXd get_forces() const;
            void set_forces(const Eigen::VectorXd& forces);

        protected:
            std::weak_ptr<Robot> _robot;
            std::vector<double> _ctrl;
            double _weight;
            bool _active, _full_control;
            size_t _start_dof, _dof, _control_dof;
#if DART_MAJOR_VERSION > 6 || (DART_MAJOR_VERSION == 6 && DART_MINOR_VERSION > 6)
            std::vector<size_t> _mimic_dofs;
#endif

            Eigen::VectorXd _get_vector_mimic(const Eigen::VectorXd& vec) const;
            Eigen::VectorXd _set_vector_mimic(const Eigen::VectorXd& vec) const;
        };
    } // namespace control
} // namespace robot_dart

#endif
