#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <robot_dart/control/pd_control.hpp>
#include <robot_dart/robot_dart_simu.hpp>

#ifdef GRAPHIC
#include <robot_dart/graphics/camera_osr.hpp>
#include <robot_dart/graphics/graphics.hpp>
#endif

struct StateDesc : public robot_dart::descriptor::BaseDescriptor {
    StateDesc(robot_dart::RobotDARTSimu& simu, size_t desc_dump = 1) : robot_dart::descriptor::BaseDescriptor(simu, desc_dump) {}

    void operator()()
    {
        if (_simu.robots().size() > 0) {
            states.push_back(_simu.robots()[0]->skeleton()->getPositions());

            auto cam = std::static_pointer_cast<robot_dart::graphics::CameraOSR>(_simu.cameras()[0]);
            cam->set_filename("cam_" + std::to_string(step) + ".png");
        }

        step++;
    }

    std::vector<Eigen::VectorXd> states;
    int step = 0;
};

int main()
{
    std::srand(std::time(NULL));
    auto global_robot = std::make_shared<robot_dart::Robot>("res/models/arm.urdf");

    global_robot->fix_to_world();
    global_robot->set_position_enforced(true);
    // g_robot->skeleton()->setPosition(1, M_PI / 2.0);
    Eigen::Vector3d size(0, 0, 0);

    global_robot->set_actuator_types(dart::dynamics::Joint::VELOCITY);

    std::vector<double> ctrl;
    ctrl = {0.0, 1.0, -1.5, 1.0};

    global_robot->add_controller(std::make_shared<robot_dart::control::PDControl>(ctrl));

    auto g_robot = global_robot->clone();

    robot_dart::RobotDARTSimu simu;
#ifdef GRAPHIC
    // setting up a first camera, with a low resolution
    auto cam1 = std::make_shared<robot_dart::graphics::CameraOSR>(simu.world(), 128, 128);
    // adding the camera to the simulation
    simu.add_camera(cam1);
    // setting the camera to recording (it will write the image in a file)
    cam1->set_recording(true);
    // setting the filename of the image
    cam1->set_filename("cam1.png");
    // setting the direction of the camera
    cam1->look_at({0.4, 1.5, 1.75}, {0.0, 0., 0.5});

    // setting up a second camera with a higher resolution
    auto cam2 = std::make_shared<robot_dart::graphics::CameraOSR>(simu.world(), 512, 512);
    simu.add_camera(cam2);
    cam2->set_recording(true);
    cam2->set_filename("cam2.png");
    cam2->look_at({0.0, 1.75, 0.45}, {0.0, 0.0, 0.45});
    // this camera is disabled, it will not render and record images.
    cam2->set_enable(false);

    // The usual graphics can be also added
    simu.set_graphics(std::make_shared<robot_dart::graphics::Graphics>(simu.world()));
#endif
    simu.add_descriptor(std::make_shared<StateDesc>(simu));
    simu.add_robot(g_robot);
    simu.run(1.);

    // a pointer to the last image taken cam be found here:
    auto image_cam1 = cam1->image();
    std::cout << "Cam1: " << image_cam1->s() << "x" << image_cam1->t() << std::endl;

#ifdef GRAPHIC
    // we can take a single shot with a camera:
    cam2->take_single_shot();
#endif

    g_robot.reset();
    global_robot.reset();
    return 0;
}
