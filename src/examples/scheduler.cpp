#include "robot_dart/scheduler.hpp"

int main()
{
    Scheduler scheduler(1e-3, true);
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

        scheduler.sync();
    }
    return 0;
}
