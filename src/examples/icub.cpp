#include <robot_dart/robot_dart_simu.hpp>
#include <robot_dart/robots/icub.hpp>

#ifdef GRAPHIC
#include <robot_dart/gui/magnum/graphics.hpp>
#endif

int main()
{
    // @LOAD_IICUB@
    auto robot = std::make_shared<robot_dart::robots::ICub>();
    // Set actuator types to VELOCITY motors so that they stay in position without any controller
    robot->set_actuator_types("velocity");
    robot_dart::RobotDARTSimu simu(0.001);
    simu.set_collision_detector("fcl");
    // @LOAD_IICUB_END@
#ifdef GRAPHIC
    auto graphics = std::make_shared<robot_dart::gui::magnum::Graphics>();
    simu.set_graphics(graphics);
    graphics->look_at({0., 3.5, 2.}, {0., 0., 0.25});
    //    graphics->record_video("icub.mp4");
#endif
    simu.add_checkerboard_floor();
    simu.add_robot(robot);
    auto ghost = robot->clone_ghost();
    ghost->skeleton()->setPosition(4, -1.57);
    ghost->skeleton()->setPosition(5, 1.1);
    simu.add_robot(ghost);
#if DART_VERSION_AT_MOST(6, 12, 99)
    // fix a bug for older versions of DART
    robot->set_color_mode("material");
#endif

    simu.set_control_freq(100); // 100 Hz
    std::vector<std::string> dofs = {
        "l_knee",
        "r_knee",
        "l_ankle_pitch",
        "r_ankle_pitch",
        "l_hip_pitch",
        "r_hip_pitch",
        "l_shoulder_pitch",
        "l_shoulder_roll",
        "r_shoulder_pitch",
        "r_shoulder_roll",
    };

    // Format Eigen to std::cout
    Eigen::IOFormat fmt(Eigen::StreamPrecision, Eigen::DontAlignCols, " ", "\n", "", "");
    std::cout.precision(4);

    auto start = std::chrono::steady_clock::now();
    while (simu.scheduler().next_time() < 20 && !simu.graphics()->done()) {
        if (simu.schedule(simu.control_freq())) {
            Eigen::VectorXd commands(dofs.size());
            commands[0] = -0.45 * sin(simu.scheduler().current_time() * 2);
            commands[1] = -0.45 * sin(simu.scheduler().current_time() * 2);
            commands[2] = -0.35 * sin(simu.scheduler().current_time() * 2);
            commands[3] = -0.35 * sin(simu.scheduler().current_time() * 2);
            commands[4] = 0.15 * sin(simu.scheduler().current_time() * 2);
            commands[5] = 0.15 * sin(simu.scheduler().current_time() * 2);
            commands[6] = -0.15 * sin(simu.scheduler().current_time() * 2);
            commands[7] = 0.15 * sin(simu.scheduler().current_time() * 2);
            commands[8] = -0.15 * sin(simu.scheduler().current_time() * 2);
            commands[9] = 0.15 * sin(simu.scheduler().current_time() * 2);
            robot->set_commands(commands, dofs);
        }
        simu.step_world();

        // Print IMU measurements
        if (simu.schedule(robot->imu().frequency())) {
            // @ICUB_PRINT_IMU@
            std::cout << "Angular    Position: " << robot->imu().angular_position_vec().transpose().format(fmt) << std::endl;
            std::cout << "Angular    Velocity: " << robot->imu().angular_velocity().transpose().format(fmt) << std::endl;
            std::cout << "Linear Acceleration: " << robot->imu().linear_acceleration().transpose().format(fmt) << std::endl;
            std::cout << "=================================" << std::endl;
            // @ICUB_PRINT_IMU_END@
        }

        // Print FT measurements
        if (simu.schedule(robot->ft_foot_left().frequency())) {
            // @ICUB_PRINT_FT@
            std::cout << "FT ( force): " << robot->ft_foot_left().force().transpose().format(fmt) << std::endl;
            std::cout << "FT (torque): " << robot->ft_foot_left().torque().transpose().format(fmt) << std::endl;
            std::cout << "=================================" << std::endl;
            // @ICUB_PRINT_FT_END@
        }
    }
    auto end = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsed_seconds = end - start;
    std::cout << "benchmark time: " << elapsed_seconds.count() << "s\n";

    return 0;
}
