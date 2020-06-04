import numpy as np
import RobotDART as rd
import dartpy # OSX breaks if this is imported before RobotDART

# Create custom controller
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

# Load robot from URDF
robot = rd.Robot("res/models/arm.urdf", "arm", False)
robot.fix_to_world()

# Initiate custom controller
control = MyController([0.0, 1.57, -0.5, 0.7], False)
# Add it to the robot
robot.add_controller(control, 1.)

# Print initial positions of the robot
print(robot.positions())

# Create simulator object
simu = rd.RobotDARTSimu(0.001)

# Add robot and floor to the simulation
simu.add_robot(robot)
simu.add_checkerboard_floor(10., 0.1, 1., np.zeros((6,1)), "floor")

simu.run(5.)

print(robot.positions())