

# File robot\_pool.cpp

[**File List**](files.md) **>** [**robot\_dart**](dir_166284c5f0440000a6384365f2a45567.md) **>** [**robot\_pool.cpp**](robot__pool_8cpp.md)

[Go to the documentation of this file](robot__pool_8cpp.md)

```C++

#include <robot_dart/robot_pool.hpp>

namespace robot_dart {
    RobotPool::RobotPool(const std::function<std::shared_ptr<Robot>()>& robot_creator, size_t pool_size, bool verbose) : _robot_creator(robot_creator), _pool_size(pool_size), _verbose(verbose)
    {
        if (_verbose) {
            std::cout << "Creating a pool of " << pool_size << " robots: ";
            std::cout.flush();
        }

        for (size_t i = 0; i < _pool_size; ++i) {
            if (_verbose) {
                std::cout << "[" << i << "]";
                std::cout.flush();
            }
            auto robot = robot_creator();
            _model_filename = robot->model_filename();
            _reset_robot(robot);
            _skeletons.push_back(robot->skeleton());
        }
        for (size_t i = 0; i < _pool_size; i++)
            _free.push_back(true);

        if (_verbose)
            std::cout << std::endl;
    }

    std::shared_ptr<Robot> RobotPool::get_robot(const std::string& name)
    {
        while (true) {
            std::lock_guard<std::mutex> lock(_skeleton_mutex);
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
                _reset_robot(robot);
                _free[i] = true;
                break;
            }
        }
    }

    void RobotPool::_reset_robot(const std::shared_ptr<Robot>& robot)
    {
        robot->reset();
    }
} // namespace robot_dart

```

