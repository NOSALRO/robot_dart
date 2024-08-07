import RobotDART as rd
import numpy as np
import dartpy
robot = rd.Iiwa()

ctrl = [-0.3, np.pi/3, 0.2, -np.pi/4,  0., 0., -0.6]
controller = rd.PDControl(ctrl)
robot.add_controller(controller)
controller.set_pd(500., 50.)

simu = rd.RobotDARTSimu(0.001)

# Graphics

gconfig = rd.gui.Graphics.default_configuration()
gconfig.width = 1024
gconfig.height = 768
graphics = rd.gui.Graphics(gconfig)

simu.set_graphics(graphics)

# set position of the main camera, and the position where the main camera is looking at
graphics.look_at([0., 3.5, 2.], [0., 0., 0.25])
# record images from main camera/graphics
graphics.camera().record(True)
graphics.record_video("video-main.mp4", simu.graphics_freq())

# @ADD_NEW_CAMERA_PYTHON@
# Add camera
camera = rd.sensor.Camera(graphics.magnum_app(), 32, 32)
# @ADD_NEW_CAMERA_PYTHON_END@

#  @MANIPULATE_CAM_SEP_PYTHON@
camera.camera().set_far_plane(5.)
camera.camera().set_near_plane(0.01)
camera.camera().set_fov(60.0)
#  @MANIPULATE_CAM_PYTHON_SEP_END@

#  @MANIPULATE_CAM_PYTHON@
camera.camera().set_camera_params(5.,  # far plane
                                  0.01,  # near plane
                                  60.0,  # field of view
                                  600,  # width
                                  400)  # height
#  @MANIPULATE_CAM_PYTHON_END@

camera.camera().record(True, True)  # cameras are recording color images by default, enable depth images as well for this example

# @RECORD_VIDEO_CAMERA_PYTHON@

# cameras can also record video
camera.record_video("video-camera.mp4")
#  @RECORD_VIDEO_CAMERA_PYTHON_END@

# @CAM_POSITION_PYTHON@
# set the position of the camera, and the position where the main camera is looking at
cam_pos = [-0.5, -3., 0.75]
cam_looks_at = [0.5, 0., 0.2]
camera.look_at(cam_pos, cam_looks_at)
# @CAM_POSITION_PYTHON_END@

# @CAM_ATTACH_PYTHON@
tf = dartpy.math.Isometry3()
rot = dartpy.math.AngleAxis(3.14, [1., 0., 0.])
rot = rot.multiply(dartpy.math.AngleAxis(1.57, [0., 0., 1.])).to_rotation_matrix()
tf.set_translation([0., 0., 0.1])
tf.set_rotation(rot)
camera.attach_to_body(robot.body_node("iiwa_link_ee"), tf)  # cameras are looking towards -z by default
# @CAM_ATTACH_PYTHON_END@
# the default checkerboard floor values are the following:
# floor_width = 10.0
# floor_height = 0.1,
# size = 1.,
# tf = dartpy.math.Isometry3.Identity() (transformation matrix of the floor)
# floor_name = "checkerboard_floor",
# first_color = magnum.Color3(0, 0, 0),
# second_color = magnum.Color3(128, 128, 128)
simu.add_checkerboard_floor()
simu.add_robot(robot)


pose = [0., 0., 0., 1.5, 0., 0.25]
simu.add_robot(rd.Robot.create_box([0.1, 0.1, 0.5], pose, "free", 1., [1, 0, 0, 1], "box"))
pose = [0., 0., 0., 1.5, -0.5, 0.25]
simu.add_robot(rd.Robot.create_ellipsoid([0.2, 0.2, 0.2], pose, "free", 1., [1, 0, 0, 1], "sphere"))
simu.add_sensor(camera)

simu.run(10.)

# a nested std::vector (w*h*3) of the last image taken can be retrieved
gimage = graphics.image()
cimage = camera.image()

# we can also save them to png
rd.gui.save_png_image("camera-small.png", cimage)
rd.gui.save_png_image("camera-main.png", gimage)

# convert an rgb image to grayscale (useful in some cases)
gray_image = rd.gui.convert_rgb_to_grayscale(gimage)
rd.gui.save_png_image("camera-gray.png", gray_image)

# get the depth image from a camera
# with a version for visualization or bigger differences in the output
rd.gui.save_png_image("camera-depth.png", camera.depth_image())
# and the raw values that can be used along with the camera parameters to transform the image to point-cloud
rd.gui.save_png_image("camera-depth-raw.png", camera.raw_depth_image())

depth_image = camera.depth_array()

small_box = rd.Robot.create_box([0.01, 0.01, 0.01], np.zeros(6), "fixed", 1., [0, 0, 1, 1], "box_pc")
point_cloud = rd.gui.point_cloud_from_depth_array(
    depth_image, camera.camera_intrinsic_matrix(), camera.camera_extrinsic_matrix(), camera.camera().far_plane())
for i in range(len(point_cloud)):
    if (i % 30 == 0):  # do not display all the points in the cloud
        pose = 0., 0., 0., point_cloud[i]
        bbox = small_box.clone_ghost("box_" + str(i), [0, 0, 1, 1])
        bbox.set_base_pose(pose)
        bbox.set_cast_shadows(False)
        simu.add_robot(bbox)


simu.set_graphics_freq(20)
simu.world().setTime(0.)
simu.scheduler().reset(simu.timestep(), True)
while (True):
    if (simu.step()):
        break
    if (simu.schedule(simu.graphics_freq())):
        depth_image = camera.depth_array()
        point_cloud = rd.gui.point_cloud_from_depth_array(
            depth_image, camera.camera_intrinsic_matrix(), camera.camera_extrinsic_matrix(), camera.camera().far_plane())
        print(simu.scheduler().current_time(), ": ", len(point_cloud))


robot.reset()
