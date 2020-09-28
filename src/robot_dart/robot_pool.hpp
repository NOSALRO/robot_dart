#ifndef _TALOS_CLONE_POOL
#define _TALOS_CLONE_POOL

#include <functional>
#include <memory>
#include <mutex>
#include <vector>

#include <robot_dart/robot.hpp>

namespace robot_dart {
    class RobotPool {
    public:
        using robot_creator_t = std::function<std::shared_ptr<Robot>()>;

        RobotPool(const robot_creator_t& robot_creator, size_t pool_size = 32, bool verbose = true);
        virtual ~RobotPool() {}

        RobotPool(const RobotPool&) = delete;
        void operator=(const RobotPool&) = delete;

        virtual std::shared_ptr<Robot> get_robot(const std::string& name = "robot");
        virtual void free_robot(const std::shared_ptr<Robot>& robot);

    protected:
        robot_creator_t _robot_creator;
        size_t _pool_size;
        bool _verbose;
        std::vector<dart::dynamics::SkeletonPtr> _skeletons;
        std::vector<bool> _free;
        std::mutex _skeleton_mutex;
        std::string _model_filename;

        virtual void _set_init_pos(const dart::dynamics::SkeletonPtr& skel);
    };
} // namespace robot_dart

#endif
