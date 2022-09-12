#include <robot_dart/control/robot_control.hpp>
#include <robot_dart/robot_dart_simu.hpp>
#include <robot_dart/robots/arm.hpp>

#ifdef GRAPHIC
#include <robot_dart/gui/magnum/graphics.hpp>
#endif

// @ROBOT_CONTROL@
class MyController : public robot_dart::control::RobotControl {
public:
    MyController() : robot_dart::control::RobotControl() {}

    MyController(const Eigen::VectorXd& ctrl, bool full_control) : robot_dart::control::RobotControl(ctrl, full_control) {}
    MyController(const Eigen::VectorXd& ctrl, const std::vector<std::string>& dof_names) : robot_dart::control::RobotControl(ctrl, dof_names) {}

    void configure() override
    {
        _active = true;
    }

    Eigen::VectorXd calculate(double) override
    {
        auto robot = _robot.lock();
        Eigen::VectorXd cmd = 100. * (_ctrl - robot->positions(_controllable_dofs));
        return cmd;
    }
    std::shared_ptr<robot_dart::control::RobotControl> clone() const override
    {
        return std::make_shared<MyController>(*this);
    }
};
// @ROBOT_CONTROL_END@

int main()
{
    // Load robot from URDF
    auto robot = std::make_shared<robot_dart::robots::Arm>();

    // Initiate custom controller
    Eigen::VectorXd ctrl(4);
    ctrl << 0.0, 1.57, -0.5, 0.7;
    auto control = std::make_shared<MyController>(ctrl, false);
    // Add it to the robot
    robot->add_controller(control, 1.);

    // Print initial positions of the robot
    std::cout << robot->positions() << std::endl;

    // Create simulator object
    robot_dart::RobotDARTSimu simu(0.001); // dt=0.001, 1KHz simulation

    // Create graphics
#ifdef GRAPHIC
    auto graphics = std::make_shared<robot_dart::gui::magnum::Graphics>();
    simu.set_graphics(graphics);
    graphics->look_at({0.5, 3., 0.75}, {0.5, 0., 0.2});
#endif
    // Add robot and floor to the simulation
    simu.add_floor();
    simu.add_robot(robot);
    simu.run(5.);
}