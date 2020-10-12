#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <thread>

#include <robot_dart/robot_dart_simu.hpp>
#include <robot_dart/robot_pool.hpp>

static constexpr int NUM_THREADS = 12;

void simulate_robot(const std::shared_ptr<robot_dart::Robot>& robot)
{
    robot->set_position_enforced(true);
    robot->skeleton()->setPosition(5, 1.1);
    robot->skeleton()->setPosition(2, 1.57);

    // Set actuator types to VELOCITY (for speed)
    robot->set_actuator_types("velocity");

    double dt = 0.001;
    robot_dart::RobotDARTSimu simu(dt);
    simu.set_collision_detector("fcl");
    simu.add_checkerboard_floor();
    simu.add_robot(robot);

    simu.set_control_freq(100);
    std::vector<std::string> dofs = {
        "arm_left_1_joint",
        "arm_left_2_joint",
        "arm_right_1_joint",
        "arm_right_2_joint",
        "torso_1_joint"};

    Eigen::VectorXd init_positions = robot->positions(dofs);

    while (simu.scheduler().next_time() <= 3.) { // 3 second of simu only
        if (simu.schedule(simu.control_freq())) {
            Eigen::VectorXd delta_pos(5);
            delta_pos << sin(simu.scheduler().current_time() * 2.),
                sin(simu.scheduler().current_time() * 2.),
                sin(simu.scheduler().current_time() * 2.),
                sin(simu.scheduler().current_time() * 2.),
                sin(simu.scheduler().current_time() * 2.);
            Eigen::VectorXd commands = (init_positions + delta_pos) - robot->positions(dofs);
            robot->set_commands(commands, dofs);
        }
        simu.step_world();
    }
}

namespace pool {
    std::shared_ptr<robot_dart::Robot> robot_creator()
    {
        std::vector<std::pair<std::string, std::string>> packages = {{"talos_description", "talos/talos_description"}};
        return std::make_shared<robot_dart::Robot>("talos/talos.urdf", packages);
    }

    robot_dart::RobotPool robot_pool(robot_creator, NUM_THREADS);
} // namespace pool

void eval_robot(int i)
{
    auto robot = pool::robot_pool.get_robot();
    std::cout << "Robot " << i << " got [" << robot->skeleton() << "]" << std::endl;

    /// --- some robot_dart code ---
    simulate_robot(robot);
    // --- do something with the result

    std::cout << "End of simulation " << i << std::endl;

    // CRITICAL : free your robot !
    pool::robot_pool.free_robot(robot);

    std::cout << "Robot " << i << " freed!" << std::endl;
}

int main(int argc, char** argv)
{
    // for the example, we run NUM_THREADS threads of eval_robot()
    std::vector<std::thread> threads(NUM_THREADS * 2); // *2 to see some reuse
    for (size_t i = 0; i < threads.size(); ++i)
        threads[i] = std::thread(eval_robot, i);

    // wait for the threads to finish
    for (size_t i = 0; i < threads.size(); ++i)
        threads[i].join();
    return 0;
}
