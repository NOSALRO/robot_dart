#include <iostream>
#include <robot_dart/robot_dart_simu.hpp>

#include <robot_dart/control/hexa_control.hpp>
#include <robot_dart/control/pd_control.hpp>

#include <robot_dart/gui/helper.hpp>
#include <robot_dart/gui/magnum/camera_osr.hpp>
#include <robot_dart/gui/magnum/graphics.hpp>
#include <robot_dart/gui/magnum/gs/helper.hpp>

class MyApp : public robot_dart::gui::magnum::Sdl2Application {
public:
    explicit MyApp(int argc, char** argv, const dart::simulation::WorldPtr& world, size_t width, size_t height, const std::string& title = "DART") : Sdl2Application(argc, argv, world, width, height, title) {}

protected:
    void keyPressEvent(KeyEvent& event) override
    {
        // One can customize their key/mouse events by inheriting Sdl2Application
        // if (event.key() == KeyEvent::Key::Right) {
        // }

        robot_dart::gui::magnum::Sdl2Application::keyPressEvent(event);
    }
};

int main()
{
    CORRADE_RESOURCE_INITIALIZE(RobotDARTShaders);
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

    robot_dart::RobotDARTSimu simu(0.001);

    // Magnum graphics
    auto graphics = std::make_shared<robot_dart::gui::magnum::Graphics<MyApp>>(simu.world(), 1024, 768);
    simu.set_graphics(graphics);
    graphics->look_at({0., 3.5, 2.}, {0., 0., 0.25});

    // record images from main camera/graphics
    graphics->set_recording(true);

    // Add camera
    auto camera = std::make_shared<robot_dart::gui::magnum::CameraOSR>(simu.world(), graphics->magnum_app(), 256, 256);
    camera->look_at({-0.5, 3., -0.75}, {0.5, 0., 0.2});
    simu.add_camera(camera); // cameras are recording by default

    // simu.world()->getConstraintSolver()->setCollisionDetector(dart::collision::BulletCollisionDetector::create());
    // simu.add_floor();
    simu.add_robot(global_robot);
    simu.run(6);

    // a pointer to the last image taken cam be found
    auto gimage = graphics->image();
    auto cimage = camera->image();

    // we can also get a nested std::vector (w*h*3) from the images
    // and save them to png
    robot_dart::gui::save_png_image("camera-small.png", robot_dart::gui::magnum::gs::rgb_from_image(cimage));
    robot_dart::gui::save_png_image("camera-main.png", robot_dart::gui::magnum::gs::rgb_from_image(gimage));

    global_robot.reset();
    return 0;
}