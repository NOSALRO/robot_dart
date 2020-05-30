import numpy as np
import RobotDART as rd
import dartpy # OSX break if this is imported before RobotDART
# import magnum

# Create custom controller
class MyController(rd.RobotControl):
    def __init__(self, ctrl, full_control):
        rd.RobotControl.__init__(self, ctrl, full_control)

    def configure(self):
        self._active = True

    def calculate(self, t):
        target = np.array([self._ctrl])
        cmd = 100*(target-self.get_positions())
        return cmd[0]

    # TO-DO: This is NOT working at the moment!
    def clone(self):
        return MyController(self._ctrl, self._full_control)

class MyDesc(rd.Descriptor):
    def __init__(self, simu, desc):
        rd.Descriptor.__init__(self, simu, desc)
        self._states = []

    def __call__(self):
        if(self._simu.num_robots()>0):
            self._states.append(self._simu.robot(0).positions())

# Load robot from URDF
robot = rd.Robot("res/models/arm.urdf", "arm", False)
robot.fix_to_world()

# Initiate custom controller
control = MyController([0.0, 2, -0.5, 0.7], False)
# Add it to the robot
robot.add_controller(control, 1.)

# Print initial positions of the robot
print(control.get_positions())

# Create simulator object
simu = rd.RobotDARTSimu(0.001)
desc = MyDesc(simu, 10)
simu.add_descriptor(desc)

# Create graphics
graphics = rd.gui.Graphics(simu, rd.gui.GraphicsConfiguration())
# graphics.clear_lights()
# mat = rd.gui.Material(magnum.Color4(0, 0, 0, 1), magnum.Color4(1, 1, 1, 1), magnum.Color4(1, 1, 1, 1), 80.)
# graphics.add_light(rd.gui.create_point_light(magnum.Vector3(-1., 1., 2.), mat, 2., magnum.Vector3(0., 0., 1.)))
# graphics.add_light(rd.gui.create_point_light(magnum.Vector3(1., -1., 2.), mat, 2., magnum.Vector3(0., 0., 1.)))
simu.set_graphics(graphics)

# Add robot and floor to the simulation
simu.add_robot(robot)
simu.add_checkerboard_floor(10., 0.1, 1., np.zeros((6,1)), "floor")

# Add a camera to the end-effector of the manipulator
camera = rd.gui.CameraOSR(simu, graphics.magnum_app(), 256, 256)
rot = dartpy.math.AngleAxis(3.14, [1., 0., 0.]).to_rotation_matrix()
rot = rot.dot(dartpy.math.AngleAxis(1.57, [0., 0., 1.]).to_rotation_matrix())
camera.attach_to("arm_link_5", dartpy.math.Isometry3(rotation=rot, translation=[0., 0., 0.1]))
simu.add_camera(camera)

simu.run(5.)

img = camera.image()
rd.gui.save_png_image('camera.png', img)

print(control.get_positions())
print(desc._states[-1])
print(len(desc._states))