#include <robot_dart/control/pd_control.hpp>
#include <robot_dart/robot_dart_simu.hpp>

#include <robot_dart/gui/magnum/graphics.hpp>
#include <robot_dart/gui/magnum/gs/light.hpp>
#include <robot_dart/gui/magnum/gs/material.hpp>

inline std::shared_ptr<robot_dart::Robot> random_box(size_t num = 0)
{
    // random pose
    Eigen::Vector6d pose = Eigen::Vector6d::Random();
    // make sure it is above the ground
    pose(5) += 0.5;
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
    pose(5) += 0.5;
    // random size
    Eigen::Vector3d size = Eigen::Vector3d::Random()[0] * Eigen::Vector3d(0.2, 0.2, 0.2).array() + 0.3;
    return robot_dart::Robot::create_ellipsoid(size, pose, "free", 1.,
        dart::Color::Blue(1.0),
        "sphere_" + std::to_string(num));
}

int main()
{
    std::srand(std::time(NULL));
    // @INIT_SIMU@
    // choose time step of 0.001 seconds
    robot_dart::RobotDARTSimu simu(0.001);
    // @INIT_SIMU_END@
    // @MODIFY_SIMU_DT@
    // set timestep to 0.005 and update control frequency(bool)
    simu.set_timestep(0.005, true);
    // @MODIFY_SIMU_DT_END@

    // @SIMU_GRAVITY@
    // Set gravitational force of mars
    Eigen::Vector3d mars_gravity = {0., 0., -3.721};
    simu.set_gravity(mars_gravity);
    // @SIMU_GRAVITY_END@
    // @GRAPHICS_PARAMS@
    robot_dart::gui::magnum::GraphicsConfiguration configuration;
    // We can change the width/height of the window (or camera image dimensions)
    configuration.width = 1280;
    configuration.height = 960;
    configuration.title = "Graphics Tutorial"; // We can set a title for our window

    // We can change the configuration for shadows
    configuration.shadowed = true;
    configuration.transparent_shadows = true;
    configuration.shadow_map_size = 1024;

    // We can also alter some specifications for the lighting
    configuration.max_lights = 3; // maximum number of lights for our scene [default=3]
    configuration.specular_strength = 0.25; // strength of the specular component

    // Some extra configuration for the main camera
    configuration.draw_main_camera = true;
    configuration.draw_debug = true;
    configuration.draw_text = true;

    // We can also change the background color [default=black]
    configuration.bg_color = Eigen::Vector4d{1.0, 1.0, 1.0, 1.0};

    // Create the graphics object with the configuration as parameter
    auto graphics = std::make_shared<robot_dart::gui::magnum::Graphics>(configuration);
    // @GRAPHICS_PARAMS_END@
    simu.set_graphics(graphics);
    // set the camera at position (0, 3, 1) looking at the center (0, 0, 0)
    graphics->look_at({0., 3., 1.}, {0., 0., 0.});

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
    simu.add_robot(arm_robot);

    // @SHADOWS_GRAPHICS@
    // Disable shadows
    graphics->enable_shadows(false, false);
    simu.run(1.);
    // Enable shadows only for non-transparent objects
    graphics->enable_shadows(true, false);
    simu.run(1.);
    // Enable shadows for transparent objects as well
    graphics->enable_shadows(true, true);
    simu.run(1.);
    // @SHADOWS_GRAPHICS_END@

    // @CLR_LIGHT@
    // Clear Lights
    graphics->clear_lights();
    // @CLR_LIGHT_END@

    simu.run(.2);

    // @LIGHT_MATERIAL@
    // Create Light material
    Magnum::Float shininess = 1000.f;
    Magnum::Color4 white = {1.f, 1.f, 1.f, 1.f};

    // ambient, diffuse, specular
    auto custom_material = robot_dart::gui::magnum::gs::Material(white, white, white, shininess);
    // @LIGHT_MATERIAL_END@

    {
        // @POINT_LIGHT@
        // Create point light
        Magnum::Vector3 position = {0.f, 0.f, 2.f};
        Magnum::Float intensity = 1.f;
        Magnum::Vector3 attenuation_terms = {1.f, 0.f, 0.f};
        auto point_light = robot_dart::gui::magnum::gs::create_point_light(position, custom_material, intensity, attenuation_terms);
        graphics->add_light(point_light);
        // @POINT_LIGHT_END@
    }

    simu.run(1.);
    graphics->clear_lights();
    simu.run(.2);

    {
        // @DIRECTIONAL_LIGHT@
        // Create directional light
        Magnum::Vector3 direction = {-1.f, -1.f, -1.f};
        auto directional_light = robot_dart::gui::magnum::gs::create_directional_light(direction, custom_material);
        graphics->add_light(directional_light);
        // @DIRECTIONAL_LIGHT_END@
    }

    simu.run(1.);
    graphics->clear_lights();
    simu.run(.2);

    {
        // @SPOT_LIGHT@
        // Create spot light
        Magnum::Vector3 position = {0.f, 0.f, 1.f};
        Magnum::Vector3 direction = {-1.f, -1.f, -1.f};
        Magnum::Float intensity = 1.f;
        Magnum::Vector3 attenuation_terms = {1.f, 0.f, 0.f};
        Magnum::Float spot_exponent = M_PI;
        Magnum::Float spot_cut_off = M_PI / 8;
        auto spot_light = robot_dart::gui::magnum::gs::create_spot_light(position, custom_material, direction, spot_exponent, spot_cut_off, intensity, attenuation_terms);
        graphics->add_light(spot_light);
        // @SPOT_LIGHT_END@
    }

    simu.run(1.);

    return 0;
}
