import RobotDART as rd
import numpy as np

# @FRANKA_PYTHON@
robot = rd.Franka()
# @FRANKA_PYTHON_END@
robot.set_actuator_types("torque")

# @PD_CONTROL_PYTHON@
# add a PD-controller to the arm
# set desired positions
ctrl = [0., np.pi / 4., 0., -np.pi / 4., 0., np.pi / 2., 0., 0.]

# add the controller to the robot
controller = rd.PDControl(ctrl)
robot.add_controller(controller)
controller.set_pd(300., 50.)
# @PD_CONTROL_END@
# choose time step of 0.001 seconds
simu = rd.RobotDARTSimu(0.001)
simu.set_collision_detector("fcl")
simu.enable_status_bar(True, 20)  # change the font size


graphics = rd.gui.Graphics()
simu.set_graphics(graphics)
graphics.look_at([0., 3., 1.], [0., 0., 0.])

simu.add_checkerboard_floor()
simu.add_robot(robot)
simu.run(30.)
robot.reset()
