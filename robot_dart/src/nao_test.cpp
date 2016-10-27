#include <iostream>
#include <cstdlib>
#include <robot_dart/robot_dart_simu.hpp>
#include <robot_dart/spd_control.hpp>

#ifdef GRAPHIC
#include <robot_dart/graphics.hpp>

struct Params {
    struct graphics : robot_dart::defaults::graphics {
        RS_PARAM_ARRAY(int, resolution, 1280, 720);
    };
};
#endif

int main()
{
    auto global_robot = std::make_shared<robot_dart::Robot>("res/models/nao.urdf");
    auto g_robot = global_robot->clone();
    g_robot->skeleton()->setPosition(5, 0.8);
    g_robot->fix_to_world();

    std::vector<double> ctrl;
    // ctrl = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    // ctrl = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    ctrl = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
#ifdef GRAPHIC
    robot_dart::RobotDARTSimu<robot_dart::robot_control<robot_dart::SPDControl>, robot_dart::graph<robot_dart::graphics::Graphics<Params>>> simu(ctrl, g_robot);
    auto g = simu.graphics();
    g.free_camera();
#else
    robot_dart::RobotDARTSimu<robot_dart::robot_control<robot_dart::SPDControl>> simu(ctrl, g_robot);
#endif
    simu.add_floor();
    simu.run(30);

    global_robot.reset();
    g_robot.reset();
    return 0;
}
