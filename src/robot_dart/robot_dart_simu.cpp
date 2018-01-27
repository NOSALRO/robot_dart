#include <robot_dart/robot_dart_simu.hpp>

namespace robot_dart {
    RobotDARTSimu::RobotDARTSimu(double time_step) : _world(std::make_shared<dart::simulation::World>()),
                                                     _old_index(0),
                                                     _desc_period(1),
                                                     _break(false)
    {
        _world->getConstraintSolver()->setCollisionDetector(dart::collision::DARTCollisionDetector::create());
        _world->setTimeStep(time_step);
        _world->setTime(0.0);
        _graphics = std::make_shared<BaseGraphics>(_world);
    }

    RobotDARTSimu::~RobotDARTSimu()
    {
        _robots.clear();
        _descriptors.clear();
    }

    void RobotDARTSimu::run(double max_duration)
    {
        _break = false;
        size_t index = _old_index;
        double old_t = _world->getTime();

        while ((_world->getTime() - old_t) < max_duration && !_graphics->done()) {
            for (auto& robot : _robots)
                robot->update(_world->getTime());

            _world->step(false);

            _graphics->refresh();

            if (index % _desc_period == 0) {
                // update descriptors
                for (auto& desc : _descriptors)
                    desc->operator()();
            }

            if (_break)
                break;

            ++index;
        }
        _old_index = index;
    }

    std::shared_ptr<BaseGraphics> RobotDARTSimu::graphics() const
    {
        return _graphics;
    }

    void RobotDARTSimu::set_graphics(const std::shared_ptr<BaseGraphics>& graphics)
    {
        _graphics = graphics;
    }

    dart::simulation::WorldPtr RobotDARTSimu::world()
    {
        return _world;
    }

    void RobotDARTSimu::add_descriptor(const std::shared_ptr<BaseDescriptor>& desc)
    {
        _descriptors.push_back(desc);
    }

    std::vector<std::shared_ptr<BaseDescriptor>> RobotDARTSimu::descriptors() const
    {
        return _descriptors;
    }

    std::shared_ptr<BaseDescriptor> RobotDARTSimu::descriptor(size_t index) const
    {
        assert(index < _descriptors.size());
        return _descriptors[index];
    }

    double RobotDARTSimu::step() const
    {
        assert(_world != nullptr);
        return _world->getTimeStep();
    }

    void RobotDARTSimu::set_step(double step)
    {
        assert(_world != nullptr);
        _world->setTimeStep(step);
        _graphics->set_render_period(step);
    }

    size_t RobotDARTSimu::desc_dump() const
    {
        return _desc_period;
    }

    void RobotDARTSimu::set_desc_dump(size_t desc_dump)
    {
        _desc_period = desc_dump;
    }

    void RobotDARTSimu::stop_sim(bool disable)
    {
        _break = disable;
    }

    void RobotDARTSimu::add_robot(const std::shared_ptr<Robot>& robot)
    {
        if (robot->skeleton()) {
            _robots.push_back(robot);
            _world->addSkeleton(robot->skeleton());
        }
    }

    std::vector<std::shared_ptr<Robot>> RobotDARTSimu::robots() const
    {
        return _robots;
    }

    void RobotDARTSimu::clear_robots()
    {
        for (auto& robot : _robots) {
            _world->removeSkeleton(robot->skeleton());
        }
        _robots.clear();
    }

    void RobotDARTSimu::add_floor(double floor_width, double floor_height, std::string floor_name, double x, double y)
    {
        // We do not want 2 floors with the same name!
        if (_world->getSkeleton(floor_name) != nullptr)
            return;

        dart::dynamics::SkeletonPtr floor_skel = dart::dynamics::Skeleton::create(floor_name);

        // Give the floor a body
        dart::dynamics::BodyNodePtr body = floor_skel->createJointAndBodyNodePair<dart::dynamics::WeldJoint>(nullptr).second;

        // Give the body a shape
        auto box = std::make_shared<dart::dynamics::BoxShape>(Eigen::Vector3d(floor_width, floor_width, floor_height));
        auto box_node = body->createShapeNodeWith<dart::dynamics::VisualAspect, dart::dynamics::CollisionAspect, dart::dynamics::DynamicsAspect>(box);
        box_node->getVisualAspect()->setColor(dart::Color::Gray());

        // Put the body into position
        Eigen::Isometry3d tf(Eigen::Isometry3d::Identity());
        tf.translation() = Eigen::Vector3d(x, y, -floor_height / 2.0);
        body->getParentJoint()->setTransformFromParentBodyNode(tf);

        _world->addSkeleton(floor_skel);
    }
} // namespace robot_dart