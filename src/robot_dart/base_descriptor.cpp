#include <robot_dart/base_descriptor.hpp>

#include <robot_dart/robot_dart_simu.hpp>

namespace robot_dart {
    BaseDescriptor::BaseDescriptor(const RobotDARTSimu& simu, size_t desc_dump) : _simu(simu), _desc_period(desc_dump) {}

    size_t BaseDescriptor::desc_dump() const
    {
        return _desc_period;
    }

    void BaseDescriptor::set_desc_dump(size_t desc_dump)
    {
        _desc_period = desc_dump;
    }
} // namespace robot_dart