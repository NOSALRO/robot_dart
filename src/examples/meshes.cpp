#include <algorithm>
#include <cstdlib>
#include <iostream>

#include <robot_dart/control/pd_control.hpp>
#include <robot_dart/robot_dart_simu.hpp>

#ifdef GRAPHIC
#include <robot_dart/gui/magnum/graphics.hpp>
#endif

int main()
{
    std::srand(std::time(NULL));

    std::vector<std::pair<std::string, std::string>> packages = {{"iiwa14", std::string(RESPATH) + "/models/meshes"}};
    auto global_robot = std::make_shared<robot_dart::Robot>("res/models/iiwa14.urdf", packages);

    global_robot->fix_to_world();
    global_robot->set_position_enforced(true);
    // Use the material information coming from the meshes instead of the URDF file
    // global_robot->set_color_mode(dart::dynamics::MeshShape::ColorMode::MATERIAL_COLOR);
    // Use the material information from the meshes only for a specific BodyNode
    // global_robot->set_color_mode(dart::dynamics::MeshShape::ColorMode::MATERIAL_COLOR, "iiwa_link_6");

    std::vector<double> ctrl;
    ctrl = {0., M_PI / 3., 0., -M_PI / 4., 0., 0., 0.};

    global_robot->add_controller(std::make_shared<robot_dart::control::PDControl>(ctrl));
    std::static_pointer_cast<robot_dart::control::PDControl>(global_robot->controllers()[0])->set_pd(300., 50.);

    // Add a ghost robot; only visuals, no dynamics, no collision
    auto ghost = global_robot->clone_ghost();

    robot_dart::RobotDARTSimu simu(0.001);
    simu.set_collision_detector("fcl");
#ifdef GRAPHIC
    auto graphics = std::make_shared<robot_dart::gui::magnum::Graphics>(&simu);
    simu.set_graphics(graphics);

    graphics->look_at({0., 3.5, 2.}, {0., 0., 0.25});
#endif
    simu.add_checkerboard_floor();
    simu.add_robot(global_robot);
    simu.add_robot(ghost);
    simu.run(20.);

    global_robot.reset();
    return 0;
}