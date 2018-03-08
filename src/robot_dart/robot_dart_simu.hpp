#ifndef ROBOT_DART_SIMU_HPP
#define ROBOT_DART_SIMU_HPP

#include <dart/simulation/World.hpp>

#include <robot_dart/descriptor/base_descriptor.hpp>
#include <robot_dart/graphics/base_graphics.hpp>
#include <robot_dart/robot.hpp>

namespace robot_dart {
    class RobotDARTSimu {
    public:
        using robot_t = std::shared_ptr<Robot>;

        RobotDARTSimu(double time_step = 0.015);

        ~RobotDARTSimu();

        void run(double max_duration = 5.0);

        std::shared_ptr<graphics::BaseGraphics> graphics() const;
        void set_graphics(const std::shared_ptr<graphics::BaseGraphics>& graphics);

        dart::simulation::WorldPtr world();

        template <typename Descriptor>
        void add_descriptor(size_t desc_dump = 1)
        {
            add_descriptor(std::make_shared<Descriptor>(Descriptor{*this, desc_dump}));
        }

        void add_descriptor(const std::shared_ptr<descriptor::BaseDescriptor>& desc);
        std::vector<std::shared_ptr<descriptor::BaseDescriptor>> descriptors() const;
        std::shared_ptr<descriptor::BaseDescriptor> descriptor(size_t index) const;

        double step() const;
        void set_step(double step);

        void stop_sim(bool disable = true);

        size_t num_robots() const;
        std::vector<robot_t> robots() const;

        void add_robot(const robot_t& robot);
        void remove_robot(const robot_t& robot);
        void remove_robot(size_t index);
        void clear_robots();

        robot_t robot(size_t index) const;

        // // pose: RPY-Position, dims: XYZ
        // void add_box(const Eigen::Vector6d& pose, const Eigen::Vector3d& dims, const std::string& type = "free", double mass = 1.0, const Eigen::Vector4d& color = dart::Color::Red(1.0), const std::string& box_name = "box")
        // {
        //     std::string name = box_name;
        //     // We do not want boxes with the same names!
        //     while (_world->getSkeleton(name) != nullptr) {
        //         if (name[name.size() - 2] == '_') {
        //             int i = name.back() - '0';
        //             i++;
        //             name.pop_back();
        //             name = name + std::to_string(i);
        //         }
        //         else {
        //             name = name + "_1";
        //         }
        //     }

        //     dart::dynamics::SkeletonPtr box_skel = dart::dynamics::Skeleton::create(name);

        //     // Give the box a body
        //     dart::dynamics::BodyNodePtr body;
        //     if (type == "free")
        //         body = box_skel->createJointAndBodyNodePair<dart::dynamics::FreeJoint>(nullptr).second;
        //     else
        //         body = box_skel->createJointAndBodyNodePair<dart::dynamics::WeldJoint>(nullptr).second;
        //     body->setMass(mass);
        //     body->setName(name);

        //     // Give the body a shape
        //     auto box = std::make_shared<dart::dynamics::BoxShape>(dims);
        //     auto box_node = body->createShapeNodeWith<dart::dynamics::VisualAspect, dart::dynamics::CollisionAspect, dart::dynamics::DynamicsAspect>(box);
        //     box_node->getVisualAspect()->setColor(color);

        //     // Put the body into position
        //     if (type == "free") // free floating
        //         box_skel->setPositions(pose);
        //     else // fixed
        //     {
        //         Eigen::Isometry3d T;
        //         T.linear() = dart::math::eulerXYZToMatrix(pose.head(3));
        //         T.translation() = pose.tail(3);
        //         body->getParentJoint()->setTransformFromParentBodyNode(T);
        //     }

        //     _world->addSkeleton(box_skel);
        //     _objects.push_back(box_skel);
        // }

        // // pose: RPY-Position, dims: XYZ
        // void add_ellipsoid(const Eigen::Vector6d& pose, const Eigen::Vector3d& dims, const std::string& type = "free", double mass = 1.0, const Eigen::Vector4d& color = dart::Color::Red(1.0), const std::string& ellipsoid_name = "sphere")
        // {
        //     std::string name = ellipsoid_name;
        //     // We do not want ellipsoids with the same names!
        //     while (_world->getSkeleton(name) != nullptr) {
        //         if (name[name.size() - 2] == '_') {
        //             int i = name.back() - '0';
        //             i++;
        //             name.pop_back();
        //             name = name + std::to_string(i);
        //         }
        //         else {
        //             name = name + "_1";
        //         }
        //     }

        //     dart::dynamics::SkeletonPtr ellipsoid_skel = dart::dynamics::Skeleton::create(name);

        //     // Give the ellipsoid a body
        //     dart::dynamics::BodyNodePtr body;
        //     if (type == "free")
        //         body = ellipsoid_skel->createJointAndBodyNodePair<dart::dynamics::FreeJoint>(nullptr).second;
        //     else
        //         body = ellipsoid_skel->createJointAndBodyNodePair<dart::dynamics::WeldJoint>(nullptr).second;
        //     body->setMass(mass);
        //     body->setName(name);

        //     // Give the body a shape
        //     auto ellipsoid = std::make_shared<dart::dynamics::EllipsoidShape>(dims);
        //     auto ellipsoid_node = body->createShapeNodeWith<dart::dynamics::VisualAspect, dart::dynamics::CollisionAspect, dart::dynamics::DynamicsAspect>(ellipsoid);
        //     ellipsoid_node->getVisualAspect()->setColor(color);

        //     // Put the body into position
        //     if (type == "free") // free floating
        //         ellipsoid_skel->setPositions(pose);
        //     else // fixed
        //     {
        //         Eigen::Isometry3d T;
        //         T.linear() = dart::math::eulerXYZToMatrix(pose.head(3));
        //         T.translation() = pose.tail(3);
        //         body->getParentJoint()->setTransformFromParentBodyNode(T);
        //     }

        //     _world->addSkeleton(ellipsoid_skel);
        //     _objects.push_back(ellipsoid_skel);
        // }

        // // pose: RPY-Position
        // void add_skeleton(const dart::dynamics::SkeletonPtr& skel, const Eigen::Vector6d& pose, const std::string& type = "free", const std::string& name = "")
        // {
        //     // Put the body into position
        //     if (type == "free") // free floating
        //     {
        //         for (int i = 0; i < 6; i++)
        //             skel->setPosition(i, pose(i));
        //     }
        //     else // fixed
        //     {
        //         Eigen::Isometry3d T;
        //         T.linear() = dart::math::eulerXYZToMatrix(pose.head(3));
        //         T.translation() = pose.tail(3);
        //         skel->getRootBodyNode()->changeParentJointType<dart::dynamics::WeldJoint>();
        //         skel->getRootBodyNode()->getParentJoint()->setTransformFromParentBodyNode(T);
        //     }

        //     if (name != "")
        //         skel->setName(name);

        //     _world->addSkeleton(skel);
        //     _objects.push_back(skel);
        // }

        void add_floor(double floor_width = 10.0, double floor_height = 0.1, const std::string& floor_name = "floor", double x = 0.0, double y = 0.0);

    protected:
        dart::simulation::WorldPtr _world;
        size_t _old_index;
        bool _break;
        std::vector<std::shared_ptr<descriptor::BaseDescriptor>> _descriptors;
        std::vector<robot_t> _robots;
        std::shared_ptr<graphics::BaseGraphics> _graphics;
    };
} // namespace robot_dart

#endif
