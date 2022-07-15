#include <robot_dart/robot_dart_simu.hpp>
#include <robot_dart/robots/a1.hpp>

#ifdef GRAPHIC
#include <robot_dart/gui/magnum/graphics.hpp>
#endif

int main()
{
    // @A1@
    auto robot = std::make_shared<robot_dart::robots::A1>();
    // @A1_END@
    robot->set_actuator_types("servo");

    robot_dart::RobotDARTSimu simu(0.001);
    simu.set_collision_detector("fcl");
#ifdef GRAPHIC
    auto graphics = std::make_shared<robot_dart::gui::magnum::Graphics>();
    simu.set_graphics(graphics);

    graphics->look_at({0., 3.5, 2.}, {0., 0., 0.25});
#endif
    simu.add_checkerboard_floor();
    simu.add_robot(robot);
    Eigen::IOFormat fmt(Eigen::StreamPrecision, Eigen::DontAlignCols, " ", "\n", "", "");
    std::cout.precision(4);


    
    while (simu.scheduler().next_time() < 20 && !simu.graphics()->done()) {
        simu.step_world();
        // Print IMU measurements
        if (simu.schedule(robot->imu().frequency())) {
            // @A1_PRINT_IMU@
            std::cout << "Angular    Position: " << robot->imu().angular_position_vec().transpose().format(fmt) << std::endl;
            std::cout << "Angular    Velocity: " << robot->imu().angular_velocity().transpose().format(fmt) << std::endl;
            std::cout << "Linear Acceleration: " << robot->imu().linear_acceleration().transpose().format(fmt) << std::endl;
            std::cout << "=================================" << std::endl;
            // @A1_PRINT_IMU_END@
        }
    }
    robot.reset();
    return 0;
}