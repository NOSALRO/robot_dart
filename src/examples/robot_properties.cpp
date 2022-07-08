#include <robot_dart/robot_dart_simu.hpp>
#include <robot_dart/robots/iiwa.hpp>

int main()
{
    robot_dart::RobotDARTSimu simu(0.001);

    simu.set_collision_detector("fcl");
    auto robot = std::make_shared<robot_dart::robots::Iiwa>();
    simu.add_robot(robot);
    std::vector<std::string> dof_names = robot->dof_names();
    std::cout << "Before: " << std::endl;
    std::cout << "Joint Name\t Actuator Type\t Position Limits\t Velocity Limits\t Acceleration Limits\t Force Limits\t" << std::endl;
    // let's print joints info
    for (size_t i = 0; i < dof_names.size(); i++) {
        std::cout << dof_names[i] << "\t " << robot->actuator_types()[i] << "\t\t (" << robot->position_lower_limits(dof_names)[i] << ", " << robot->position_upper_limits(dof_names)[i] << ")"
                  << "\t (" << robot->velocity_lower_limits(dof_names)[i] << ", " << robot->velocity_upper_limits(dof_names)[i] << ")"
                  << "\t\t (" << robot->acceleration_lower_limits(dof_names)[i] << ", " << robot->acceleration_upper_limits(dof_names)[i] << ")"
                  << "\t\t (" << robot->force_lower_limits(dof_names)[i] << ", " << robot->force_upper_limits(dof_names)[i] << ")" << std::endl;
    }
    // @SET_ACTUATOR@
    // Set all DoFs to same actuator
    robot->set_actuator_types("servo"); // actuator types can be "servo", "torque", "velocity", "passive", "locked", "mimic"
    // You can also set actuator types separately
    robot->set_actuator_type("torque", "iiwa_joint_5");
    // @SET_ACTUATOR_END@

    // @POSITIONS_ENFORCED@
    // Εnforce joint position and velocity limits
    robot->set_position_enforced(true);
    // @POSITIONS_ENFORCED_END@

    // @MODIFY_LIMITS@
    // Modify Position Limits
    Eigen::RowVectorXd pos_upper_lims(7);
    pos_upper_lims << 2.096, 2.096, 2.096, 2.096, 2.096, 2.096, 2.096;
    robot->set_position_upper_limits(pos_upper_lims, dof_names);
    robot->set_position_lower_limits(-pos_upper_lims, dof_names);

    // Modify Velocity Limits
    Eigen::RowVectorXd vel_upper_lims(7);
    vel_upper_lims << 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5;
    robot->set_velocity_upper_limits(vel_upper_lims, dof_names);
    robot->set_velocity_lower_limits(-vel_upper_lims, dof_names);

    // Modify Force Limits
    Eigen::RowVectorXd force_upper_lims(7);
    force_upper_lims << 150, 150, 150, 150, 150, 150, 150;
    robot->set_force_upper_limits(force_upper_lims, dof_names);
    robot->set_force_lower_limits(-force_upper_lims, dof_names);

    // Modify Acceleration Limits
    Eigen::RowVectorXd acc_upper_lims(7);
    acc_upper_lims << 1500, 1500, 1500, 1500, 1500, 1500, 1500;
    robot->set_acceleration_upper_limits(acc_upper_lims, dof_names);
    robot->set_acceleration_lower_limits(-acc_upper_lims, dof_names);
    // @MODIFY_LIMITS_END@
    std::cout << "After: " << std::endl;
    std::cout << "Joint Name\t Actuator Type\t Position Limits\t Velocity Limits\t Acceleration Limits\t Force Limits\t" << std::endl;
    // let's print joints info
    for (size_t i = 0; i < dof_names.size(); i++) {
        std::cout << dof_names[i] << "\t " << robot->actuator_types()[i] << "\t\t (" << robot->position_lower_limits(dof_names)[i] << ", " << robot->position_upper_limits(dof_names)[i] << ")"
                  << "\t (" << robot->velocity_lower_limits(dof_names)[i] << ", " << robot->velocity_upper_limits(dof_names)[i] << ")"
                  << "\t\t (" << robot->acceleration_lower_limits(dof_names)[i] << ", " << robot->acceleration_upper_limits(dof_names)[i] << ")"
                  << "\t\t (" << robot->force_lower_limits(dof_names)[i] << ", " << robot->force_upper_limits(dof_names)[i] << ")" << std::endl;
    }
    return 0;
}
