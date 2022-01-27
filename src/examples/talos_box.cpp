#include <robot_dart/robot_dart_simu.hpp>
#include <robot_dart/robots/talos.hpp>

#include <dart/collision/fcl/FCLCollisionDetector.hpp>
#include <dart/constraint/ConstraintSolver.hpp>

#ifdef GRAPHIC
#include <robot_dart/gui/magnum/graphics.hpp>
#endif

// fast TalosFastCollision:
// - use dart for collision detection (instead of FCL) [only handle boxes and spheres]
// - the collisions are detected with boxes, you can have the boxes visual with talos_box.urdf
// - the boxes are totally covering the meshes. There is one box per link
// - the urdf does not have the mimic (used for grippers)
// - you have to add a collisionFilter, it is advised to use simu.set_collision_masks with robot->collision_vec(), an example is below
int main()
{
    auto robot = std::make_shared<robot_dart::robots::TalosFastCollision>(1000, "talos/talos_box.urdf");
    std::cout << "The model used is: [" << robot->model_filename() << "]" << std::endl;

    robot->set_position_enforced(true);
    auto positions = robot->positions();
    positions[2] = M_PI / 2.;
    positions[5] = 1.1;
    robot->set_positions(positions);
    robot->set_self_collision();

    // Set actuator types to VELOCITY (for speed)
    robot->set_actuator_types("velocity");

    double dt = 0.001;
    robot_dart::RobotDARTSimu simu(dt);
    // we use dart, but only the feet are used
    simu.set_collision_detector("dart");
#ifdef GRAPHIC
    auto graphics = std::make_shared<robot_dart::gui::magnum::Graphics>();
    simu.set_graphics(graphics);
    graphics->look_at({0., 3.5, 2.}, {0., 0., 0.25});
    graphics->record_video("talos.mp4");
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
    dart::collision::CollisionResult collision_result;

    while (simu.scheduler().next_time() < 20. && !simu.graphics()->done()) {

        collision_result.clear();
        simu.world()->getConstraintSolver()->getCollisionDetector()->collide(simu.world()->getConstraintSolver()->getCollisionGroup().get(), simu.world()->getConstraintSolver()->getCollisionOption(), &collision_result);
        auto& bodies = collision_result.getCollidingBodyNodes();
        std::cout << "collision names" << std::endl;
        for (auto& b : bodies)
            std::cout << b->getName() << std::endl;
        std::cout << std::endl;

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
