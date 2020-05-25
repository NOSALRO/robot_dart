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
        _graphics = std::make_shared<gui::Base>(_world);
    }

    RobotDARTSimu::~RobotDARTSimu()
    {
        _robots.clear();
        _descriptors.clear();
        _cameras.clear();
    }

    void RobotDARTSimu::run(double max_duration)
    {
        _break = false;
        size_t index = _old_index;
        double old_t = _world->getTime();
        double factor = _world->getTimeStep() / 2.;

        while ((_world->getTime() - old_t - max_duration) < -factor && !_graphics->done()) {
            for (auto& robot : _robots)
                robot->update(_world->getTime());

            _world->step(false);

            _graphics->refresh();

            // update descriptors
            for (auto& desc : _descriptors)
                if (index % desc->desc_dump() == 0)
                    desc->operator()();

            // update cameras
            for (auto& cam : _cameras)
                cam->refresh();

            ++index;

            if (_break)
                break;
        }
        _old_index = index;
    }

     void RobotDARTSimu::refresh()
    {
        _break = false;
        size_t index = _old_index;
        double old_t = _world->getTime();

        _world->step(false);

        _graphics->refresh();

        // update descriptors
        for (auto& desc : _descriptors)
            if (index % desc->desc_dump() == 0)
                desc->operator()();

        // update cameras
        for (auto& cam : _cameras)
            cam->refresh();

        ++index;
            
        _old_index = index;
    }


    std::shared_ptr<gui::Base> RobotDARTSimu::graphics() const
    {
        return _graphics;
    }

    void RobotDARTSimu::set_graphics(const std::shared_ptr<gui::Base>& graphics)
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

    void RobotDARTSimu::add_camera(const std::shared_ptr<gui::Base>& cam)
    {
        _cameras.push_back(cam);
    }

    std::vector<std::shared_ptr<gui::Base>> RobotDARTSimu::cameras() const
    {
        return _cameras;
    }

    std::shared_ptr<gui::Base> RobotDARTSimu::camera(size_t index) const
    {
        ROBOT_DART_ASSERT(index < _cameras.size(), "Camera index out of bounds", nullptr);
        return _cameras[index];
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

    void RobotDARTSimu::remove_camera(const std::shared_ptr<gui::Base>& cam)
    {
        auto it = std::find(_cameras.begin(), _cameras.end(), cam);
        if (it != _cameras.end()) {
            _cameras.erase(it);
        }
    }

    void RobotDARTSimu::remove_camera(size_t index)
    {
        ROBOT_DART_ASSERT(index < _cameras.size(), "Cameras index out of bounds", );
        _cameras.erase(_cameras.begin() + index);
    }

    void RobotDARTSimu::clear_cameras()
    {
        _cameras.clear();
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

    void RobotDARTSimu::add_checkerboard_floor(double floor_width, double floor_height, double size, const Eigen::Vector6d& pose, const std::string& floor_name)
    {
        // We do not want 2 floors with the same name!
        if (_world->getSkeleton(floor_name) != nullptr)
            return;

        // Add main floor skeleton
        dart::dynamics::SkeletonPtr main_floor_skel = dart::dynamics::Skeleton::create(floor_name + "_main");

        // Give the floor a body
        dart::dynamics::BodyNodePtr main_body = main_floor_skel->createJointAndBodyNodePair<dart::dynamics::WeldJoint>(nullptr).second;

        // Give the body a shape
        auto box = std::make_shared<dart::dynamics::BoxShape>(Eigen::Vector3d(floor_width, floor_width, floor_height));
        // No visual shape for this one; only collision and dynamics
        main_body->createShapeNodeWith<dart::dynamics::CollisionAspect, dart::dynamics::DynamicsAspect>(box);

        // Put the body into position
        Eigen::Isometry3d tf(Eigen::Isometry3d::Identity());
        // tf.translation() = Eigen::Vector3d(x, y, -floor_height / 2.0);
        tf.linear() = dart::math::eulerXYZToMatrix(pose.head(3));
        tf.translation() = pose.tail(3);
        tf.translation()[2] -= floor_height / 2.0;
        main_body->getParentJoint()->setTransformFromParentBodyNode(tf);

        _world->addSkeleton(main_floor_skel);

        // Add visual bodies just for visualization
        int step = std::ceil(floor_width / size);
        int c = 0;
        for (int i = 0; i < step; i++) {
            c = i;
            for (int j = 0; j < step; j++) {
                Eigen::Vector3d init_pose;
                init_pose << -floor_width / 2. + size / 2 + i * size, -floor_width / 2. + size / 2 + j * size, 0.;
                int id = i * step + j;

                dart::dynamics::WeldJoint::Properties properties;
                properties.mName = "joint_" + std::to_string(id);
                dart::dynamics::BodyNode::Properties bd_properties;
                bd_properties.mName = "body_" + std::to_string(id);
                dart::dynamics::BodyNodePtr body = main_body->createChildJointAndBodyNodePair<dart::dynamics::WeldJoint>(properties, bd_properties).second;

                auto box = std::make_shared<dart::dynamics::BoxShape>(Eigen::Vector3d(size, size, floor_height));
                // no collision/dynamics for these ones; only visual shape
                auto box_node = body->createShapeNodeWith<dart::dynamics::VisualAspect>(box);
                if (c % 2 == 0)
                    box_node->getVisualAspect()->setColor(dart::Color::Gray());
                else
                    box_node->getVisualAspect()->setColor(dart::Color::White());

                // Put the body into position
                Eigen::Isometry3d tf(Eigen::Isometry3d::Identity());
                tf.linear() = dart::math::eulerXYZToMatrix(pose.head(3));
                tf.translation() = pose.tail(3) + init_pose;
                body->getParentJoint()->setTransformFromParentBodyNode(tf);

                c++;
            }
        }
    }
} // namespace robot_dart
