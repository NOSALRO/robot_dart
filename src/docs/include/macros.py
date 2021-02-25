
def define_env(env):
    variables = {'HELLO_WORLD_INCLUDE': '```cpp\n#include <robot_dart/robot_dart_simu.hpp>\n\n#ifdef GRAPHIC\n#include <robot_dart/gui/magnum/graphics.hpp>\n#endif\n```', 'HELLO_WORLD_ROBOT_CREATION': '```cpp\nauto robot = std::make_shared<robot_dart::Robot>("pexod.urdf");\n```', 'HELLO_WORLD_ROBOT_PLACING': '```cpp\nrobot->set_base_pose(robot_dart::make_tf({0., 0., 0.2}));\n```', 'HELLO_WORLD_ROBOT_SIMU': '```cpp\nrobot_dart::RobotDARTSimu simu(0.001); // dt=0.001, 1KHz simulation\nsimu.add_floor();\nsimu.add_robot(robot);\n```', 'HELLO_WORLD_ROBOT_GRAPHIC': '```cpp\n#ifdef GRAPHIC\n    auto graphics = std::make_shared<robot_dart::gui::magnum::Graphics>();\n    simu.set_graphics(graphics);\n    graphics->look_at({0.5, 3., 0.75}, {0.5, 0., 0.2});\n#endif\n```', 'HELLO_WORLD_ROBOT_RUN': '```cpp\nsimu.run(10.);\n```'}
    for v in variables.items():
        env.variables[v[0]] = variables[v[0]]
