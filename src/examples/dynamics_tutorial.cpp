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
              << coriolis << std::endl;
    std::cout << "___________________________" << std::endl;
    std::cout << "Gravity:\n"
              << gravity << std::endl;
    std::cout << "___________________________" << std::endl;
    std::cout << "Coriolis/Gravity:\n"
              << coriolis_gravity << std::endl;
    std::cout << "___________________________" << std::endl;
    std::cout << "Consraint Forces vector:\n"
              << constraint_forces << std::endl;
    std::cout << "___________________________" << std::endl;

    return 0;
}
