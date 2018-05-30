#include "robot_dart_simu.hpp"
#include "utils.hpp"

#include <dart/collision/dart/DARTCollisionDetector.hpp>
#include <dart/constraint/ConstraintSolver.hpp>
#include <dart/dynamics/BoxShape.hpp>
#include <dart/dynamics/WeldJoint.hpp>

namespace robot_dart {
    RobotDARTSimu::RobotDARTSimu(double time_step) : _world(std::make_shared<dart::simulation::World>()),
                                                     _old_index(0),
                                                     _break(false)
    {
        _world->getConstraintSolver()->setCollisionDetector(dart::collision::DARTCollisionDetector::create());
        _world->setTimeStep(time_step);
        _world->setTime(0.0);
        _graphics = std::make_shared<graphics::BaseGraphics>(_world);
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

            // update descriptors
            for (auto& desc : _descriptors)
                if (index % desc->desc_dump() == 0)
                    desc->operator()();

            if (_break)
                break;

            ++index;
        }
        _old_index = index;
    }

    std::shared_ptr<graphics::BaseGraphics> RobotDARTSimu::graphics() const
    {
        return _graphics;
    }

    void RobotDARTSimu::set_graphics(const std::shared_ptr<graphics::BaseGraphics>& graphics)
    {
        _graphics = graphics;
    }

    dart::simulation::WorldPtr RobotDARTSimu::world()
    {
        return _world;
    }

    void RobotDARTSimu::add_descriptor(const std::shared_ptr<descriptor::BaseDescriptor>& desc)
    {
        _descriptors.push_back(desc);
    }

    std::vector<std::shared_ptr<descriptor::BaseDescriptor>> RobotDARTSimu::descriptors() const
    {
        return _descriptors;
    }

    std::shared_ptr<descriptor::BaseDescriptor> RobotDARTSimu::descriptor(size_t index) const
    {
        ROBOT_DART_ASSERT(index < _descriptors.size(), "Descriptor index out of bounds", nullptr);
        return _descriptors[index];
    }

    double RobotDARTSimu::step() const
    {
        return _world->getTimeStep();
    }

    void RobotDARTSimu::set_step(double step)
    {
        _world->setTimeStep(step);
        _graphics->set_render_period(step);
    }

    void RobotDARTSimu::stop_sim(bool disable)
    {
        _break = disable;
    }

    bool RobotDARTSimu::halted_sim() const
    {
        return _break;
    }

    size_t RobotDARTSimu::num_robots() const
    {
        return _robots.size();
    }

    std::vector<std::shared_ptr<Robot>> RobotDARTSimu::robots() const
    {
        return _robots;
    }

    std::shared_ptr<Robot> RobotDARTSimu::robot(size_t index) const
    {
        ROBOT_DART_ASSERT(index < _robots.size(), "Robot index out of bounds", nullptr);
        return _robots[index];
    }

    void RobotDARTSimu::add_robot(const std::shared_ptr<Robot>& robot)
    {
        if (robot->skeleton()) {
            _robots.push_back(robot);
            _world->addSkeleton(robot->skeleton());
        }
    }

    void RobotDARTSimu::remove_robot(const std::shared_ptr<Robot>& robot)
    {
        auto it = std::find(_robots.begin(), _robots.end(), robot);
        if (it != _robots.end()) {
            _world->removeSkeleton(robot->skeleton());
            _robots.erase(it);
        }
    }

    void RobotDARTSimu::remove_robot(size_t index)
    {
        ROBOT_DART_ASSERT(index < _robots.size(), "Robot index out of bounds", );
        _world->removeSkeleton(_robots[index]->skeleton());
        _robots.erase(_robots.begin() + index);
    }

    void RobotDARTSimu::clear_robots()
    {
        for (auto& robot : _robots) {
            _world->removeSkeleton(robot->skeleton());
        }
        _robots.clear();
    }

    void RobotDARTSimu::remove_descriptor(const std::shared_ptr<descriptor::BaseDescriptor>& desc)
    {
        auto it = std::find(_descriptors.begin(), _descriptors.end(), desc);
        if (it != _descriptors.end()) {
            _descriptors.erase(it);
        }
    }

    void RobotDARTSimu::remove_descriptor(size_t index)
    {
        ROBOT_DART_ASSERT(index < _descriptors.size(), "Descriptor index out of bounds", );
        _descriptors.erase(_descriptors.begin() + index);
    }

    void RobotDARTSimu::clear_descriptors()
    {
        _descriptors.clear();
    }

    void RobotDARTSimu::add_floor(double floor_width, double floor_height, const Eigen::Vector6d& pose, const std::string& floor_name)
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
        // tf.translation() = Eigen::Vector3d(x, y, -floor_height / 2.0);
        tf.linear() = dart::math::eulerXYZToMatrix(pose.head(3));
        tf.translation() = pose.tail(3);
        tf.translation()[2] -= floor_height / 2.0;
        body->getParentJoint()->setTransformFromParentBodyNode(tf);

        _world->addSkeleton(floor_skel);
    }
} // namespace robot_dart