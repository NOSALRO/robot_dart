#ifndef ROBOT_DART_ROBOT_CONTROL
#define ROBOT_DART_ROBOT_CONTROL

#include <Eigen/Core>

#include <memory>
#include <vector>

namespace robot_dart {

    class Robot;

    class RobotControl {
    public:
        RobotControl();
        RobotControl(const std::vector<double>& ctrl, bool full_control = false);

        void set_parameters(const std::vector<double>& ctrl);
        std::vector<double> parameters() const;

        void init();

        void set_robot(const std::shared_ptr<Robot>& robot);
        std::shared_ptr<Robot> robot() const;

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

    protected:
        std::shared_ptr<Robot> _robot;
        std::vector<double> _ctrl;
        double _weight;
        bool _active, _full_control;
        size_t _start_dof, _dof, _control_dof;
    };
} // namespace robot_dart

#endif
