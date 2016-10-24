#include <iostream>
#include <cstdlib>
#include <robot_dart/robot_dart_simu.hpp>
#include <robot_dart/force_control.hpp>

// void setGeometry(const dart::dynamics::BodyNodePtr& bn)
// {
//     // Create a BoxShape to be used for both visualization and collision checking
//     std::shared_ptr<dart::dynamics::BoxShape> box(new dart::dynamics::BoxShape(
//         Eigen::Vector3d(1.0, 0.2, 0.2)));
//
//     // Create a shape node for visualization and collision checking
//     auto shapeNode
//         = bn->createShapeNodeWith<dart::dynamics::VisualAspect, dart::dynamics::CollisionAspect, dart::dynamics::DynamicsAspect>(box);
//     shapeNode->getVisualAspect()->setColor(dart::Color::Blue());
//
//     // Set the location of the shape node
//     Eigen::Isometry3d box_tf(Eigen::Isometry3d::Identity());
//     Eigen::Vector3d center = Eigen::Vector3d(0, 0, 1.0 / 2.0);
//     box_tf.translation() = center;
//     shapeNode->setRelativeTransform(box_tf);
//
//     // Move the center of mass to the center of the object
//     bn->setLocalCOM(center);
// }
//
// dart::dynamics::BodyNode* makeRootBody(const dart::dynamics::SkeletonPtr& pendulum, const std::string& name)
// {
//     // dart::dynamics::BallJoint::Properties properties;
//     // properties.mName = name + "_joint";
//     // properties.mRestPositions = Eigen::Vector3d::Constant(0.0);
//     // properties.mSpringStiffnesses = Eigen::Vector3d::Constant(0.0);
//     // properties.mDampingCoefficients = Eigen::Vector3d::Constant(5.0);
//     //
//     // dart::dynamics::BodyNodePtr bn = pendulum->createJointAndBodyNodePair<dart::dynamics::BallJoint>(
//     //                                                nullptr, properties, dart::dynamics::BodyNode::AspectProperties(name)).second;
//     // Set up the properties for the Joint
//     dart::dynamics::RevoluteJoint::Properties properties;
//     properties.mName = name + "_joint";
//     properties.mAxis = Eigen::Vector3d::UnitY();
//     properties.mT_ParentBodyToJoint.translation() = Eigen::Vector3d(0, 0, 1.0);
//     properties.mRestPosition = 0.0;
//     properties.mSpringStiffness = 0.0;
//     properties.mDampingCoefficient = 5.0;
//
//     // Create a new BodyNode, attached to its parent by a RevoluteJoint
//     dart::dynamics::BodyNodePtr bn = pendulum->createJointAndBodyNodePair<dart::dynamics::RevoluteJoint>(
//                                                    nullptr, properties, dart::dynamics::BodyNode::AspectProperties(name)).second;
//
//     // Set the geometry of the Body
//     setGeometry(bn);
//
//     return bn;
// }
//
// dart::dynamics::BodyNode* addBody(const dart::dynamics::SkeletonPtr& pendulum, dart::dynamics::BodyNode* parent,
//     const std::string& name)
// {
//     // Set up the properties for the Joint
//     dart::dynamics::RevoluteJoint::Properties properties;
//     properties.mName = name + "_joint";
//     properties.mAxis = Eigen::Vector3d::UnitY();
//     properties.mT_ParentBodyToJoint.translation() = Eigen::Vector3d(0, 0, 1.0);
//     properties.mRestPosition = 0.0;
//     properties.mSpringStiffness = 0.0;
//     properties.mDampingCoefficient = 5.0;
//
//     // Create a new BodyNode, attached to its parent by a RevoluteJoint
//     dart::dynamics::BodyNodePtr bn = pendulum->createJointAndBodyNodePair<dart::dynamics::RevoluteJoint>(
//                                                    parent, properties, dart::dynamics::BodyNode::AspectProperties(name)).second;
//
//     // Set the geometry of the Body
//     setGeometry(bn);
//
//     return bn;
// }

int main()
{
    // std::vector<robot_dart::RobotDamage> brk = {};
    // Examples of damages
    // TODO

    auto global_robot = std::make_shared<robot_dart::Robot>("nao.urdf");
    auto g_robot = global_robot->clone();
    g_robot->set_position_enforced(true);
    g_robot->skeleton()->setPosition(5, 5.0);

    std::vector<double> ctrl;
    ctrl = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    robot_dart::RobotDARTSimu<robot_dart::robot_control<robot_dart::ForceControl>> simu(ctrl, g_robot);
    simu.add_floor();
#ifdef GRAPHIC
    simu.free_camera();
#endif
    simu.run(10000);

    global_robot.reset();
    g_robot.reset();
    return 0;
}
