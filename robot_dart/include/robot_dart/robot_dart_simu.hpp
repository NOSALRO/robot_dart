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

#ifdef GRAPHIC
#include <dart/gui/osg/osg.hpp>
#endif

namespace robot_dart {

    BOOST_PARAMETER_TEMPLATE_KEYWORD(robot_control)
    BOOST_PARAMETER_TEMPLATE_KEYWORD(desc)
    BOOST_PARAMETER_TEMPLATE_KEYWORD(viz)

    typedef boost::parameter::parameters<boost::parameter::optional<tag::robot_control>,
        boost::parameter::optional<tag::desc>,
        boost::parameter::optional<tag::viz>>
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

    template <class A1 = boost::parameter::void_, class A2 = boost::parameter::void_, class A3 = boost::parameter::void_>
    class RobotDARTSimu {
    public:
        using robot_t = std::shared_ptr<Robot>;
        // defaults
        struct defaults {
            using robot_control_t = RobotControl;
            using descriptors_t = boost::fusion::vector<>;
            using viz_t = boost::fusion::vector<>;
        };

        // extract the types
        using args = typename class_signature::bind<A1, A2, A3>::type;
        using robot_control_t = typename boost::parameter::binding<args, tag::robot_control, typename defaults::robot_control_t>::type;
        using Descriptors = typename boost::parameter::binding<args, tag::desc, typename defaults::descriptors_t>::type;
        using Visualizations = typename boost::parameter::binding<args, tag::viz, typename defaults::viz_t>::type;
        using descriptors_t = typename boost::mpl::if_<boost::fusion::traits::is_sequence<Descriptors>, Descriptors, boost::fusion::vector<Descriptors>>::type;
        using viz_t = typename boost::mpl::if_<boost::fusion::traits::is_sequence<Visualizations>, Visualizations, boost::fusion::vector<Visualizations>>::type;

        RobotDARTSimu(const std::vector<double>& ctrl, robot_t robot) : _energy(0.0),
                                                                        _world(std::make_shared<dart::simulation::World>()),
                                                                        _controller(ctrl, robot),
                                                                        _old_index(0),
                                                                        _desc_period(1),
                                                                        _break(false)
        {
            // TODO: Make it more generic
            _world->getConstraintSolver()->setCollisionDetector(dart::collision::DARTCollisionDetector::create());
            _robot = robot;
            _world->setTimeStep(0.015);
            _world->addSkeleton(_robot->skeleton());

            _world->setTime(0.0);

#ifdef GRAPHIC
            _fixed_camera = false;
            _osg_world_node = new dart::gui::osg::WorldNode(_world);
            _osg_viewer.addWorldNode(_osg_world_node);
            _osg_viewer.setUpViewInWindow(0, 0, 640, 480);
// full-screen
// _osg_viewer.setUpViewOnSingleScreen();
#endif
        }

        ~RobotDARTSimu() {}

        void run(double max_duration = 5.0)
        {
            _break = false;
            robot_t rob = this->robot();
            size_t index = _old_index;
            double old_t = _world->getTime();

#ifdef GRAPHIC
            while ((_world->getTime() - old_t) < max_duration && !_osg_viewer.done())
#else
            while ((_world->getTime() - old_t) < max_duration)
#endif
            {
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

#ifdef GRAPHIC
                if (!_fixed_camera) {
                    auto COM = rob->skeleton()->getCOM();
                    // set camera to follow hexapod
                    _osg_viewer.getCameraManipulator()->setHomePosition(
                        osg::Vec3d(-0.5, 3, 1), osg::Vec3d(COM(0), COM(1), COM(2)), osg::Vec3d(0, 0, 1));
                    _osg_viewer.home();
                }
                // process next frame
                _osg_viewer.frame();
#endif

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

        dart::simulation::WorldPtr world()
        {
            return _world;
        }

#ifdef GRAPHIC
        void fixed_camera(const Eigen::Vector3d& camera_pos, const Eigen::Vector3d& look_at = Eigen::Vector3d(0, 0, 0), const Eigen::Vector3d& up = Eigen::Vector3d(0, 0, 1))
        {
            _fixed_camera = true;
            _camera_pos = camera_pos;
            _look_at = look_at;
            _camera_up = up;

            // set camera position
            _osg_viewer.getCameraManipulator()->setHomePosition(
                osg::Vec3d(_camera_pos(0), _camera_pos(1), _camera_pos(2)), osg::Vec3d(_look_at(0), _look_at(1), _look_at(2)), osg::Vec3d(_camera_up(0), _camera_up(1), _camera_up(2)));
            _osg_viewer.home();
        }

        void follow_robot()
        {
            _fixed_camera = false;
        }
#endif

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
#ifdef GRAPHIC
        bool _fixed_camera;
        Eigen::Vector3d _look_at;
        Eigen::Vector3d _camera_pos;
        Eigen::Vector3d _camera_up;
        osg::ref_ptr<dart::gui::osg::WorldNode> _osg_world_node;
        dart::gui::osg::Viewer _osg_viewer;
#endif
    };
}

#endif
