#include <iostream>
#include <robot_dart/robot_dart_simu.hpp>

#include <robot_dart/control/hexa_control.hpp>
#include <robot_dart/control/pd_control.hpp>

#ifdef GRAPHIC
#include <robot_dart/gui/magnum/graphics.hpp>
#endif

#include <dart/collision/bullet/BulletCollisionDetector.hpp>
#include <dart/constraint/ConstraintSolver.hpp>

#include <dart/dynamics/BodyNode.hpp>
#include <dart/dynamics/BoxShape.hpp>
#include <dart/dynamics/CapsuleShape.hpp>
#include <dart/dynamics/ConeShape.hpp>
#include <dart/dynamics/CylinderShape.hpp>
#include <dart/dynamics/FreeJoint.hpp>
#include <dart/dynamics/Skeleton.hpp>
#include <dart/dynamics/WeldJoint.hpp>

inline dart::dynamics::SkeletonPtr createCone(const std::string& name = "cone", const Eigen::Vector3d& color = Eigen::Vector3d(0.8, 0., 0.))
{
    /* The size of our box */
    const double box_size = 0.17;

    /* Calculate the mass of the box */
    const double box_density = 260; // kg/m^3
    const double box_mass = box_density * std::pow(box_size, 3.);
    /* Create a Skeleton with the given name */
    dart::dynamics::SkeletonPtr box = dart::dynamics::Skeleton::create(name);

    /* Create a body for the box */
    dart::dynamics::BodyNodePtr body = box->createJointAndBodyNodePair<dart::dynamics::FreeJoint>(nullptr).second;

    /* Create a shape for the box */
    std::shared_ptr<dart::dynamics::ConeShape> box_shape(
        new dart::dynamics::ConeShape(box_size / 2., box_size));
    auto shapeNode = body->createShapeNodeWith<dart::dynamics::VisualAspect, dart::dynamics::CollisionAspect, dart::dynamics::DynamicsAspect>(box_shape);
    shapeNode->getVisualAspect()->setColor(color);

    /* Set up inertia for the box */
    dart::dynamics::Inertia inertia;
    inertia.setMass(box_mass);
    inertia.setMoment(box_shape->computeInertia(box_mass));
    body->setInertia(inertia);

    /* Setup the center of the box properly */
    box->getDof("Joint_pos_z")->setPosition(box_size / 2.0);

    return box;
}

int main()
{
    std::srand(std::time(NULL));
    auto global_robot = std::make_shared<robot_dart::Robot>("res/models/pexod.urdf");

    global_robot->set_position_enforced(true);

    global_robot->set_actuator_types(dart::dynamics::Joint::SERVO);
    global_robot->skeleton()->enableSelfCollisionCheck();

    auto g_robot = global_robot->clone();
    g_robot->skeleton()->setPosition(5, 0.15);

    // std::vector<double> ctrl(18, 0.);
    // g_robot->add_controller(std::make_shared<robot_dart::SimpleControl>());
    std::vector<double> ctrl = {1, 0, 0.5, 0.25, 0.25, 0.5, 1, 0.5, 0.5, 0.25, 0.75, 0.5, 1, 0, 0.5, 0.25, 0.25, 0.5, 1, 0, 0.5, 0.25, 0.75, 0.5, 1, 0.5, 0.5, 0.25, 0.25, 0.5, 1, 0, 0.5, 0.25, 0.75, 0.5};

    double ctrl_dt = 0.015;
    g_robot->add_controller(std::make_shared<robot_dart::control::HexaControl>(ctrl_dt, ctrl));
    std::static_pointer_cast<robot_dart::control::HexaControl>(g_robot->controller(0))->set_h_params(std::vector<double>(1, ctrl_dt));

    auto skel = createCone();
    skel->setPosition(0, M_PI / 4.);
    skel->setPosition(1, M_PI / 3.);
    // skel->setPosition(3, 0.25);
    skel->setPosition(5, 0.8);
    auto skel_robot = std::make_shared<robot_dart::Robot>(skel, skel->getName());
    // Eigen::Vector6d pose;
    // pose << 0., 0., 0., 0.2, 0.5, 0.5;
    // auto skel_robot = robot_dart::Robot::create_box({0.1, 0.1, 0.1}, pose);

    robot_dart::RobotDARTSimu simu(0.001);
#ifdef GRAPHIC
    simu.set_graphics(std::make_shared<robot_dart::gui::magnum::Graphics>(simu.world(), 1024, 768));
    std::static_pointer_cast<robot_dart::gui::magnum::Graphics>(simu.graphics())->look_at({0.5, 3., 0.75}, {0.5, 0., 0.2});
#endif
    simu.world()->getConstraintSolver()->setCollisionDetector(dart::collision::BulletCollisionDetector::create());
    simu.add_floor();
    simu.add_robot(g_robot);
    simu.add_robot(skel_robot);
    simu.run(6);

    std::cout << g_robot->skeleton()->getPositions().head(6).tail(3).transpose() << std::endl;

    g_robot.reset();
    global_robot.reset();
    skel_robot.reset();
    return 0;
}