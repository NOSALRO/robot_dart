#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE test_robot

#include <boost/filesystem.hpp>
#include <boost/test/unit_test.hpp>

#include <dart/dynamics/BodyNode.hpp>
#include <dart/dynamics/BoxShape.hpp>
#include <dart/dynamics/EllipsoidShape.hpp>

#include <robot_dart/robot.hpp>
#include <robot_dart/utils.hpp>

using namespace robot_dart;

BOOST_AUTO_TEST_CASE(test_constructors)
{
    // bad urdf file location
    BOOST_REQUIRE_EXCEPTION(std::make_shared<Robot>("/tmp/bad-path.urdf"), Assertion, [](const Assertion&) { return true; });
    // bad skeleton pointer
    dart::dynamics::SkeletonPtr null_skeleton = nullptr;
    BOOST_REQUIRE_EXCEPTION(std::make_shared<Robot>(null_skeleton), Assertion, [](const Assertion&) { return true; });
    // test user case
    std::shared_ptr<Robot> robot = nullptr;
    try {
        robot = std::make_shared<Robot>(null_skeleton);
    }
    catch (Assertion&) {
    }

    BOOST_CHECK(robot == nullptr);

    // well-defined URDF
    auto pendulum = std::make_shared<Robot>(std::string(RESPATH) + "/models/pendulum.urdf");
    BOOST_REQUIRE(pendulum);
    // well-defined skeleton
    dart::dynamics::SkeletonPtr dummy_skel = dart::dynamics::Skeleton::create("dummy");
    auto dummy = std::make_shared<Robot>(dummy_skel, "dummy_robot");
    BOOST_REQUIRE(dummy);

    // check name given
    BOOST_CHECK(pendulum->name() == "robot");
    BOOST_CHECK(pendulum->name() == pendulum->skeleton()->getName());

    BOOST_CHECK(dummy->name() == "dummy_robot");
    BOOST_CHECK(dummy->name() == dummy->skeleton()->getName());
    BOOST_CHECK(dummy->name() == dummy_skel->getName());

    // TO-DO: Add checks for damages
}

BOOST_AUTO_TEST_CASE(test_fix_free)
{
    auto pendulum = std::make_shared<Robot>(std::string(RESPATH) + "/models/pendulum.urdf");
    BOOST_REQUIRE(pendulum);

    pendulum->fix_to_world();
    // check that is fixed
    BOOST_CHECK(pendulum->fixed());
    // check that is not free
    BOOST_CHECK(!pendulum->free());

    pendulum->free_from_world();
    // check that is free
    BOOST_CHECK(pendulum->free());
    // check that is not fixed
    BOOST_CHECK(!pendulum->fixed());
}

BOOST_AUTO_TEST_CASE(test_actuators)
{
    auto pexod = std::make_shared<Robot>(std::string(RESPATH) + "/models/pexod.urdf");
    BOOST_REQUIRE(pexod);
    // fix to world
    pexod->fix_to_world();

    // set different actuator type
    pexod->set_actuator_types(dart::dynamics::Joint::ActuatorType::PASSIVE);

    auto types = pexod->actuator_types();
    for (size_t i = 0; i < types.size(); i++) {
        BOOST_CHECK(types[i] == dart::dynamics::Joint::ActuatorType::PASSIVE);
    }

    types[0] = dart::dynamics::Joint::ActuatorType::SERVO;
    types[types.size() - 1] = dart::dynamics::Joint::ActuatorType::LOCKED;
    pexod->set_actuator_types(types);

    types.clear();
    types = pexod->actuator_types();
    for (size_t i = 0; i < types.size(); i++) {
        dart::dynamics::Joint::ActuatorType type = dart::dynamics::Joint::ActuatorType::PASSIVE;
        if (i == 0)
            type = dart::dynamics::Joint::ActuatorType::SERVO;
        else if (i == types.size() - 1)
            type = dart::dynamics::Joint::ActuatorType::LOCKED;
        BOOST_CHECK(types[i] == type);
    }
}

BOOST_AUTO_TEST_CASE(test_static_creation)
{
    // box creation tests
    {
        Eigen::Vector6d pose;
        pose << 0., 0., 0., 2., 0., 0.;
        auto box = Robot::create_box(Eigen::Vector3d(1., 1., 1.), pose);
        // check construction
        BOOST_REQUIRE(box);
        // check size
        Eigen::Vector3d size = std::static_pointer_cast<dart::dynamics::BoxShape>(box->skeleton()->getBodyNode(0)->getShapeNodesWith<dart::dynamics::VisualAspect>()[0]->getShape())->getSize();
        BOOST_CHECK(size(0) == 1. && size(1) == 1. && size(2) == 1.);
        // check if free (as requested by default)
        BOOST_CHECK(box->free());
        // check position
        for (size_t i = 0; i < 6; i++) {
            double val = 0.;
            if (i == 3)
                val = 2.;
            BOOST_CHECK(box->skeleton()->getPosition(i) == val);
        }
        // check name
        BOOST_CHECK(box->name() == "box");
        // check if fixed works
        auto box2 = Robot::create_box(Eigen::Vector3d(1., 1., 1.), pose, "fixed");
        BOOST_REQUIRE(box2);
        // check if fixed (as requested)
        BOOST_CHECK(box2->fixed());
    }
    // ellipsoid creation tests
    {
        Eigen::Vector6d pose;
        pose << 2., 3., 0., 0., 2., 0.;
        auto ellipsoid = Robot::create_ellipsoid(Eigen::Vector3d(1., 2., 3.), pose);
        // check construction
        BOOST_REQUIRE(ellipsoid);
        // check size
        Eigen::Vector3d size = std::static_pointer_cast<dart::dynamics::EllipsoidShape>(ellipsoid->skeleton()->getBodyNode(0)->getShapeNodesWith<dart::dynamics::VisualAspect>()[0]->getShape())->getDiameters();
        BOOST_CHECK(size(0) == 1. && size(1) == 2. && size(2) == 3.);
        // check if free (as requested by default)
        BOOST_CHECK(ellipsoid->free());
        // check position
        for (size_t i = 0; i < 6; i++) {
            double val = 0.;
            if (i == 0 || i == 4)
                val = 2.;
            else if (i == 1)
                val = 3.;
            BOOST_CHECK(ellipsoid->skeleton()->getPosition(i) == val);
        }
        // check name
        BOOST_CHECK(ellipsoid->name() == "ellipsoid");
        // check if fixed works
        auto ellipsoid2 = Robot::create_ellipsoid(Eigen::Vector3d(1., 1., 1.), pose, "fixed");
        BOOST_REQUIRE(ellipsoid2);
        // check if fixed (as requested)
        BOOST_CHECK(ellipsoid2->fixed());
    }
}
