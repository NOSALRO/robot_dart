import RobotDART as rd
import numpy as np

# @IIWA_PYTHON@
robot = rd.Iiwa()
# @IIWA_PYTHON_END@
ctrl = [0., np.pi / 3., 0., -np.pi / 4., 0., 0., 0.]
controller = rd.PDControl(ctrl)
robot.add_controller(controller)
controller.set_pd(300., 50.)

simu = rd.RobotDARTSimu(0.001)
simu.set_collision_detector("fcl")

graphics = rd.gui.Graphics()
simu.set_graphics(graphics)
graphics.look_at([0., 3.5, 2.], [0., 0., 0.25])

simu.add_checkerboard_floor()
simu.add_robot(robot)

# @ROBOT_GHOST_PYTHON@
# Add a ghost robot; only visuals, no dynamics, no collision
ghost = robot.clone_ghost()
simu.add_robot(ghost)
# @ROBOT_GHOST_PYTHON_END@

simu.set_text_panel("Iiwa simulation")
simu.run(20.)
robot.reset()
