import RobotDART as rd
import numpy as np


robot = rd.Robot("pendulum.urdf")
robot.fix_to_world()
robot.set_position_enforced(True)
robot.set_positions([np.pi])

ctrl = [0.0]
# @SIMPLE_CONTROL_PYTHON@
controller1 = rd.SimpleControl(ctrl)
# add the controller to the robot, with a default weight of 1.0
robot.add_controller(controller1)
# @SIMPLE_CONTROL_PYTHON_END@
ctrl = [-1.0]
controller2 = rd.SimpleControl(ctrl)
# add the controller to the robot, with a weight of 5.0
robot.add_controller(controller2, 5.)
# initialize the simulation with a default timestep of 0.015s
simu = rd.RobotDARTSimu()

simu.set_graphics(rd.gui.Graphics())

simu.add_robot(robot)
# the dimensions of the pendulum to calculate the end effector position
size = [0.0402, 0.05, 1]
print((robot.body_pose("pendulum_link_1").multiply(size)).transpose())
simu.run(2.5)
print((robot.body_pose("pendulum_link_1").multiply(size)).transpose())
ctrl = [2.5]
controller1.set_parameters(ctrl)
simu.run(2.5)
print((robot.body_pose("pendulum_link_1").multiply(size)).transpose())

# reset the positions, velocities, and accelerations of the robot, clear internal and external forces, and also commands
robot.reset()
