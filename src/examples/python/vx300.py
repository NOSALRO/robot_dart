import RobotDART as rd

# @VX300_PYTHON@
robot = rd.Vx300()
# @VX300_PYTHON_END@
robot.set_actuator_types("servo")

ctrl = [0.0, 1.0, -1.5, 1.0, 0.5, 0.]

controller = rd.PDControl(ctrl)
robot.add_controller(controller)

simu = rd.RobotDARTSimu()
simu.set_collision_detector("fcl")

simu.set_graphics(rd.gui.Graphics())
simu.add_robot(robot)
simu.add_checkerboard_floor()

for n in robot.dof_names():
    print(n)

simu.run(2.5)

ctrl = [0.0, -0.5, 0.5, -0.5, 0., 1.]
controller.set_parameters(ctrl)
controller.set_pd(20., 0.)
simu.run(2.5)
