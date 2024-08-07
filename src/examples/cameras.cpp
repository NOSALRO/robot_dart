#include <robot_dart/control/pd_control.hpp>
#include <robot_dart/gui/magnum/graphics.hpp>
#include <robot_dart/gui/magnum/sensor/camera.hpp>
#include <robot_dart/robot_dart_simu.hpp>
#include <robot_dart/robots/iiwa.hpp>

class MyApp : public robot_dart::gui::magnum::GlfwApplication {
public:
    explicit MyApp(int argc, char** argv, robot_dart::RobotDARTSimu* simu, const robot_dart::gui::magnum::GraphicsConfiguration& configuration = robot_dart::gui::magnum::GraphicsConfiguration())
        : GlfwApplication(argc, argv, simu, configuration)
    {
        // we synchronize by default if we have the graphics activated
        simu->scheduler().set_sync(true);
        // enable summary text when graphics activated
        simu->enable_text_panel(true);
        simu->enable_status_bar(true);
    }

protected:
    void keyPressEvent(KeyEvent& event) override
    {
        // One can customize their key/mouse events by inheriting GlfwApplication
        // if (event.key() == KeyEvent::Key::Right) {
        // }

        robot_dart::gui::magnum::GlfwApplication::keyPressEvent(event);
    }
};

