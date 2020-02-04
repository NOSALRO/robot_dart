import numpy as np
import RobotDART as rd

class MyController(rd.RobotControl):
    def __init__(self, ctrl, full_control):
        rd.RobotControl.__init__(self, ctrl, full_control)

    def configure(self):
        self._active = True

    def calculate(self, t):
        target = np.array([self._ctrl])
        cmd = 100*(target-self.get_positions())
        return cmd[0]

robot = rd.Robot("res/models/arm.urdf", "arm", False)
robot.fix_to_world()

control = MyController([0.0, 1.0, -1.5, 1.0], False)

# pdcontrol = rd.PDControl([0.0, 1.0, -1.5, 1.0], False)
# robot.add_controller(pdcontrol, 1.)
# pdcontrol.set_pd(200., 20.)
robot.add_controller(control, 1.)

print(control.get_positions())

simu = rd.RobotDARTSimu(0.001)
simu.add_robot(robot)

simu.set_graphics(rd.gui.Graphics(simu.world(), 640, 480, True, False, "DART"))

simu.run(5.)

print(control.get_positions())