#include <robot_dart/scheduler.hpp>

// this is a simple example to show how the scheduling mechanism works
// see examples/control_loop.cpp for a simulation example
int main()
{
    robot_dart::Scheduler scheduler(1e-3, true);

    while (scheduler.next_time() < 5) {
        if (scheduler(1000)) {
            std::cout << 1000 << std::endl;
        }

        if (scheduler(50)) {
            std::cout << 50 << std::endl;
        }

        if (scheduler(20)) {
            std::cout << 20 << std::endl;
        }

        scheduler.step();
    }
    return 0;
}
