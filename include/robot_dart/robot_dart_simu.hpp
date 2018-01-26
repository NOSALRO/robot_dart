#ifndef ROBOT_DART_SIMU_HPP
#define ROBOT_DART_SIMU_HPP

#include <boost/fusion/include/accumulate.hpp>
#include <boost/fusion/include/find.hpp>
#include <boost/fusion/include/for_each.hpp>
#include <boost/fusion/include/vector.hpp>
#include <boost/parameter.hpp>

#include <Eigen/Core>
#include <dart/collision/dart/DARTCollisionDetector.hpp>
#include <dart/dart.hpp>
#include <robot_dart/descriptors.hpp>
#include <robot_dart/no_graphics.hpp>
#include <robot_dart/robot.hpp>

namespace robot_dart {

    BOOST_PARAMETER_TEMPLATE_KEYWORD(desc)
    BOOST_PARAMETER_TEMPLATE_KEYWORD(graphics)
    BOOST_PARAMETER_TEMPLATE_KEYWORD(collision)

    typedef boost::parameter::parameters<boost::parameter::optional<tag::desc>,
        boost::parameter::optional<tag::graphics>,
        boost::parameter::optional<tag::collision>>
        class_signature;

    template <typename Simu, typename robot>
    struct Refresh {
        Refresh(Simu& simu, const std::vector<std::shared_ptr<robot>>& robots)
            : _simu(simu), _robots(robots) {}

        Simu& _simu;
        std::vector<std::shared_ptr<robot>> _robots;

        template <typename T>
        void operator()(T& x) const { x(_simu, _robots); }
    };

    template <class A1 = boost::parameter::void_, class A2 = boost::parameter::void_, class A3 = boost::parameter::void_>
    class RobotDARTSimu {
    public:
        using robot_t = std::shared_ptr<Robot>;
        // defaults
        struct defaults {
            using descriptors_t = boost::fusion::vector<>;
            using graphics_t = No_Graphics;
            using collision_t = dart::collision::DARTCollisionDetector;
        };

        // extract the types
        using args = typename class_signature::bind<A1, A2, A3>::type;
        using Descriptors = typename boost::parameter::binding<args, tag::desc, typename defaults::descriptors_t>::type;
        using descriptors_t = typename boost::mpl::if_<boost::fusion::traits::is_sequence<Descriptors>, Descriptors, boost::fusion::vector<Descriptors>>::type;
        using graphics_t = typename boost::parameter::binding<args, tag::graphics, typename defaults::graphics_t>::type;
        using collision_t = typename boost::parameter::binding<args, tag::collision, typename defaults::collision_t>::type;

        RobotDARTSimu(double time_step = 0.015) : _world(std::make_shared<dart::simulation::World>()),
                                                  _old_index(0),
                                                  _desc_period(1),
                                                  _break(false)
        {
            _world->getConstraintSolver()->setCollisionDetector(collision_t::create());
            _world->setTimeStep(time_step);
            _world->setTime(0.0);
            _graphics = std::make_shared<graphics_t>(_world);
        }

        ~RobotDARTSimu()
        {
            _robots.clear();
        }

        void run(double max_duration = 5.0)
        {
            _break = false;
            size_t index = _old_index;
            double old_t = _world->getTime();

            while ((_world->getTime() - old_t) < max_duration && !_graphics->done()) {
                for (auto& robot : _robots)
                    robot->update(_world->getTime());

                _world->step(false);

                _graphics->refresh(*this);

                if (index % _desc_period == 0) {
                    // update descriptors
                    boost::fusion::for_each(_descriptors, Refresh<RobotDARTSimu, Robot>(*this, _robots));
                }

                if (_break)
                    break;

                ++index;
            }
            _old_index = index;
        }

        std::shared_ptr<graphics_t> graphics()
        {
            return _graphics;
        }

        dart::simulation::WorldPtr world()
        {
            return _world;
        }

        template <typename Desc, typename T>
        void get_descriptor(T& result)
        {
            auto d = boost::fusion::find<Desc>(_descriptors);
            (*d).get(result);
        }

        double step() const
        {
            assert(_world != nullptr);
            return _world->getTimeStep();
        }

        void set_step(double step)
        {
            assert(_world != nullptr);
            _world->setTimeStep(step);
            _graphics->set_render_period(step);
        }

        size_t desc_dump() const
        {
            return _desc_period;
        }

        void set_desc_dump(size_t desc_dump)
        {
            _desc_period = desc_dump;
        }

        void stop_sim(bool disable = true)
        {
            _break = disable;
        }

        void add_robot(const robot_t& robot)
        {
            if (robot->skeleton()) {
                _robots.push_back(robot);
                _world->addSkeleton(robot->skeleton());
            }
        }

        void clear_robots()
        {
            for (auto& robot : _robots) {
                _world->removeSkeleton(robot->skeleton());
            }
            _robots.clear();
        }

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

        // void clear_objects()
        // {
        //     for (auto obj : _objects) {
        //         _world->removeSkeleton(obj);
        //     }
        //     _objects.clear();
        // }

        void add_floor(double floor_width = 10.0, double floor_height = 0.1, std::string floor_name = "floor", double x = 0.0, double y = 0.0)
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

    protected:
        dart::simulation::WorldPtr _world;
        size_t _old_index;
        size_t _desc_period;
        bool _break;
        descriptors_t _descriptors;
        std::vector<robot_t> _robots;
        std::shared_ptr<graphics_t> _graphics;
    };
} // namespace robot_dart

#endif
