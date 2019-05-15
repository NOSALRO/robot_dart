#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <robot_dart/control/pd_control.hpp>
#include <robot_dart/robot_dart_simu.hpp>

#ifdef GRAPHIC
#include <robot_dart/graphics/graphics.hpp>
#include <robot_dart/graphics/camera_osr.hpp>
#endif

struct StateDesc : public robot_dart::descriptor::BaseDescriptor {
    StateDesc(robot_dart::RobotDARTSimu& simu, size_t desc_dump = 1) : robot_dart::descriptor::BaseDescriptor(simu, desc_dump) {}

    void operator()()
    {
        if (_simu.robots().size() > 0) {
            states.push_back(_simu.robots()[0]->skeleton()->getPositions());
        }
    }

    std::vector<Eigen::VectorXd> states;
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
    // setting up a first camera, with a high resolution
    auto cam1 = std::make_shared<robot_dart::graphics::CameraOSR>(simu.world(), 512, 512);
    // adding the camera to the simulation
    simu.add_camera(cam1);
    // setting the camera to recording (it will write the image in a file)
    cam1->set_recording(true);
    // setting the filename of the image
    cam1->set_filename("cam1.png");
    // setting the direction of the camera
    cam1->look_at({0.4, 1.5, 1.75}, {0.0, 0., 0.5});
    // a pointer to the image cam be found here:
    auto image_cam1 = cam1->get_image();
    

    // setting up a second camera with a different resolution
    auto cam2 = std::make_shared<robot_dart::graphics::CameraOSR>(simu.world(), 128, 128);
    simu.add_camera(cam2);
    cam2->set_recording(true);
    cam2->set_filename("cam2.png");
    cam2->look_at({0.0, 1.75, 0.45}, {0.0, 0.0, 0.45});

    // The usual graphics can be also added
    //simu.set_graphics(std::make_shared<robot_dart::graphics::Graphics>(simu.world()));
#endif
    simu.add_descriptor(std::make_shared<StateDesc>(simu));
    simu.add_robot(g_robot);
    std::cout << (g_robot->body_trans("arm_link_5") * size).transpose() << std::endl;
    simu.run(0.2);
    std::cout << (g_robot->body_trans("arm_link_5") * size).transpose() << std::endl;
    ctrl = {0.0, -1.0, 1.5, -1.0};
    g_robot->controllers()[0]->set_parameters(ctrl);
    std::static_pointer_cast<robot_dart::control::PDControl>(g_robot->controllers()[0])->set_pd(20., 0.);
    simu.run(0.2);
    std::cout << (g_robot->body_trans("arm_link_5") * size).transpose() << std::endl;

    std::cout << std::static_pointer_cast<StateDesc>(simu.descriptor(0))->states.size() << std::endl;

    g_robot.reset();
    global_robot.reset();
    return 0;
}
