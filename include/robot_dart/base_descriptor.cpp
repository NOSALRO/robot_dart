#include <robot_dart/base_descriptor.hpp>

#include <robot_dart/robot_dart_simu.hpp>

namespace robot_dart {
    BaseDescriptor::BaseDescriptor(const RobotDARTSimu& simu) : _simu(simu) {}

    void BaseDescriptor::set_robots(const std::vector<std::shared_ptr<Robot>>& robots)
    {
        _robots = robots;
    }
} // namespace robot_dart