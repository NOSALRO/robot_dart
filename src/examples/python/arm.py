import RobotDART as rd

# @SIMPLE_ARM_PYTHON@
robot = rd.Arm()
# @SIMPLE_ARM_PYTHON_END@
robot.set_actuator_types("velocity")

ctrl = [0.0, 1.0, -1.5, 1.0]
controller = rd.PDControl(ctrl)
robot.add_controller(controller)

simu = rd.RobotDARTSimu()

simu.set_graphics(rd.gui.Graphics())

simu.add_robot(robot)

print(robot.body_pose("arm_link_5").translation().transpose())

simu.run(2.5)
print(robot.body_pose("arm_link_5").translation().transpose())

ctrl = [0.0, -1.0, 1.5, -1.0]

controller.set_parameters(ctrl)
controller.set_pd(20.0, 0.)
simu.run(2.5)
print(robot.body_pose("arm_link_5").translation().transpose())
