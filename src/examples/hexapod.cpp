#include <iostream>
#include <robot_dart/robot_dart_simu.hpp>

#include <robot_dart/control/policy_control.hpp>

#include <hexapod_controller/hexapod_controller_simple.hpp>

#ifdef GRAPHIC
#include <robot_dart/graphics/graphics.hpp>
#endif

using namespace hexapod_controller;

struct HexaPolicy {
    void set_params(const std::vector<double>& ctrl)
    {
        _controller.set_parameters(ctrl);
    }

    size_t output_size() const { return 18; }

    Eigen::VectorXd query(const std::shared_ptr<robot_dart::Robot>& robot, double t)
    {
        auto angles = _controller.pos(t);

        Eigen::VectorXd target_positions = Eigen::VectorXd::Zero(18 + 6);
        for (size_t i = 0; i < angles.size(); i++)
            target_positions(i + 6) = ((i % 3 == 1) ? 1.0 : -1.0) * angles[i];

        Eigen::VectorXd q = robot->skeleton()->getPositions();
        Eigen::VectorXd q_err = target_positions - q;

        double gain = 1.0 / (dart::math::constants<double>::pi() * robot->skeleton()->getTimeStep());
        Eigen::VectorXd vel = q_err * gain;

        return vel.tail(18);
    }

    HexapodControllerSimple _controller;
};

int main()
{
    std::srand(std::time(NULL));
    auto global_robot = std::make_shared<robot_dart::Robot>("res/models/pexod.urdf");

    global_robot->set_position_enforced(true);

    global_robot->set_actuator_types(dart::dynamics::Joint::VELOCITY);

    auto g_robot = global_robot->clone();
    g_robot->skeleton()->setPosition(5, 0.2);

    // std::vector<double> ctrl(18, 0.);
    // g_robot->add_controller(std::make_shared<robot_dart::SimpleControl>());
    std::vector<double> ctrl = {1, 0, 0.5, 0.25, 0.25, 0.5, 1, 0.5, 0.5, 0.25, 0.75, 0.5, 1, 0, 0.5, 0.25, 0.25, 0.5, 1, 0, 0.5, 0.25, 0.75, 0.5, 1, 0.5, 0.5, 0.25, 0.25, 0.5, 1, 0, 0.5, 0.25, 0.75, 0.5};

    using HexaControl = robot_dart::control::PolicyControl<HexaPolicy>;
    g_robot->add_controller(std::make_shared<HexaControl>(ctrl));

    robot_dart::RobotDARTSimu simu;
#ifdef GRAPHIC
    simu.set_graphics(std::make_shared<robot_dart::graphics::Graphics>(simu.world()));
    std::static_pointer_cast<robot_dart::graphics::Graphics>(simu.graphics())->look_at({0.5, 3., 0.75}, {0.5, 0., 0.2});
#endif
    simu.add_floor();
    simu.add_robot(g_robot);
    simu.run(3);

    std::cout << g_robot->skeleton()->getPositions().head(6).tail(3).transpose() << std::endl;

    g_robot.reset();
    global_robot.reset();
    return 0;
}