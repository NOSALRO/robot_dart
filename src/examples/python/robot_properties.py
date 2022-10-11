import RobotDART as rd
import numpy as np


def print_coeffs(dof_names, robot):
    print("Joint Name\t Damping Coefficients\t Coulomb Frictions\t Spring Stiffness")
    for i in range(0, len(dof_names)):
        print(dof_names[i], "\t ", robot.damping_coeffs()[i], "\t\t\t ", robot.coulomb_coeffs()[
              i], "\t\t\t ", robot.spring_stiffnesses()[i], "\t\t\t ")


def print_limits(dof_names, robot):
    print("Joint Name\t Actuator Type\t\t Position Limits\t Velocity Limits\t Acceleration Limits\t Force Limits\t")
    for i in range(len(dof_names)):
        print(dof_names[i],  "\t ",  robot.actuator_types()[i],  "\t\t (",  robot.position_lower_limits(dof_names)[i],  ", ",  robot.position_upper_limits(dof_names)[i],  ")",  "\t (",  robot.velocity_lower_limits(dof_names)[i],  ", ",  robot.velocity_upper_limits(
            dof_names)[i],  ")",  "\t\t (",  robot.acceleration_lower_limits(dof_names)[i],  ", ",  robot.acceleration_upper_limits(dof_names)[i],  ")",  "\t\t (",  robot.force_lower_limits(dof_names)[i],  ", ",  robot.force_upper_limits(dof_names)[i],  ")")


simu = rd.RobotDARTSimu(0.001)
simu.set_collision_detector("fcl")

robot = rd.Iiwa()
simu.add_robot(robot)

dof_names = robot.dof_names()

print("Before: ")
print_limits(dof_names, robot)

# @SET_ACTUATOR_PYTHON@
# Set all DoFs to same actuator
# actuator types can be "servo", "torque", "velocity", "passive", "locked", "mimic"
robot.set_actuator_types("servo")
# You can also set actuator types separately
robot.set_actuator_type("torque", "iiwa_joint_5")
# @SET_ACTUATOR_PYTHON_END@

# @POSITIONS_ENFORCED_PYTHON@
# Εnforce joint position and velocity limits
robot.set_position_enforced(True)
# @POSITIONS_ENFORCED_PYTHON_END@

# @MODIFY_LIMITS_PYTHON@
# Modify Position Limits
pos_upper_lims = np.array([2.096, 2.096, 2.096, 2.096, 2.096, 2.096, 2.096])
robot.set_position_upper_limits(pos_upper_lims, dof_names)
robot.set_position_lower_limits(-1*pos_upper_lims, dof_names)

# Modify Velocity Limits
vel_upper_lims = np.array([1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5])

robot.set_velocity_upper_limits(vel_upper_lims, dof_names)
robot.set_velocity_lower_limits(-1*vel_upper_lims, dof_names)

# Modify Force Limits
force_upper_lims = np.array([150, 150, 150, 150, 150, 150, 150])
robot.set_force_upper_limits(force_upper_lims, dof_names)
robot.set_force_lower_limits(-1*force_upper_lims, dof_names)

# Modify Acceleration Limits
acc_upper_lims = np.array([1500, 1500, 1500, 1500, 1500, 1500, 1500])
robot.set_acceleration_upper_limits(acc_upper_lims, dof_names)
robot.set_acceleration_lower_limits(-1*acc_upper_lims, dof_names)
# @MODIFY_LIMITS_PYTHON_END@

print("After: ")
print_limits(dof_names, robot)


print("Before: ")
print_coeffs(dof_names, robot)

# @MODIFY_COEFFS_PYTHON@
# Modify Damping Coefficients
damps = [0.4, 0.4, 0.4, 0.4, 0.4, 0.4, 0.4]
robot.set_damping_coeffs(damps, dof_names)

# Modify Coulomb Frictions
cfrictions = [0.001, 0.001, 0.001, 0.001, 0.001, 0.001, 0.001]
robot.set_coulomb_coeffs(cfrictions, dof_names)

# Modify  Spring Stiffness
stiffnesses = [0.001, 0.001, 0.001, 0.001, 0.001, 0.001, 0.001]
robot.set_spring_stiffnesses(stiffnesses, dof_names)
# @MODIFY_COEFFS_PYTHON_END@
print("After: ")
print_coeffs(dof_names, robot)
