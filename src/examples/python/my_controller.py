import numpy as np
import RobotDART as rd
import dartpy  # OSX breaks if this is imported before RobotDART
# import magnum

# Create custom controller
# @ROBOT_CONTROL_PYTHON@
class MyController(rd.RobotControl):
    def __init__(self, ctrl, full_control):
        rd.RobotControl.__init__(self, ctrl, full_control)

    def __init__(self, ctrl, controllable_dofs):
        rd.RobotControl.__init__(self, ctrl, controllable_dofs)

    def configure(self):
        self._active = True

    def calculate(self, t):
        target = np.array([self._ctrl])
        cmd = 100*(target-self.robot().positions(self._controllable_dofs))
        return cmd[0]

    # TO-DO: This is NOT working at the moment!
    def clone(self):
        return MyController(self._ctrl, self._controllable_dofs)
# @ROBOT_CONTROL_PYTHON_END@


# Load robot from URDF
robot = rd.Arm()

# Initiate custom controller
control = MyController([0.0, 1.57, -0.5, 0.7], False)
# Add it to the robot
robot.add_controller(control, 1.)

# Print initial positions of the robot
print(robot.positions())

# Create simulator object
simu = rd.RobotDARTSimu(0.001)

# Create graphics
graphics = rd.gui.Graphics()
simu.set_graphics(graphics)


# Add robot and floor to the simulation
simu.add_floor()
simu.add_robot(robot)
simu.run(5.)
