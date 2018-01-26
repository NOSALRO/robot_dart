#ifndef ROBOT_DART_ROBOT_CONTROL
#define ROBOT_DART_ROBOT_CONTROL

#include <Eigen/Core>

#include <memory>
#include <vector>

namespace robot_dart {

    class Robot;

    class RobotControl {
    public:
        RobotControl() : _active(false) {}
        RobotControl(const std::vector<double>& ctrl) : _ctrl(ctrl), _active(false) {}

        void set_parameters(const std::vector<double>& ctrl) { _ctrl = ctrl; }

        std::vector<double> parameters() const { return _ctrl; }

        virtual Eigen::VectorXd commands(double t) = 0;

        virtual void init() {}

        virtual void set_robot(const std::shared_ptr<Robot>& robot) { _robot = robot; }

        bool active() const { return _active; }

    protected:
        std::shared_ptr<Robot> _robot;
        std::vector<double> _ctrl;
        bool _active;
    };
} // namespace robot_dart

#endif
