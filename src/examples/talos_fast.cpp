#include <robot_dart/robot_dart_simu.hpp>
#include <robot_dart/robots/talos.hpp>

#ifdef GRAPHIC
#include <robot_dart/gui/magnum/graphics.hpp>
#endif

// fast Talos:
// - use dart for collision detection (instead of FCL) [only handle boxes and spheres]
// - the collisions are detected only for the feet
// - the urdf does not have the mimic (used for grippers)
int main()
{
    auto robot = std::make_shared<robot_dart::robots::TalosLight>();
    std::cout << "The model used is: [" << robot->model_filename() << "]" << std::endl;

    // Set actuator types to VELOCITY (for speed)
    robot->set_actuator_types("velocity");

    double dt = 0.001;
    robot_dart::RobotDARTSimu simu(dt);
    // we use dart, but only the feet are used
    simu.set_collision_detector("dart");
#ifdef GRAPHIC
    auto graphics = std::make_shared<robot_dart::gui::magnum::Graphics>();
    simu.set_graphics(graphics);
    graphics->look_at({0., 3.5, 2.}, {0., 0., 0.25});
    graphics->record_video("talos_light.mp4");
#endif
    simu.add_floor();
    simu.add_robot(robot);

    simu.set_control_freq(100);
    std::vector<std::string> dofs = {"arm_left_1_joint",
        "arm_left_2_joint",
        "arm_right_1_joint",
        "arm_right_2_joint",
        "torso_1_joint"};

    Eigen::VectorXd init_positions = robot->positions(dofs);

    auto start = std::chrono::steady_clock::now();
    while (simu.scheduler().next_time() < 20. && !simu.graphics()->done()) {
        if (simu.schedule(simu.control_freq())) {
            Eigen::VectorXd delta_pos(5);
            delta_pos << sin(simu.scheduler().current_time() * 2.),
                sin(simu.scheduler().current_time() * 2.),
                sin(simu.scheduler().current_time() * 2.),
                sin(simu.scheduler().current_time() * 2.),
                sin(simu.scheduler().current_time() * 2.);
            Eigen::VectorXd commands = (init_positions + delta_pos) - robot->positions(dofs);
            robot->set_commands(commands, dofs);
        }
        simu.step_world();
    }

    auto end = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsed_seconds = end - start;
    std::cout << "benchmark time: " << elapsed_seconds.count() << "s\n";

    robot.reset();
    return 0;
}
