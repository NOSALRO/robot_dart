#ifndef ROBOT_DART_SIMU_HPP
#define ROBOT_DART_SIMU_HPP

#include <boost/parameter.hpp>
#include <boost/fusion/include/vector.hpp>
#include <boost/fusion/include/accumulate.hpp>
#include <boost/fusion/include/for_each.hpp>
#include <boost/fusion/include/find.hpp>

#include <dart/dart.hpp>
#include <dart/collision/dart/DARTCollisionDetector.hpp>
#include <Eigen/Core>
#include <robot_dart/robot.hpp>
#include <robot_dart/robot_control.hpp>
#include <robot_dart/descriptors.hpp>
#include <robot_dart/visualizations.hpp>
#include <robot_dart/no_graphics.hpp>

namespace robot_dart {

    BOOST_PARAMETER_TEMPLATE_KEYWORD(robot_control)
    BOOST_PARAMETER_TEMPLATE_KEYWORD(desc)
    BOOST_PARAMETER_TEMPLATE_KEYWORD(viz)
    BOOST_PARAMETER_TEMPLATE_KEYWORD(graphics)
    BOOST_PARAMETER_TEMPLATE_KEYWORD(collision)

    typedef boost::parameter::parameters<boost::parameter::optional<tag::robot_control>,
        boost::parameter::optional<tag::desc>,
        boost::parameter::optional<tag::viz>,
        boost::parameter::optional<tag::graphics>,
        boost::parameter::optional<tag::collision>>
        class_signature;

    template <typename Simu, typename robot>
    struct Refresh {
        Refresh(Simu& simu, std::shared_ptr<robot> rob)
            : _simu(simu), _robot(rob) {}

        Simu& _simu;
        std::shared_ptr<robot> _robot;

        template <typename T>
        void operator()(T& x) const { x(_simu, _robot); }
    };

    template <class A1 = boost::parameter::void_, class A2 = boost::parameter::void_, class A3 = boost::parameter::void_, class A4 = boost::parameter::void_, class A5 = boost::parameter::void_>
    class RobotDARTSimu {
    public:
        using robot_t = std::shared_ptr<Robot>;
        // defaults
        struct defaults {
            using robot_control_t = RobotControl;
            using descriptors_t = boost::fusion::vector<>;
            using viz_t = boost::fusion::vector<>;
            using graphics_t = No_Graphics;
            using collision_t = dart::collision::DARTCollisionDetector;
        };

        // extract the types
        using args = typename class_signature::bind<A1, A2, A3, A4, A5>::type;
        using robot_control_t = typename boost::parameter::binding<args, tag::robot_control, typename defaults::robot_control_t>::type;
        using Descriptors = typename boost::parameter::binding<args, tag::desc, typename defaults::descriptors_t>::type;
        using Visualizations = typename boost::parameter::binding<args, tag::viz, typename defaults::viz_t>::type;
        using descriptors_t = typename boost::mpl::if_<boost::fusion::traits::is_sequence<Descriptors>, Descriptors, boost::fusion::vector<Descriptors>>::type;
        using viz_t = typename boost::mpl::if_<boost::fusion::traits::is_sequence<Visualizations>, Visualizations, boost::fusion::vector<Visualizations>>::type;
        using graphics_t = typename boost::parameter::binding<args, tag::graphics, typename defaults::graphics_t>::type;
        using collision_t = typename boost::parameter::binding<args, tag::collision, typename defaults::collision_t>::type;

        RobotDARTSimu(const std::vector<double>& ctrl, robot_t robot) : _energy(0.0),
                                                                        _world(std::make_shared<dart::simulation::World>()),
                                                                        _controller(ctrl, robot),
                                                                        _old_index(0),
                                                                        _desc_period(1),
                                                                        _break(false)
        {
            _robot = robot;
            _world->getConstraintSolver()->setCollisionDetector(collision_t::create());
            _world->setTimeStep(0.015);
            _world->addSkeleton(_robot->skeleton());
            _world->setTime(0.0);
            _graphics = std::make_shared<graphics_t>(_world);
        }

        ~RobotDARTSimu() {}

        void run(double max_duration = 5.0)
        {
            _break = false;
            robot_t rob = this->robot();
            size_t index = _old_index;
            double old_t = _world->getTime();

            while ((_world->getTime() - old_t) < max_duration && !_graphics->done()) {
                Eigen::VectorXd positions = rob->skeleton()->getPositions();
                _controller.update(_world->getTime());

                _world->step(false);

                // integrate Torque (force) over time
                Eigen::VectorXd state = rob->skeleton()->getForces().array().abs() * _world->getTimeStep();
                _energy += state.sum();
                // if ((positions - rob->skeleton()->getPositions()).norm() < 1e-5) //std::numeric_limits<double>::epsilon())
                //     break;

                // update visualizations
                boost::fusion::for_each(_visualizations, Refresh<RobotDARTSimu, Robot>(*this, rob));

                if (_break) {
                    _energy = -10002.0;
                    return;
                }

                _graphics->refresh(*this);

                if (index % _desc_period == 0) {
                    // update descriptors
                    boost::fusion::for_each(_descriptors, Refresh<RobotDARTSimu, Robot>(*this, rob));
                }

                ++index;
            }
            _old_index = index;
        }