int main()
{
    auto robot = std::make_shared<robot_dart::robots::Iiwa>();

    Eigen::VectorXd ctrl = robot_dart::make_vector({-0.3, M_PI / 3., 0.2, -M_PI / 4., 0., 0., -0.6});

    auto controller = std::make_shared<robot_dart::control::PDControl>(ctrl);
    robot->add_controller(controller);
    controller->set_pd(500., 50.);

    robot_dart::RobotDARTSimu simu(0.001);

    // Magnum graphics
    robot_dart::gui::magnum::GraphicsConfiguration configuration = robot_dart::gui::magnum::Graphics::default_configuration();
    configuration.width = 1024;
    configuration.height = 768;
    auto graphics = std::make_shared<robot_dart::gui::magnum::BaseGraphics<MyApp>>(configuration);
    simu.set_graphics(graphics);

    // set the position of the main camera, and the position where the main camera is looking at
    graphics->look_at({0., 3.5, 2.}, {0., 0., 0.25});
    // record images from main camera/graphics
    graphics->camera().record(true);
    // we can also record a video directly to a file --- requires the executable of ffmpeg
    graphics->record_video("video-main.mp4", simu.graphics_freq());
    // @ADD_NEW_CAMERA@
    // Add camera
    auto camera = std::make_shared<robot_dart::sensor::Camera>(graphics->magnum_app(), 256, 256);
    // @ADD_NEW_CAMERA_END@

    // @MANIPULATE_CAM_SEP@
    camera->camera().set_far_plane(5.f);
    camera->camera().set_near_plane(0.01f);
    camera->camera().set_fov(60.0f);
    // @MANIPULATE_CAM_SEP_END@

    // @MANIPULATE_CAM@
    camera->camera().set_camera_params(5., // far plane
        0.01f, // near plane
        60.0f, // field of view
        600, // width
        400 // height
    );
    // @MANIPULATE_CAM_END@

    camera->camera().record(true, true); // cameras are recording color images by default, enable depth images as well for this example
    // @RECORD_VIDEO_CAMERA@
    // cameras can also record video
    camera->record_video("video-camera.mp4");
    // @RECORD_VIDEO_CAMERA_END@

    // @CAM_POSITION@
    // set the position of the camera, and the position where the main camera is looking at
    Eigen::Vector3d cam_pos = {-0.5, -3., 0.75};
    Eigen::Vector3d cam_looks_at = {0.5, 0., 0.2};
    camera->look_at(cam_pos, cam_looks_at);
    // @CAM_POSITION_END@

    // @CAM_ATTACH@
    Eigen::Isometry3d tf;
    tf = Eigen::AngleAxisd(3.14, Eigen::Vector3d{1., 0., 0.});
    tf *= Eigen::AngleAxisd(1.57, Eigen::Vector3d{0., 0., 1.});
    tf.translation() = Eigen::Vector3d(0., 0., 0.1);
    camera->attach_to_body(robot->body_node("iiwa_link_ee"), tf); // cameras are looking towards -z by default
    // @CAM_ATTACH_END@
    // the default checkerboard floor values are the following:
    // double floor_width = 10.0
    // double floor_height = 0.1
    // double size = 1.
    // const Eigen::Isometry3d& tf = Eigen::Isometry3d::Identity()
    // const std::string& floor_name = "checkerboard_floor"
    // const Eigen::Vector4d& first_color = dart::Color::White(1.)
    // const Eigen::Vector4d& second_color = dart::Color::Gray(1.))
    simu.add_checkerboard_floor();
    simu.add_robot(robot);
    Eigen::Vector6d pose;
    pose << 0., 0., 0., 1.5, 0., 0.25;
    simu.add_robot(robot_dart::Robot::create_box({0.1, 0.1, 0.5}, pose, "free", 1., dart::Color::Red(1.), "box"));
    pose << 0., 0., 0., 1.5, -0.5, 0.25;
    simu.add_robot(robot_dart::Robot::create_ellipsoid({0.2, 0.2, 0.2}, pose, "free", 1., dart::Color::Red(1.), "sphere"));
    simu.add_sensor(camera);

    simu.run(10.);

    // a nested std::vector (w*h*3) of the last image taken can be retrieved
    auto gimage = graphics->image();
    auto cimage = camera->image();

    // we can also save them to png
    robot_dart::gui::save_png_image("camera-small.png", cimage);
    robot_dart::gui::save_png_image("camera-main.png", gimage);

    // convert an rgb image to grayscale (useful in some cases)
    auto gray_image = robot_dart::gui::convert_rgb_to_grayscale(gimage);
    robot_dart::gui::save_png_image("camera-gray.png", gray_image);

    // get the depth image from a camera
    // with a version for visualization or bigger differences in the output
    robot_dart::gui::save_png_image("camera-depth.png", camera->depth_image());
    // and the raw values that can be used along with the camera parameters to transform the image to point-cloud
    robot_dart::gui::save_png_image("camera-depth-raw.png", camera->raw_depth_image());

    auto depth_image = camera->depth_array();

    auto small_box = robot_dart::Robot::create_box({0.01, 0.01, 0.01}, Eigen::Vector6d::Zero(), "fixed", 1., dart::Color::Blue(1.), "box_pc");
    std::vector<Eigen::Vector3d> point_cloud = robot_dart::gui::point_cloud_from_depth_array(depth_image, camera->camera_intrinsic_matrix(), camera->camera_extrinsic_matrix(), camera->camera().far_plane());
    for (size_t i = 0; i < point_cloud.size(); i++) {
        if (i % 30 == 0) { // do not display all the points in the cloud
            Eigen::Vector6d pose;
            pose << 0., 0., 0., point_cloud[i];
            auto bbox = small_box->clone_ghost("box_" + std::to_string(i), dart::Color::Blue(1.));
            bbox->set_base_pose(pose);
            bbox->set_cast_shadows(false);
            simu.add_robot(bbox);
        }
    }

    simu.set_graphics_freq(20);
    simu.world()->setTime(0.);
    simu.scheduler().reset(simu.timestep(), true);
    while (true) {
        if (simu.step())
            break;
        if (simu.schedule(simu.graphics_freq())) {
            auto depth_image = camera->depth_array();
            std::vector<Eigen::Vector3d> point_cloud = robot_dart::gui::point_cloud_from_depth_array(depth_image, camera->camera_intrinsic_matrix(), camera->camera_extrinsic_matrix(), camera->camera().far_plane());
            std::cout << simu.scheduler().current_time() << ": " << point_cloud.size() << std::endl;
        }
    }

    robot.reset();
    return 0;
}
