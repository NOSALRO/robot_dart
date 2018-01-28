#ifndef ROBOT_DART_BASE_DESCRIPTOR_HPP
#define ROBOT_DART_BASE_DESCRIPTOR_HPP

// for size_t
#include <cstddef>

namespace robot_dart {

    class RobotDARTSimu;
    class Robot;

    struct BaseDescriptor {
    public:
        BaseDescriptor(const RobotDARTSimu& simu, size_t desc_dump = 1);

        virtual void operator()() = 0;

        size_t desc_dump() const;
        void set_desc_dump(size_t desc_dump);

    protected:
        const RobotDARTSimu& _simu;
        size_t _desc_period;
    };
} // namespace robot_dart

#endif
