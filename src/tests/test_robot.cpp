#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE test_robot

#include <boost/filesystem.hpp>
#include <boost/test/unit_test.hpp>

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
    BOOST_CHECK(pendulum);
    // well-defined skeleton
    dart::dynamics::SkeletonPtr dummy_skel = dart::dynamics::Skeleton::create("dummy");
    auto dummy = std::make_shared<Robot>(dummy_skel, "dummy_robot");
    BOOST_CHECK(dummy);

    // check name given
    BOOST_CHECK(pendulum->name() == "robot");
    BOOST_CHECK(pendulum->name() == pendulum->skeleton()->getName());

    BOOST_CHECK(dummy->name() == "dummy_robot");
    BOOST_CHECK(dummy->name() == dummy->skeleton()->getName());
    BOOST_CHECK(dummy->name() == dummy_skel->getName());

    // TO-DO: Add checks for damages
}