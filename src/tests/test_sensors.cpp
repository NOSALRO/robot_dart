#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE test_robot

#include <boost/filesystem.hpp>
#include <boost/test/unit_test.hpp>

#include <dart/dynamics/BodyNode.hpp>
#include <dart/dynamics/Joint.hpp>

#include <robot_dart/robot.hpp>
#include <robot_dart/robot_dart_simu.hpp>
#include <robot_dart/sensor/force_torque.hpp>
#include <robot_dart/sensor/imu.hpp>
#include <robot_dart/sensor/torque.hpp>
#include <robot_dart/utils.hpp>

using namespace robot_dart;

BOOST_AUTO_TEST_CASE(test_imu)
{
    // Tests for linear acceleration
    {
        // Load robot & setup sim
        Eigen::Vector6d pose = Eigen::Vector6d::Zero();
        pose(5) += 1.5;
        // random size
        Eigen::Vector3d size = Eigen::Vector3d(0.1, 0.1, 0.1);

        auto robot = robot_dart::Robot::create_box(size, pose, "free", 1.);

        double g = 9.81;
        robot_dart::RobotDARTSimu simu(0.001);
        simu.set_gravity(Eigen::Vector3d(0., 0., -g));

        // add floor of square size of 10 meters and height of 0.2 meters
        simu.add_floor(10., 0.2);

        simu.add_robot(robot);

        // Add an IMU sensor
        robot_dart::sensor::IMUConfig imu_config;
        imu_config.body = robot->body_node("box");
        imu_config.frequency = 1000; // big update rate to get result in one time-step
        auto imu_sensor = simu.add_sensor<robot_dart::sensor::IMU>(imu_config);

        // Test IMU zero linear acceleration on free-fall
        // Do one step
        simu.step_world();

        // Get IMU readings
        Eigen::Vector3d linear_accel = imu_sensor->linear_acceleration();
        Eigen::Vector3d ang_vel = imu_sensor->angular_velocity();

        for (int i = 0; i < 3; i++) {
            BOOST_CHECK_SMALL(std::abs(linear_accel[i]), 1e-10);
            BOOST_CHECK_SMALL(std::abs(ang_vel[i]), 1e-10);
        }

        // Iterate until box hits the ground
        for (int i = 0; i < 1000; i++)
            simu.step_world();

        linear_accel = imu_sensor->linear_acceleration();
        ang_vel = imu_sensor->angular_velocity();

        // linear acceleration in z-axis should be positive gravity
        for (int i = 0; i < 3; i++) {
            double val = 0.;
            if (i == 2)
                val = g;
            BOOST_CHECK_SMALL(std::abs(linear_accel[i] - val), 1e-10);
            BOOST_CHECK_SMALL(std::abs(ang_vel[i]), 1e-10);
        }
    }

    // Test for angular velocity?
}

