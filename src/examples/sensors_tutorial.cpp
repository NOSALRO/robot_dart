#include <robot_dart/control/pd_control.hpp>
#include <robot_dart/robot_dart_simu.hpp>
#include <robot_dart/robots/iiwa.hpp>

#include <robot_dart/gui/magnum/graphics.hpp>
#include <robot_dart/gui/magnum/sensor/camera.hpp>
#include <robot_dart/sensor/force_torque.hpp>
#include <robot_dart/sensor/imu.hpp>
#include <robot_dart/sensor/torque.hpp>

int main()
{
    auto robot = std::make_shared<robot_dart::robots::Iiwa>();
    robot->set_actuator_types("torque");

    Eigen::VectorXd ctrl = robot_dart::make_vector({0., M_PI / 3., 0., -M_PI / 4., 0., 0., 0.});

    auto controller = std::make_shared<robot_dart::control::PDControl>(ctrl);
    robot->add_controller(controller);
    controller->set_pd(300., 50.);

    robot_dart::RobotDARTSimu simu(0.001);

    auto graphics = std::make_shared<robot_dart::gui::magnum::Graphics>();
    simu.set_graphics(graphics);

    graphics->look_at({0., 3.5, 2.}, {0., 0., 0.25});

    simu.add_robot(robot);
    simu.add_checkerboard_floor();
    Eigen::Vector6d pose;
    pose << 0., 0., 0., 1.5, 0., 0.25;

    simu.add_robot(robot_dart::Robot::create_box({0.1, 0.1, 0.5}, pose, "free", 1., dart::Color::Red(1.), "box"));
    // Format Eigen to std::cout
    Eigen::IOFormat fmt(Eigen::StreamPrecision, Eigen::DontAlignCols, " ", "\n", "", "");
    std::cout.precision(5);

    robot->set_damping_coeffs(0.);
    robot->set_coulomb_coeffs(0.);
    robot->set_spring_stiffnesses(0.);

    auto dampings = robot->damping_coeffs();
    auto frictions = robot->coulomb_coeffs();
    auto stiffnesses = robot->spring_stiffnesses();

    for (size_t i = 0; i < dampings.size(); ++i) {
        std::cout << "DoF #" << i << std::endl;
        std::cout << "    Stiffness: " << stiffnesses[i] << std::endl;
        std::cout << "    Friction: " << frictions[i] << std::endl;
        std::cout << "    Damping: " << dampings[i] << std::endl;
    }
    auto camera = std::make_shared<robot_dart::sensor::Camera>(graphics->magnum_app(), 256, 256);

    camera->camera().record(true, true); // cameras are recording color images by default, enable depth images as well for this example
    // cameras can also record video
    camera->record_video("video-camera.mp4");

    // attach camera to body
    Eigen::Isometry3d tf;
    tf = Eigen::AngleAxisd(3.14, Eigen::Vector3d{1., 0., 0.});
    tf *= Eigen::AngleAxisd(1.57, Eigen::Vector3d{0., 0., 1.});
    tf.translation() = Eigen::Vector3d(0., 0., 0.1);
    camera->attach_to_body(robot->body_node("iiwa_link_ee"), tf); // cameras are looking towards -z by default

    // @TORQUE_SENSOR@
    // Add torque sensors to the robot
    int ct = 0;
    std::vector<std::shared_ptr<robot_dart::sensor::Torque>> tq_sensors(robot->num_dofs());
    for (const auto& joint : robot->dof_names())
        tq_sensors[ct++] = simu.add_sensor<robot_dart::sensor::Torque>(robot, joint, 1000);
    // @TORQUE_SENSOR_END@

    // @FORCE_TORQUE_SENSOR@
    // Add force-torque sensors to the robot
    ct = 0;
    std::vector<std::shared_ptr<robot_dart::sensor::ForceTorque>> f_tq_sensors(robot->num_dofs());
    for (const auto& joint : robot->dof_names())
        f_tq_sensors[ct++] = simu.add_sensor<robot_dart::sensor::ForceTorque>(robot, joint, 1000, "parent_to_child");
    // @TORQUE_FORCE_SENSOR_END@

    // @IMU_SENSOR@
    // Add IMU sensors to the robot
    ct = 0;
    std::vector<std::shared_ptr<robot_dart::sensor::IMU>> imu_sensors(robot->num_bodies());
    for (const auto& body_node : robot->body_names()) {
        // _imu(std::make_shared<sensor::IMU>(sensor::IMUConfig(body_node("head"), frequency))),
        imu_sensors[ct++] = simu.add_sensor<robot_dart::sensor::IMU>(robot_dart::sensor::IMUConfig(robot->body_node(body_node), 1000));
    }
    // @IMU_SENSOR_END@

    // Cameras can be used as sensors to
    simu.add_sensor(camera);

    auto start = std::chrono::steady_clock::now();
    Eigen::Vector3d external_force = Eigen::Vector3d::Zero();

    while (simu.scheduler().next_time() < 20 && !simu.graphics()->done()) {
        simu.step();

        // Print torque sensor measurement
        if (simu.schedule(tq_sensors[0]->frequency())) {
            ct = 0;
            // @TORQUE_MEASUREMENT@
            // vector that contains the torque measurement for every joint (scalar)
            Eigen::VectorXd torques_measure(robot->num_dofs());
            for (const auto& tq_sens : tq_sensors)
                torques_measure.block<1, 1>(ct++, 0) = tq_sens->torques();
            // @TORQUE_MEASUREMENT_END@
            // @FORCE_TORQUE_MEASUREMENT@
            //  matrix that contains the torque measurement for every joint (3d vector)
            Eigen::MatrixXd ft_torques_measure(robot->num_dofs(), 3);
            //  matrix that contains the force measurement for every joint (3d vector)
            Eigen::MatrixXd ft_forces_measure(robot->num_dofs(), 3);
            //  matrix that contains the wrench measurement for every joint (6d vector)[torque, force]
            Eigen::MatrixXd ft_wrench_measure(robot->num_dofs(), 6);
            ct = 0;
            for (const auto& f_tq_sens : f_tq_sensors) {
                ft_torques_measure.block<1, 3>(ct, 0) = f_tq_sens->torque();
                ft_forces_measure.block<1, 3>(ct, 0) = f_tq_sens->force();
                ft_wrench_measure.block<1, 6>(ct, 0) = f_tq_sens->wrench();
                ct++;
            }
            // @FORCE_TORQUE_MEASUREMENT_END@

            // @IMU_MEASUREMENT@
            Eigen::MatrixXd imu_angular_positions_measure(robot->num_bodies(), 3);
            Eigen::MatrixXd imu_angular_velocities_measure(robot->num_bodies(), 3);
            Eigen::MatrixXd imu_linear_acceleration_measure(robot->num_bodies(), 3);
            ct = 0;
            for (const auto& imu_sens : imu_sensors) {
                imu_angular_positions_measure.block<1, 3>(ct, 0) = imu_sens->angular_position_vec();
                imu_angular_velocities_measure.block<1, 3>(ct, 0) = imu_sens->angular_velocity();
                imu_linear_acceleration_measure.block<1, 3>(ct, 0) = imu_sens->linear_acceleration();
                ct++;
            }
            // @IMU_MEASUREMENT_END@

            std::cout
                << " Torque sensors' torque:  \n"
                << torques_measure.transpose().format(fmt) << std::endl;
            std::cout << " Force-Torque sensors' torque:  \n"
                      << ft_torques_measure.transpose().format(fmt) << std::endl;
            std::cout << " Force-Torque sensors' force:  \n"
                      << ft_forces_measure.format(fmt) << std::endl;
            std::cout << " Force-Torque sensors' wrench:  \n"
                      << ft_wrench_measure.format(fmt) << std::endl;
            std::cout << "IMU sensors Angular Position: \n"
                      << imu_angular_positions_measure.format(fmt) << std::endl;
            std::cout << "IMU sensors Angular Velocity: \n"
                      << imu_angular_positions_measure.format(fmt) << std::endl;
            std::cout << "IMU sensors Linear Acceleration: \n"
                      << imu_angular_positions_measure.format(fmt) << std::endl;
            std::cout << "=================================" << std::endl;
        }

        // add external force
        int time_in_seconds = static_cast<int>(simu.scheduler().next_time());
        if (time_in_seconds % 3 >= 1 && time_in_seconds % 3 < 2) {
            external_force = Eigen::Vector3d::Constant(30.0);
            //std::cout << "Applying force on iiwa_link_4" << std::endl;
        }
        else {

            external_force = Eigen::Vector3d::Zero();
        }

        robot->set_external_force("iiwa_link_4", external_force);
    }
    // @RGB_SENSOR@
    // a nested std::vector (w*h*3) of the last image taken can be retrieved
    auto rgb_image = camera->image();
    // @RGB_SENSOR_END@
    // @RGB_SENSOR_MEASURE@
    // we can also save them to png
    robot_dart::gui::save_png_image("camera-small.png", rgb_image);
    // convert an rgb image to grayscale (useful in some cases)
    auto gray_image = robot_dart::gui::convert_rgb_to_grayscale(rgb_image);
    robot_dart::gui::save_png_image("camera-gray.png", gray_image);
    // @RGB_SENSOR_MEASURE_END@

    // @RGB_D_SENSOR@
    auto rgb_d_image = camera->depth_image();
    // @RGB_D_SENSOR_END@
    // @RGB_D_SENSOR_MEASURE@
    // get the depth image from a camera
    // with a version for visualization or bigger differences in the output
    robot_dart::gui::save_png_image("camera-depth.png", rgb_d_image);
    // and the raw values that can be used along with the camera parameters to transform the image to point-cloud
    robot_dart::gui::save_png_image("camera-depth-raw.png", rgb_d_image);
    // @RGB_D_SENSOR_MEASURE_END@
    auto end = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsed_seconds = end - start;
    std::cout << "benchmark time: " << elapsed_seconds.count() << "s\n";

    robot.reset();
    return 0;
}