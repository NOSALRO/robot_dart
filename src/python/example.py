import numpy as np
import dartpy
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

graphics = rd.gui.Graphics(simu.world(), 640, 480, True, False, "DART")
simu.set_graphics(graphics)
simu.add_checkerboard_floor(10., 0.1, 1., np.zeros((6,1)), "floor")

# camera = rd.gui.CameraOSR(simu.world(), graphics, 256, 256)
# camera.attach_to("arm_link_5", dartpy.math.Isometry3())
# simu.add_camera(camera)

simu.run(5.)

# img = camera.image()
# rd.gui.save_png_image('test.png', img)

print(control.get_positions())