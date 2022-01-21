#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE test_control

#include <boost/filesystem.hpp>
#include <boost/test/unit_test.hpp>

#include <robot_dart/control/robot_control.hpp>
#include <robot_dart/robot.hpp>

#include <robot_dart/control/pd_control.hpp>
#include <robot_dart/control/simple_control.hpp>
#include <robot_dart/utils.hpp>

using namespace robot_dart;

BOOST_AUTO_TEST_CASE(test_pd_control)
{
    // default constructor
    auto pd_control = std::make_shared<control::PDControl>();
    // this controller should not be active
    BOOST_CHECK(!pd_control->active());
    // try to initialize
    pd_control->init();
    // it should still not be active
    BOOST_CHECK(!pd_control->active());

    // dummy parameters
    Eigen::VectorXd ctrl(4);
    ctrl << 1., 2., 3., 4.;
    pd_control = std::make_shared<control::PDControl>(ctrl);
    // this controller should not be active
    BOOST_CHECK(!pd_control->active());
    // check that the parameters are properly passed
    for (int i = 0; i < ctrl.size(); i++) {
        BOOST_CHECK(ctrl[i] == pd_control->parameters()[i]);
    }
    // try to initialize
    pd_control->init();
    // it should still not be active
    BOOST_CHECK(!pd_control->active());

    // load a robot
    auto pendulum = std::make_shared<Robot>(std::string(ROBOT_DART_BUILD_DIR) + "/utheque/pendulum.urdf");
    BOOST_REQUIRE(pendulum);

    // set proper parameters
    ctrl = Eigen::VectorXd(1);
    ctrl << 1.;
    pd_control->set_parameters(ctrl);
    // it should still not be active
    BOOST_CHECK(!pd_control->active());
    // set robot
    pd_control->set_robot(pendulum);
    pd_control->init();
    // now it should be active
    BOOST_CHECK(pd_control->active());

    // change pd values
    pd_control->set_pd(10.0, 20.0);
    Eigen::VectorXd p, d;
    std::tie(p, d) = pd_control->pd();
    // check that they are properly set
    BOOST_CHECK(p(0) == 10.0);
    BOOST_CHECK(d(0) == 20.0);

    // change weight
    pd_control->set_weight(30.0);
    BOOST_CHECK(pd_control->weight() == 30.0);
}

BOOST_AUTO_TEST_CASE(test_simple_control)
{
    // default constructor
    auto simple_control = std::make_shared<control::SimpleControl>();
    // this controller should not be active
    BOOST_CHECK(!simple_control->active());
    // try to initialize
    simple_control->init();
    // it should still not be active
    BOOST_CHECK(!simple_control->active());

    // dummy parameters
    Eigen::VectorXd ctrl(4);
    ctrl << 1., 2., 3., 4.;
    simple_control = std::make_shared<control::SimpleControl>(ctrl);
    // this controller should not be active
    BOOST_CHECK(!simple_control->active());
    // check that the parameters are properly passed
    for (int i = 0; i < ctrl.size(); i++) {
        BOOST_CHECK(ctrl[i] == simple_control->parameters()[i]);
    }
    // try to initialize
    simple_control->init();
    // it should still not be active
    BOOST_CHECK(!simple_control->active());

    // load a robot
    auto pendulum = std::make_shared<Robot>(std::string(ROBOT_DART_BUILD_DIR) + "/utheque/pendulum.urdf");
    BOOST_REQUIRE(pendulum);
    pendulum->fix_to_world();

    // set proper parameters
    ctrl = Eigen::VectorXd(1);
    ctrl << 1.;
    simple_control->set_parameters(ctrl);
    // it should still not be active
    BOOST_CHECK(!simple_control->active());
    // set robot
    simple_control->set_robot(pendulum);
    simple_control->init();
    // now it should be active
    BOOST_CHECK(simple_control->active());

    // change weight
    simple_control->set_weight(30.0);
    BOOST_CHECK(simple_control->weight() == 30.0);

    // check controls
    Eigen::VectorXd commands = simple_control->calculate(0.0);
    for (int i = 0; i < ctrl.size(); i++) {
        BOOST_CHECK(ctrl[i] == commands(i));
    }
}

BOOST_AUTO_TEST_CASE(test_robot_control)
{
    // load a robot
    auto pendulum = std::make_shared<Robot>(std::string(ROBOT_DART_BUILD_DIR) + "/utheque/pendulum.urdf");
    BOOST_REQUIRE(pendulum);
    pendulum->fix_to_world();

    Eigen::VectorXd ctrl(1);
    // add a PD controller with weight 1.
    ctrl << 2.;
    pendulum->add_controller(std::make_shared<control::PDControl>(ctrl), 1.0);

    // verify that it is added
    BOOST_CHECK(pendulum->num_controllers() == 1);
    auto pd_control = std::dynamic_pointer_cast<control::PDControl>(pendulum->controller(0));
    BOOST_REQUIRE(pd_control);
    // check that it is valid
    BOOST_CHECK(pd_control->active());
    // check that the weight is 1.
    BOOST_CHECK(pd_control->weight() == 1.0);

    // add a Simple controller with weight 30.
    ctrl << -2.;
    pendulum->add_controller(std::make_shared<control::SimpleControl>(ctrl), 30.0);

    // verify that it is added
    BOOST_CHECK(pendulum->num_controllers() == 2);
    auto simple_control = std::dynamic_pointer_cast<control::SimpleControl>(pendulum->controller(1));
    BOOST_REQUIRE(simple_control);
    // check that it is valid
    BOOST_CHECK(simple_control->active());
    // check that the weight is 30.
    BOOST_CHECK(simple_control->weight() == 30.0);

    // check that commands are combined
    Eigen::VectorXd commands = Eigen::VectorXd::Zero(pendulum->skeleton()->getNumDofs());
    commands += pd_control->weight() * pd_control->calculate(0.0);
    commands += simple_control->weight() * simple_control->calculate(0.0);
    // clamp commands to limits
    for (int i = 0; i < commands.size(); i++) {
        if (commands(i) > pendulum->skeleton()->getForceUpperLimit(i)) {
            commands(i) = pendulum->skeleton()->getForceUpperLimit(i);
        }
        else if (commands(i) < pendulum->skeleton()->getForceLowerLimit(i)) {
            commands(i) = pendulum->skeleton()->getForceLowerLimit(i);
        }
    }

    // reset state of PD controller
    pd_control->init();

    pendulum->update(0.0);
    Eigen::VectorXd robot_commands = pendulum->skeleton()->getCommands();
    for (int i = 0; i < robot_commands.size(); i++) {
        BOOST_CHECK(robot_commands(i) == commands(i));
    }
}