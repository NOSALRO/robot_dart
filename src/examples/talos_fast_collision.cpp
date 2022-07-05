#include <robot_dart/robot_dart_simu.hpp>
#include <robot_dart/robots/talos.hpp>

#ifdef GRAPHIC
#include <robot_dart/gui/magnum/graphics.hpp>
#endif

// fast TalosFastCollision:
// - use dart for collision detection (instead of FCL) [only handle boxes and spheres]
// - the collisions are detected with boxes, you can have the boxes visual with talos_box.urdf
// - the boxes are totally covering the meshes. There is one box per link
// - the urdf does not have the mimic (used for grippers)
int main()
{
    auto robot = std::make_shared<robot_dart::robots::TalosFastCollision>();
    std::cout << "The model used is: [" << robot->model_filename() << "]" << std::endl;

    // Set actuator types to VELOCITY (for speed)
    robot->set_actuator_types("velocity");

    double dt = 0.001;
    robot_dart::RobotDARTSimu simu(dt);
    // we can use the DART (fast) collision detector
    simu.set_collision_detector("dart");
#ifdef GRAPHIC
    auto graphics = std::make_shared<robot_dart::gui::magnum::Graphics>();
    simu.set_graphics(graphics);
    graphics->look_at({0., 3.5, 2.}, {0., 0., 0.25});
    graphics->record_video("talos_fast.mp4");
#endif
    simu.add_floor();
    simu.add_robot(robot);

    simu.set_control_freq(100);
    std::vector<std::string> dofs = {"arm_left_1_joint",
        "arm_left_2_joint",
        "arm_right_1_joint",
        "arm_right_2_joint",
        "torso_1_joint"};

    Eigen::VectorXd init_positions = robot->positions(dofs);
    std::vector<int> undamaged_joints = {0, 1, 2, 3, 4};
    auto start = std::chrono::steady_clock::now();

    while (simu.scheduler().next_time() < 20. && !simu.graphics()->done()) {
        if (simu.scheduler().next_time() > 2. && simu.scheduler().next_time() < 2.0 + dt) {
            robot->set_actuator_type("passive", "arm_left_1_joint");
            robot->set_actuator_type("passive", "arm_left_2_joint");
            robot->set_actuator_type("passive", "arm_left_3_joint");
            robot->set_actuator_type("passive", "arm_left_4_joint");
            undamaged_joints = {2, 3, 4};
        }

        if (simu.schedule(simu.control_freq())) {
            Eigen::VectorXd delta_pos(5);
            delta_pos << sin(simu.scheduler().current_time() * 2.),
                sin(simu.scheduler().current_time() * 2.),
                sin(simu.scheduler().current_time() * 2.),
                sin(simu.scheduler().current_time() * 2.),
                sin(simu.scheduler().current_time() * 2.);
            Eigen::VectorXd commands = (init_positions + delta_pos) - robot->positions(dofs);
            Eigen::VectorXd commands_to_send(undamaged_joints.size());
            std::vector<std::string> dofs_to_send;
            for (int i = 0; i < commands_to_send.size(); i++) {
                commands_to_send[i] = commands[undamaged_joints[i]];
                dofs_to_send.push_back(dofs[undamaged_joints[i]]);
            }
            robot->set_commands(commands_to_send, dofs_to_send);
        }
        simu.step_world();
    }

    auto end = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsed_seconds = end - start;
    std::cout << "benchmark time: " << elapsed_seconds.count() << "s\n";

    robot.reset();
    return 0;
}