        robot_t robot()
        {
            return _robot;
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

        double energy() const
        {
            return _energy;
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

        robot_control_t& controller()
        {
            return _controller;
        }

        // pose: Orientation-Position, dims: XYZ
        void add_box(const Eigen::Vector6d& pose, const Eigen::Vector3d& dims, std::string type = "free", double mass = 1.0, const Eigen::Vector4d& color = dart::Color::Red(1.0), const std::string& box_name = "box")
        {
            std::string name = box_name;
            // We do not want boxes with the same names!
            while (_world->getSkeleton(name) != nullptr) {
                if (name[name.size() - 2] == '_') {
                    int i = name.back() - '0';
                    i++;
                    name.pop_back();
                    name = name + std::to_string(i);
                }
                else {
                    name = name + "_1";
                }
            }

            dart::dynamics::SkeletonPtr box_skel = dart::dynamics::Skeleton::create(name);

            // Give the box a body
            dart::dynamics::BodyNodePtr body;
            if (type == "free")
                body = box_skel->createJointAndBodyNodePair<dart::dynamics::FreeJoint>(nullptr).second;
            else
                body = box_skel->createJointAndBodyNodePair<dart::dynamics::WeldJoint>(nullptr).second;
            body->setMass(mass);
            body->setName(name);

            // Give the body a shape
            auto box = std::make_shared<dart::dynamics::BoxShape>(dims);
            auto box_node = body->createShapeNodeWith<dart::dynamics::VisualAspect, dart::dynamics::CollisionAspect, dart::dynamics::DynamicsAspect>(box);
            box_node->getVisualAspect()->setColor(color);

            // Put the body into position
            if (type == "free") // free floating
                box_skel->setPositions(pose);
            else // fixed
                body->getParentJoint()->setTransformFromParentBodyNode(dart::math::expMap(pose));

            _world->addSkeleton(box_skel);
            _objects.push_back(box_skel);
        }

        // pose: Orientation-Position, dims: XYZ
        void add_ellipsoid(const Eigen::Vector6d& pose, const Eigen::Vector3d& dims, std::string type = "free", double mass = 1.0, const Eigen::Vector4d& color = dart::Color::Red(1.0), const std::string& ellipsoid_name = "sphere")
        {
            std::string name = ellipsoid_name;
            // We do not want ellipsoids with the same names!
            while (_world->getSkeleton(name) != nullptr) {
                if (name[name.size() - 2] == '_') {
                    int i = name.back() - '0';
                    i++;
                    name.pop_back();
                    name = name + std::to_string(i);
                }
                else {
                    name = name + "_1";
                }
            }

            dart::dynamics::SkeletonPtr ellipsoid_skel = dart::dynamics::Skeleton::create(name);

            // Give the ellipsoid a body
            dart::dynamics::BodyNodePtr body;
            if (type == "free")
                body = ellipsoid_skel->createJointAndBodyNodePair<dart::dynamics::FreeJoint>(nullptr).second;
            else
                body = ellipsoid_skel->createJointAndBodyNodePair<dart::dynamics::WeldJoint>(nullptr).second;
            body->setMass(mass);
            body->setName(name);

            // Give the body a shape
            auto ellipsoid = std::make_shared<dart::dynamics::EllipsoidShape>(dims);
            auto ellipsoid_node = body->createShapeNodeWith<dart::dynamics::VisualAspect, dart::dynamics::CollisionAspect, dart::dynamics::DynamicsAspect>(ellipsoid);
            ellipsoid_node->getVisualAspect()->setColor(color);

            // Put the body into position
            if (type == "free") // free floating
                ellipsoid_skel->setPositions(pose);
            else // fixed
                body->getParentJoint()->setTransformFromParentBodyNode(dart::math::expMap(pose));

            _world->addSkeleton(ellipsoid_skel);
            _objects.push_back(ellipsoid_skel);
        }

        void clear_objects()
        {
            for (auto obj : _objects) {
                _world->removeSkeleton(obj);
            }
            _objects.clear();
        }

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
        robot_t _robot;
        Eigen::Vector3d _final_pos;
        double _energy;
        dart::simulation::WorldPtr _world;
        robot_control_t _controller;
        size_t _old_index;
        size_t _desc_period;
        bool _break;
        descriptors_t _descriptors;
        viz_t _visualizations;
        std::vector<dart::dynamics::SkeletonPtr> _objects;
        std::shared_ptr<graphics_t> _graphics;
    };
}

#endif
