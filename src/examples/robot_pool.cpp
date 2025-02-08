#include <thread>
#include <robot_dart/robot_dart_simu.hpp>
// @ROBOT_POOL_INCLUDE@
#include <robot_dart/robot_pool.hpp>
// @ROBOT_POOL_INCLUDE_END@
#include <robot_dart/robots/talos.hpp>


static constexpr int NUM_THREADS = 12;

inline void simulate_robot(const std::shared_ptr<robot_dart::Robot>& robot)
{
    robot->set_position_enforced(true);
    auto positions = robot->positions();
    positions[2] = M_PI / 2.;
    positions[5] = 1.1;
    robot->set_positions(positions);

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

// @ROBOT_POOL_GLOBAL_NAMESPACE@
namespace pool {
    // This function should load one robot: here we load Talos
    inline std::shared_ptr<robot_dart::Robot> robot_creator()
    {
        return std::make_shared<robot_dart::robots::Talos>();
    }

    // To create the object we need to pass the robot_creator function and the number of maximum parallel threads
    robot_dart::RobotPool robot_pool(robot_creator, NUM_THREADS);
} // namespace pool
// @ROBOT_POOL_GLOBAL_NAMESPACE_END@

// @ROBOT_POOL_EVAL@
inline void eval_robot(int i)
{
    // We get one available robot
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
// @ROBOT_POOL_EVAL_END@

int main()
{
    // @ROBOT_POOL_CREATE_THREADS@
    // for the example, we run NUM_THREADS threads of eval_robot()
    std::vector<std::thread> threads(NUM_THREADS * 2); // *2 to see some reuse
    for (size_t i = 0; i < threads.size(); ++i)
        threads[i] = std::thread(eval_robot, i); // eval_robot is the function that uses the robot
    // @ROBOT_POOL_CREATE_THREADS_END@

    // wait for the threads to finish
    for (size_t i = 0; i < threads.size(); ++i)
        threads[i].join();
    return 0;
}
