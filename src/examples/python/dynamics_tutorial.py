from re import L
import RobotDART as rd

simu = rd.RobotDARTSimu(0.001)
simu.set_collision_detector("fcl")
robot = rd.Franka()
robot.set_actuator_types("servo")
simu.add_robot(robot)

link_name = "panda_ee"

# @KINEMATICS_PYTHON@
# Get Joint Positions(Angles)
joint_positions = robot.positions()

# Get Joint Velocities
joint_vels = robot.velocities()

# Get Joint Accelerations
joint_accs = robot.accelerations()

# Get link_name(str) Transformation matrix with respect to the world frame.
eef_tf = robot.body_pose(link_name)

# Get translation vector from transformation matrix
eef_pos = eef_tf.translation()

# Get rotation matrix from tranformation matrix
eef_rot = eef_tf.rotation()

# Get link_name 6d pose vector [logmap(eef_tf.linear()), eef_tf.translation()]
eef_pose_vec = robot.body_pose_vec(link_name)

# Get link_name 6d velocity vector [angular, cartesian]
eef_vel = robot.body_velocity(link_name)

# Get link_name 6d acceleration vector [angular, cartesian]
eef_acc = robot.body_acceleration(link_name)

# Jacobian targeting the origin of link_name(str)
jacobian = robot.jacobian(link_name)

# Jacobian time derivative
jacobian_deriv = robot.jacobian_deriv(link_name)

# Center of Mass Jacobian
com_jacobian = robot.com_jacobian()

# Center of Mass Jacobian Time Derivative
com_jacobian_deriv = robot.com_jacobian_deriv()
# @KINEMATICS_PYTHON_END@

# @DYNAMICS_PYTHON@
# Get Joint Forces
joint_forces = robot.forces()

# Get link's mass
eef_mass = robot.body_mass(link_name)

# Mass Matrix of robot
mass_matrix = robot.mass_matrix()

# Inverse of Mass Matrix
inv_mass_matrix = robot.inv_mass_matrix()

# Augmented Mass matrix
aug_mass_matrix = robot.aug_mass_matrix()

# Inverse of Augmented Mass matrix
inv_aug_mass_matrix = robot.inv_aug_mass_matrix()

# Coriolis Force vector
coriolis = robot.coriolis_forces()

# Gravity Force vector
gravity = robot.gravity_forces()

# Combined vector of Coriolis Force and Gravity Force
coriolis_gravity = robot.coriolis_gravity_forces()

# NOT IMPLEMENTED
# # Constraint Force Vector
# constraint_forces = robot.constraint_forces()

# @DYNAMICS_PYTHON_END@

print("Joint Positions:\n", joint_positions.transpose())
print("___________________________")
print("Joint Velocities:\n", joint_vels.transpose())
print("___________________________")
print("Joint Accelerations:\n", joint_accs.transpose())
print("___________________________")
print("End effector Transformation matrix:\n", eef_tf.matrix())
print("___________________________")
print("End effector Cartesian Position:\n", eef_pos.transpose())
print("___________________________")
print("End effector Rotation Matrix:\n", eef_rot)
print("___________________________")
print("End effector pose vector:\n", eef_pose_vec.transpose())
print("___________________________")
print("End effector Velocity:\n", eef_vel.transpose())
print("___________________________")
print("End effector Acceleration:\n", eef_acc.transpose())
print("___________________________")
print("Jacobian:\n", jacobian)
print("___________________________")
print("Jacobian Derivative:\n", jacobian_deriv)
print("___________________________")
print("Center of Mass Jacobian:\n", com_jacobian)
print("___________________________")
print("Center of Mass Jacobian Derivative:\n", com_jacobian_deriv)
print("___________________________")
print("Joint Forces:\n", joint_forces.transpose())
print("___________________________")
print("End effector Mass:\n", eef_mass)
print("___________________________")
print("Mass Matrix:\n", mass_matrix)
print("___________________________")
print("Inverse Mass Matrix:\n", inv_mass_matrix)
print("___________________________")
print("Augmented Mass Matrix:\n", aug_mass_matrix)
print("___________________________")
print("Inverse Augmented Mass Matrix:\n", inv_aug_mass_matrix)
print("___________________________")
print("Coriolis:\n", coriolis.transpose())
print("___________________________")
print("Gravity:\n", gravity.transpose())
print("___________________________")
print("Coriolis/Gravity:\n", coriolis_gravity.transpose())
print("___________________________")
