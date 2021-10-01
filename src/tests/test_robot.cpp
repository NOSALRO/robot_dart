#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE test_robot

#include <boost/filesystem.hpp>
#include <boost/test/unit_test.hpp>

#include <robot_dart/robot.hpp>
#include <robot_dart/utils.hpp>
#include <robot_dart/utils_headers_dart_dynamics.hpp>

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
    auto pendulum = std::make_shared<Robot>(std::string(ROBOT_DART_BUILD_DIR) + "/robots/pendulum.urdf");
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
}

BOOST_AUTO_TEST_CASE(test_dof_maps)
{
    auto pendulum = std::make_shared<Robot>(std::string(ROBOT_DART_BUILD_DIR) + "/robots/pendulum.urdf");
    BOOST_REQUIRE(pendulum);

    // check dofs
    auto names = pendulum->dof_names();
    BOOST_CHECK(names.size() == 7);

    // check if joint/dof map is updated
    std::vector<std::string> name = {"pendulum_joint_1"};
    pendulum->set_positions(make_vector({0.1}), name);

    BOOST_CHECK(pendulum->positions(name)[0] == 0.1);
}

BOOST_AUTO_TEST_CASE(test_fix_free)
{
    auto pendulum = std::make_shared<Robot>(std::string(ROBOT_DART_BUILD_DIR) + "/robots/pendulum.urdf");
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

BOOST_AUTO_TEST_CASE(test_actuators_and_dofs)
{
    auto pexod = std::make_shared<Robot>(std::string(ROBOT_DART_BUILD_DIR) + "/robots/pexod.urdf");
    BOOST_REQUIRE(pexod);
    // fix to world
    pexod->fix_to_world();

    // Actuator types
    {
        // set different actuator type to all DOFs
        pexod->set_actuator_types("passive", {}, true, true);

        // check if the change is applied
        auto types = pexod->actuator_types();
        for (size_t i = 0; i < types.size(); i++) {
            if (pexod->skeleton()->getJoint(i)->getNumDofs() > 0)
                BOOST_CHECK(types[i] == "passive");
        }

        // check simple dof setting
        pexod->set_actuator_type("torque", pexod->joint_names()[0]);
        BOOST_CHECK(pexod->actuator_type(pexod->joint_names()[0]) == "torque");
    }

    // Limits enforcement
    {
        // enforce position limits to all DOFs
        pexod->set_position_enforced(true);

        // check if the change is applied
        auto enforced = pexod->position_enforced();
        for (size_t i = 0; i < enforced.size(); i++) {
            BOOST_CHECK(enforced[i]);
        }

        // disable position limits for specific DOFs
        enforced[0] = false;
        enforced[3] = false;
        pexod->set_position_enforced(enforced);

        // check if the change is applied
        enforced.clear();
        enforced = pexod->position_enforced();
        for (size_t i = 0; i < enforced.size(); i++) {
            bool pos = true;
            if (i == 0 || i == 3)
                pos = false;
            BOOST_CHECK(enforced[i] == pos);
        }

        // check simple dof setting
        auto dof_name = pexod->dof_name(5); // get DoF name of DoFu with index = 5
        pexod->set_position_enforced(false, {dof_name});
        BOOST_CHECK(!pexod->position_enforced({dof_name})[0]);
    }

    // Damping coefficients
    {
        // set different damping coefficients to all DOFs
        pexod->set_damping_coeffs(2.0);

        // check if the change is applied
        auto damps = pexod->damping_coeffs();
        for (size_t i = 0; i < damps.size(); i++) {
            BOOST_CHECK(damps[i] == 2.0);
        }

        // disable position limits for specific DOFs
        damps[0] = 1.;
        damps[3] = 0.25;
        pexod->set_damping_coeffs(damps);

        // check if the change is applied
        damps.clear();
        damps = pexod->damping_coeffs();
        for (size_t i = 0; i < damps.size(); i++) {
            double d = 2.0;
            if (i == 0)
                d = 1.0;
            else if (i == 3)
                d = 0.25;
            BOOST_CHECK(damps[i] == d);
        }

        // check simple dof setting
        auto dof_name = pexod->dof_name(0); // get DoF name of DoF with index = 0
        pexod->set_damping_coeffs(10.0, {dof_name});
        BOOST_CHECK(pexod->damping_coeffs({dof_name})[0] == 10.0);
    }

    // Coulomb coefficients
    {
        // set different damping coefficients to all DOFs
        pexod->set_coulomb_coeffs(2.0);

        // check if the change is applied
        auto cfrictions = pexod->coulomb_coeffs();
        for (size_t i = 0; i < cfrictions.size(); i++) {
            BOOST_CHECK(cfrictions[i] == 2.0);
        }

        // disable position limits for specific DOFs
        cfrictions[0] = 1.;
        cfrictions[3] = 0.25;
        pexod->set_coulomb_coeffs(cfrictions);

        // check if the change is applied
        cfrictions.clear();
        cfrictions = pexod->coulomb_coeffs();
        for (size_t i = 0; i < cfrictions.size(); i++) {
            double d = 2.0;
            if (i == 0)
                d = 1.0;
            else if (i == 3)
                d = 0.25;
            BOOST_CHECK(cfrictions[i] == d);
        }

        // check simple dof setting
        auto dof_name = pexod->dof_name(3); // get DoF name of DoF with index = 3u
        pexod->set_coulomb_coeffs(10.0, {dof_name});
        BOOST_CHECK(pexod->coulomb_coeffs({dof_name})[0] == 10.0);
    }

    // Spring stiffness
    {
        // set different damping coefficients to all DOFs
        pexod->set_spring_stiffnesses(2.0);

        // check if the change is applied
        auto stiffnesses = pexod->spring_stiffnesses();
        for (size_t i = 0; i < stiffnesses.size(); i++) {
            BOOST_CHECK(stiffnesses[i] == 2.0);
        }

        // disable position limits for specific DOFs
        stiffnesses[0] = 1.;
        stiffnesses[3] = 0.25;
        pexod->set_spring_stiffnesses(stiffnesses);

        // check if the change is applied
        stiffnesses.clear();
        stiffnesses = pexod->spring_stiffnesses();
        for (size_t i = 0; i < stiffnesses.size(); i++) {
            double d = 2.0;
            if (i == 0)
                d = 1.0;
            else if (i == 3)
                d = 0.25;
            BOOST_CHECK(stiffnesses[i] == d);
        }

        // check simple dof setting
        auto dof_name = pexod->dof_name(3); // get DoF name of DoF with index = 3u
        pexod->set_spring_stiffnesses(10.0, {dof_name});
        BOOST_CHECK(pexod->spring_stiffnesses({dof_name})[0] == 10.0);
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
