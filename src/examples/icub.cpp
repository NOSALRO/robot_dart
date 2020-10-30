#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <robot_dart/control/pd_control.hpp>
#include <robot_dart/robot_dart_simu.hpp>

#include <dart/collision/fcl/FCLCollisionDetector.hpp>
#include <dart/constraint/ConstraintSolver.hpp>

#include <robot_dart/sensor/force_torque.hpp>
#include <robot_dart/sensor/imu.hpp>

#ifdef GRAPHIC
#include <robot_dart/gui/magnum/graphics.hpp>
#endif

int main()
{
    std::srand(std::time(NULL));

    std::vector<std::pair<std::string, std::string>> packages = {{"icub_description", "icub/icub_description"}};
    auto robot = std::make_shared<robot_dart::Robot>("icub/icub.urdf", packages);

    robot->set_color_mode("material");
    robot->set_color_mode("aspect", "right_foot");
    robot->set_color_mode("aspect", "left_foot");

    robot->set_position_enforced(true);
    Eigen::Vector6d pose;
    pose << 0., 0., M_PI / 2., 0., 0., 0.5;
    robot->set_base_pose(pose);

    // Set actuator types to VELOCITY motors so that they stay in position without any controller
    robot->set_actuator_types("velocity");

    robot_dart::RobotDARTSimu simu(0.001);
    simu.set_collision_detector("fcl");
#ifdef GRAPHIC
    auto graphics = std::make_shared<robot_dart::gui::magnum::Graphics>();
    simu.set_graphics(graphics);
    graphics->look_at({0., 3.5, 2.}, {0., 0., 0.25});
    //    graphics->record_video("icub.mp4");
#endif
    simu.add_checkerboard_floor();
    simu.add_robot(robot);

    // Add an IMU sensor to the "chest" body link
    robot_dart::sensor::IMUConfig imu_config;
    imu_config.body = robot->body_node("chest"); // choose which body the sensor is attached to
    imu_config.frequency = 200; // update rate of the sensor
    auto imu_sensor = simu.add_sensor<robot_dart::sensor::IMU>(imu_config);

    // Add a force/torque sensor in "r_ankle_roll" joint
    auto ft_sensor = simu.add_sensor<robot_dart::sensor::ForceTorque>(robot, "r_ankle_roll");

    // Add some visualizations
    robot->set_draw_axis(imu_config.body->getName());
    robot->set_draw_axis("r_ankle_2");

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
        if (simu.schedule(imu_sensor->frequency())) {
            std::cout << "Angular    Position: " << imu_sensor->angular_position_vec().transpose().format(fmt) << std::endl;
            std::cout << "Angular    Velocity: " << imu_sensor->angular_velocity().transpose().format(fmt) << std::endl;
            std::cout << "Linear Acceleration: " << imu_sensor->linear_acceleration().transpose().format(fmt) << std::endl;
            std::cout << "=================================" << std::endl;
        }

        // Print FT measurements
        if (simu.schedule(ft_sensor->frequency())) {
            std::cout << "FT ( force): " << ft_sensor->force().transpose().format(fmt) << std::endl;
            std::cout << "FT (torque): " << ft_sensor->torque().transpose().format(fmt) << std::endl;
            std::cout << "=================================" << std::endl;
        }
    }
    auto end = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsed_seconds = end - start;
    std::cout << "benchmark time: " << elapsed_seconds.count() << "s\n";

    robot.reset();
    return 0;
}
