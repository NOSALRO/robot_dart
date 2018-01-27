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

        virtual void operator()() = 0;

    protected:
        const RobotDARTSimu& _simu;
    };
} // namespace robot_dart

#endif
