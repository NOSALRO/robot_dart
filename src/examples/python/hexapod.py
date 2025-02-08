import RobotDART as rd

# @HEXAPOD_PYTHON@
robot = rd.Hexapod()
# @HEXAPOD_PYTHON_END@
robot.set_actuator_types("servo")

simu = rd.RobotDARTSimu(0.001)

graphics = rd.gui.Graphics()
simu.set_graphics(graphics)
graphics.look_at([0., 3.5, 2.], [0., 0., 0.25])

simu.add_floor()
simu.add_robot(robot)
simu.run(10.)
print(robot.base_pose_vec()[-3:].transpose())
robot.reset()