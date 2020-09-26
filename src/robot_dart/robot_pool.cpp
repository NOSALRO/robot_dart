#include <robot_dart/robot_pool.hpp>

namespace robot_dart {
    RobotPool::RobotPool(const std::function<std::shared_ptr<Robot>()>& robot_creator, size_t pool_size) : _robot_creator(robot_creator), _pool_size(pool_size)
    {
        for (size_t i = 0; i < _pool_size; ++i) {
            auto robot = robot_creator();
            _set_init_pos(robot->skeleton());
            _skeletons.push_back(robot->skeleton());
        }
        for (size_t i = 0; i < _pool_size; i++)
            _free.push_back(true);
    }

    std::shared_ptr<Robot> RobotPool::get_robot(const std::string& name)
    {
        std::lock_guard<std::mutex> lock(_skeleton_mutex);
        while (true) {
            for (size_t i = 0; i < _pool_size; i++)
                if (_free[i]) {
                    _free[i] = false;
                    return std::make_shared<Robot>(_skeletons[i], name);
                }
        }
    }

    void RobotPool::free_robot(const std::shared_ptr<Robot>& robot)
    {
        std::lock_guard<std::mutex> lock(_skeleton_mutex);
        for (size_t i = 0; i < _pool_size; i++) {
            if (_skeletons[i] == robot->skeleton()) {
                _set_init_pos(_skeletons[i]);
                _free[i] = true;
                break;
            }
        }
    }

    void RobotPool::_set_init_pos(const dart::dynamics::SkeletonPtr& skel)
    {
        skel->resetPositions();
        skel->resetVelocities();
        skel->resetAccelerations();
        skel->clearExternalForces();
        skel->clearInternalForces();
        skel->clearConstraintImpulses();
        skel->resetCommands();

        // reset any altered properties (e.g., altered mass of a link) and set initial positions
    }
} // namespace robot_dart