BOOST_AUTO_TEST_CASE(test_force_torque)
{
    // clang-format off
    // Create a simplistic model to test force/torque sensor
    std::string urdf_robot = 
    "<?xml version=\"1.0\" ?>"
    "<robot name=\"test_robot\" xmlns:xacro=\"http://ros.org/wiki/xacro\">"
    "<link name=\"base_link\">"
    "    <visual>"
    "    <origin rpy=\"0 0 0\" xyz=\"0 0 0.05715\"/>"
    "    <geometry>"
    "        <box size=\"0.1143 0.0545 0.1143\"/>"
    "    </geometry>"
    "    </visual>"
    "    <inertial>"
    "    <!-- CENTER OF MASS -->"
    "    <origin rpy=\"0 0 0\" xyz=\"0 0 0.05715\"/>"
    "    <mass value=\"0.2\"/>"
    "    <!-- box inertia: 1/12*m(y^2+z^2), ... -->"
    "    <inertia ixx=\"0.000267245666667\" ixy=\"0\" ixz=\"0\" iyy=\"0.000435483\" iyz=\"0\" izz=\"0.000267245666667\"/>"
    "    </inertial>"
    "</link>"
    "<joint name=\"joint_1\" type=\"revolute\">"
    "    <parent link=\"base_link\"/>"
    "    <child link=\"link_1\"/>"
    "    <limit effort=\"2.5\" velocity=\"10.0\" lower=\"-1.57079632679\" upper=\"1.57079632679\"/>"
    "    <origin rpy=\"0 0 0\" xyz=\"0 0.05225 0.05715\"/>"
    "    <axis xyz=\"0 1 0\"/>"
    "</joint>"
    "<link name=\"link_1\">"
    "    <visual>"
    "    <origin rpy=\"0 0 0\" xyz=\"0 0 1\"/>"
    "    <geometry>"
    "        <box size=\"0.1 0.1 0.1\"/>"
    "    </geometry>"
    "    </visual>"
    "    <inertial>"
    "    <!-- CENTER OF MASS -->"
    "    <origin rpy=\"0 0 0\" xyz=\"0 0 1\"/>"
    "    <mass value=\"1\"/>"
    "    <!-- box inertia: 1/12*m(y^2+z^2), ... -->"
    "    <inertia ixx=\"0.00083333333\" ixy=\"0\" ixz=\"0\" iyy=\"0.00083333333\" iyz=\"0\" izz=\"0.00083333333\"/>"
    "    </inertial>"
    "</link>"
    "</robot>";
    // clang-format on
    // FT sensor
    {
        // Load robot & setup sim
        auto robot = std::make_shared<robot_dart::Robot>(urdf_robot, "test_robot", true);
        robot->fix_to_world();

        robot_dart::RobotDARTSimu simu(0.001);
        simu.add_robot(robot);

        // Put the robot in unstable position to hit the limits with gravity
        Eigen::VectorXd pos(1);
        pos << M_PI / 3.;
        robot->set_positions(pos);

        // Add a force/torque sensor
        auto ft_sensor = simu.add_sensor<robot_dart::sensor::ForceTorque>(robot, "joint_1");

        // Do a few steps to hit the limit
        for (int i = 0; i < 5000; i++)
            simu.step_world();

        // force and torque magnitude should be equal to gravity magnitude
        BOOST_CHECK_SMALL(std::abs(ft_sensor->force().norm() - simu.gravity().norm()), 1e-6);
        BOOST_CHECK_SMALL(std::abs(ft_sensor->torque().norm() - simu.gravity().norm()), 1e-6);

        // force (abs) x-axis should be very close to gravity magnitude
        BOOST_CHECK_SMALL(std::abs(ft_sensor->force()[0]) - simu.gravity().norm(), 1e-4);
        // torque (abs) y-axis should be very close to gravity magnitude
        BOOST_CHECK_SMALL(std::abs(ft_sensor->torque()[1]) - simu.gravity().norm(), 1e-6);
    }

    // torque sensor
    {
        // Load robot & setup sim
        auto robot = std::make_shared<robot_dart::Robot>(urdf_robot, "test_robot", true);
        robot->fix_to_world();
        robot->set_position_enforced(false); // Do not enforce limits
        robot->set_damping_coeffs(0.); // no damping
        robot->set_cfriction_coeffs(0.); // no coulomb friction

        robot_dart::RobotDARTSimu simu(0.001);
        simu.add_robot(robot);

        // Put the robot in unstable position to hit the limits with gravity
        Eigen::VectorXd pos(1);
        pos << M_PI / 3.;
        robot->set_positions(pos);

        // Add a torque sensor
        auto torque_sensor = simu.add_sensor<robot_dart::sensor::Torque>(&simu, robot, "joint_1");

        Eigen::VectorXd cmd(1);
        cmd << 1.;

        // Do a few steps of simulation
        for (int i = 0; i < 50; i++) {
            robot->set_commands(cmd);
            simu.step_world();
        }

        BOOST_CHECK_SMALL(std::abs(cmd(0) - torque_sensor->torques().norm()), 1e-6);
    }
}
