#include <robot_dart/robot_dart_simu.hpp>
#include <robot_dart/robots/franka.hpp>

int main()
{
    robot_dart::RobotDARTSimu simu(0.001);

    simu.set_collision_detector("fcl");
    auto robot = std::make_shared<robot_dart::robots::Franka>();
    robot->set_actuator_types("servo");
    simu.add_robot(robot);

    std::string link_name = "panda_ee";
    // @KINEMATICS@
    // Get Joint Positions(Angles)
    auto joint_positions = robot->positions();
    // Get Joint Velocities
    auto joint_vels = robot->velocities();
    // Get Joint Accelerations
    auto joint_accs = robot->accelerations();
    // Get link_name(str) Transformation matrix with respect to the world frame.
    auto eef_tf = robot->body_pose(link_name);
    // Get translation vector from transformation matrix
    auto eef_pos = eef_tf.translation();
    // Get rotation matrix from tranformation matrix
    auto eef_rot = eef_tf.rotation();
    // Get link_name 6d pose vector [logmap(eef_tf.linear()), eef_tf.translation()]
    auto eef_pose_vec = robot->body_pose_vec(link_name);
    // Get link_name 6d velocity vector [angular, cartesian]
    auto eef_vel = robot->body_velocity(link_name);
    // Get link_name 6d acceleration vector [angular, cartesian]
    auto eef_acc = robot->body_acceleration(link_name);
    // Jacobian targeting the origin of link_name(str)
    auto jacobian = robot->jacobian(link_name);
    // Jacobian time derivative
    auto jacobian_deriv = robot->jacobian_deriv(link_name);
    // Center of Mass Jacobian
    auto com_jacobian = robot->com_jacobian(robot->dof_names());
    // Center of Mass Jacobian Time Derivative
    auto com_jacobian_deriv = robot->com_jacobian_deriv(robot->dof_names());
    // @KINEMATICS_END@
    // @DYNAMICS@
    // Get Joint Forces
    auto joint_forces = robot->forces();
    // Get link's mass
    auto eef_mass = robot->body_mass(link_name);
    // Mass Matrix of robot
    auto mass_matrix = robot->mass_matrix();
    // Inverse of Mass Matrix
    auto inv_mass_matrix = robot->inv_mass_matrix();
    // Augmented Mass matrix
    auto aug_mass_matrix = robot->aug_mass_matrix();
    // Inverse of Augmented Mass matrix
    auto inv_aug_mass_matrix = robot->inv_aug_mass_matrix();
    // Coriolis Force vector
    auto coriolis = robot->coriolis_forces();
    // Gravity Force vector
    auto gravity = robot->gravity_forces();
    // Combined vector of Coriolis Force and Gravity Force
    auto coriolis_gravity = robot->coriolis_gravity_forces();
    // Constraint Force Vector
    auto constraint_forces = robot->constraint_forces(robot->dof_names());
    // @DYNAMICS_END@
    std::cout << "Joint Positions:\n"
              << joint_positions.transpose() << std::endl;
    std::cout << "___________________________" << std::endl;
    std::cout << "Joint Velocities:\n"
              << joint_vels.transpose() << std::endl;
    std::cout << "___________________________" << std::endl;
    std::cout << "Joint Accelerations:\n"
              << joint_accs.transpose() << std::endl;
    std::cout << "___________________________" << std::endl;
    std::cout << "End effector Transformation matrix:\n"
              << eef_tf.matrix() << std::endl;
    std::cout << "___________________________" << std::endl;
    std::cout << "End effector Cartesian Position:\n"
              << eef_pos.transpose() << std::endl;
    std::cout << "___________________________" << std::endl;
    std::cout << "End effector Rotation Matrix:\n"
              << eef_rot << std::endl;
    std::cout << "___________________________" << std::endl;
    std::cout << "End effector pose vector:\n"
              << eef_pose_vec.transpose() << std::endl;
    std::cout << "___________________________" << std::endl;
    std::cout << "End effector Velocity:\n"
              << eef_vel.transpose() << std::endl;
    std::cout << "___________________________" << std::endl;
    std::cout << "End effector Acceleration:\n"
              << eef_acc.transpose() << std::endl;
    std::cout << "___________________________" << std::endl;
    std::cout << "Jacobian:\n"
              << jacobian << std::endl;
    std::cout << "___________________________" << std::endl;
    std::cout << "Jacobian Derivative:\n"
              << jacobian_deriv << std::endl;
    std::cout << "___________________________" << std::endl;
    std::cout << "Center of Mass Jacobian:\n"
              << com_jacobian << std::endl;
    std::cout << "___________________________" << std::endl;
    std::cout << "Center of Mass Jacobian Derivative:\n"
              << com_jacobian_deriv << std::endl;
    std::cout << "___________________________" << std::endl;
    std::cout << "Joint Forces:\n"
              << joint_forces.transpose() << std::endl;
    std::cout << "___________________________" << std::endl;
    std::cout << "End effector Mass:\n"
              << eef_mass << std::endl;
    std::cout << "___________________________" << std::endl;
    std::cout << "Mass Matrix:\n"
              << mass_matrix << std::endl;
    std::cout << "___________________________" << std::endl;
    std::cout << "Inverse Mass Matrix:\n"
              << inv_mass_matrix << std::endl;
    std::cout << "___________________________" << std::endl;
    std::cout << "Augmented Mass Matrix:\n"
              << aug_mass_matrix << std::endl;
    std::cout << "___________________________" << std::endl;
    std::cout << "Inverse Augmented Mass Matrix:\n"
              << inv_aug_mass_matrix << std::endl;
    std::cout << "___________________________" << std::endl;
    std::cout << "Coriolis:\n"
              << coriolis.transpose() << std::endl;
    std::cout << "___________________________" << std::endl;
    std::cout << "Gravity:\n"
              << gravity.transpose() << std::endl;
    std::cout << "___________________________" << std::endl;
    std::cout << "Coriolis/Gravity:\n"
              << coriolis_gravity.transpose() << std::endl;
    std::cout << "___________________________" << std::endl;
    std::cout << "Constraint Forces vector:\n"
              << constraint_forces.transpose() << std::endl;
    std::cout << "___________________________" << std::endl;

    return 0;
}
