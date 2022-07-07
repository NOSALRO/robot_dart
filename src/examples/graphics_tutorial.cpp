#include <robot_dart/robot_dart_simu.hpp>

#include <robot_dart/control/pd_control.hpp>

#ifdef GRAPHIC
#include <robot_dart/gui/magnum/graphics.hpp>
#endif

inline std::shared_ptr<robot_dart::Robot> random_box(size_t num = 0)
{
    // random pose
    Eigen::Vector6d pose = Eigen::Vector6d::Random();
    // make sure it is above the ground
    pose(5) += 1.5;
    // random size
    Eigen::Vector3d size = Eigen::Vector3d::Random().array() * Eigen::Vector3d(0.1, 0.2, 0.1).array() + 0.3;
    return robot_dart::Robot::create_box(size, pose, "free", 1.,
        dart::Color::Red(1.0),
        "box_" + std::to_string(num));
}

inline std::shared_ptr<robot_dart::Robot> random_sphere(size_t num = 0)
{
    // random pose
    Eigen::Vector6d pose = Eigen::Vector6d::Random();
    // make sure it is above the ground
    pose(5) += 1.5;
    // random size
    Eigen::Vector3d size = Eigen::Vector3d::Random()[0] * Eigen::Vector3d(0.2, 0.2, 0.2).array() + 0.3;
    return robot_dart::Robot::create_ellipsoid(size, pose, "free", 1.,
        dart::Color::Blue(1.0),
        "sphere_" + std::to_string(num));
}

int main()
{
    std::srand(std::time(NULL));
    // choose time step of 0.001 seconds
    robot_dart::RobotDARTSimu simu(0.001);
#ifdef GRAPHIC
    auto graphics = std::make_shared<robot_dart::gui::magnum::Graphics>();
    simu.set_graphics(graphics);
    // set the camera at position (0, 3, 1) looking at the center (0, 0, 0)
    graphics->look_at({0., 3., 1.}, {0., 0., 0.});
#endif

    // add floor of square size of 10 meters and height of 0.2 meters
    simu.add_floor(10., 0.2);

    // add random objects to the world
    simu.add_robot(random_box(1));
    simu.add_robot(random_sphere(1));

    // add a simple arm
    auto arm_robot = std::make_shared<robot_dart::Robot>("arm.urdf");

    // pin the arm to world
    arm_robot->fix_to_world();
    arm_robot->set_position_enforced(true);


#ifdef GRAPHIC
    // @SHADOWS_GRAPHICS@
    // Disable shadows
    graphics->enable_shadows(false, false);
    simu.run(1.);
    // Enable non-transparent shadows
    graphics->enable_shadows(true, false);
    simu.run(1.);
    // Enable transparent shadows
    graphics->enable_shadows(true, true);
    simu.run(1.);
    // @SHADOWS_GRAPHICS_END@
#endif
    return 0;
}