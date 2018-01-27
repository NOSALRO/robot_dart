#ifndef ROBOT_DART_BASE_DESCRIPTOR_HPP
#define ROBOT_DART_BASE_DESCRIPTOR_HPP

#include <memory>
#include <vector>

#include <Eigen/Core>

namespace robot_dart {

    class RobotDARTSimu;
    class Robot;

    struct BaseDescriptor {
    public:
        BaseDescriptor(const RobotDARTSimu& simu);

        void set_robots(const std::vector<std::shared_ptr<Robot>>& robots);

        virtual void operator()() = 0;

    protected:
        const RobotDARTSimu& _simu;
        std::vector<std::shared_ptr<Robot>> _robots;
    };
} // namespace robot_dart

#endif
