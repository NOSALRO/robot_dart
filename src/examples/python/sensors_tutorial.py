import RobotDART as rd
import numpy as np
import magnum
import dartpy
import time
robot = rd.Iiwa()
robot.set_actuator_types("torque")

ctrl = [0., np.pi / 3., 0., -np.pi / 4., 0., 0., 0.]

controller = rd.PDControl(ctrl)
robot.add_controller(controller)
controller.set_pd(300., 50.)

simu = rd.RobotDARTSimu(0.001)

graphics = rd.gui.Graphics()
simu.set_graphics(graphics)

graphics.look_at([0., 3.5, 2.], [0., 0., 0.25])

simu.add_robot(robot)
simu.add_checkerboard_floor()

pose = [0., 0., 0., 1.5, 0., 0.25]

simu.add_robot(rd.Robot.create_box(
    [0.1, 0.1, 0.5], pose, "free", 1., magnum.Color4.red(1.), "box"))


robot.set_damping_coeffs(0.)
robot.set_coulomb_coeffs(0.)
robot.set_spring_stiffnesses(0.)

dampings = robot.damping_coeffs()
frictions = robot.coulomb_coeffs()
stiffnesses = robot.spring_stiffnesses()

for i in range(len(dampings)):
    print("DoF #", i)
    print("    Stiffness: ", stiffnesses[i])
    print("    Friction: ", frictions[i])
    print("    Damping: ", dampings[i])

camera = rd.sensor.Camera(graphics.magnum_app(), 256, 256)

# @CAMERA_SENSOR_RGBD_RECORD_DEPTH_PYTHON@
# cameras are recording color images by default, enable depth images as well for this example
camera.camera().record(True, True)
# @CAMERA_SENSOR_RGBD_RECORD_DEPTH_PYTHON_END@
# cameras can also record video
camera.record_video("video-camera.mp4")

# attach camera to body
tf = dartpy.math.Isometry3()
rot = dartpy.math.AngleAxis(3.14, [1., 0., 0.])
rot = rot.multiply(dartpy.math.AngleAxis(1.57, [0., 0., 1.])).to_rotation_matrix()
tf.set_translation([0., 0., 0.1])
# cameras are looking towards -z by default
camera.attach_to_body(robot.body_node("iiwa_link_ee"), tf)

# @TORQUE_SENSOR_PYTHON@
# Add torque sensors to the robot
tq_sensors = np.empty(robot.num_dofs(), dtype=rd.sensor.Torque)
ct = 0
for joint in robot.dof_names():
    simu.add_sensor(rd.sensor.Torque(robot, joint, 1000))
    tq_sensors[ct] = simu.sensors()[-1]
    ct += 1
# @TORQUE_SENSOR_PYTHON_END@

# @FORCE_TORQUE_SENSOR_PYTHON@
# Add force-torque sensors to the robot
f_tq_sensors = np.empty(robot.num_dofs(), dtype=rd.sensor.ForceTorque)
ct = 0
for joint in robot.dof_names():
    simu.add_sensor(rd.sensor.ForceTorque(
        robot, joint, 1000, "parent_to_child"))
    f_tq_sensors[ct] = simu.sensors()[-1]
    print(f_tq_sensors)
    ct += 1
# @FORCE_TORQUE_SENSOR_PYTHON_END@

# @IMU_SENSOR_PYTHON@
# Add IMU sensors to the robot
ct = 0
imu_sensors = np.empty(robot.num_bodies(), dtype=rd.sensor.IMU)
for body_node in robot.body_names():
    simu.add_sensor(rd.sensor.IMU(
        rd.sensor.IMUConfig(robot.body_node(body_node), 1000)))
    imu_sensors[ct] = simu.sensors()[-1]
    ct += 1
# @IMU_SENSOR_PYTHON_END@

# Cameras can be used as sensors too
simu.add_sensor(camera)
start = time.time()
external_force = np.zeros(3)

while (simu.scheduler().next_time() < 20 and not simu.graphics().done()):
    simu.step()

    # Print torque sensor measurement
    if (simu.schedule(tq_sensors[0].frequency())):
        
        # @TORQUE_MEASUREMENT_PYTHON@
        # vector that contains the torque measurement for every joint (scalar)
        torques_measure = np.empty(robot.num_dofs())
        ct = 0
        for tq_sens in tq_sensors:
            torques_measure[ct] = tq_sens.torques()
            ct += 1
        # @TORQUE_MEASUREMENT_PYTHON_END@
        
        # @FORCE_TORQUE_MEASUREMENT_PYTHON@
        #  matrix that contains the torque measurement for every joint (3d vector)
        ft_torques_measure = np.empty([robot.num_dofs(), 3])
        #  matrix that contains the force measurement for every joint (3d vector)
        ft_forces_measure = np.empty([robot.num_dofs(), 3])
        #  matrix that contains the wrench measurement for every joint (6d vector)[torque, force]
        ft_wrench_measure = np.empty([robot.num_dofs(), 6])
        ct = 0
        for f_tq_sens in f_tq_sensors:
            ft_torques_measure[ct, :] = f_tq_sens.torque()
            ft_forces_measure[ct, :] = f_tq_sens.force()
            ft_wrench_measure[ct, :] = f_tq_sens.wrench()
            ct += 1

        # @FORCE_TORQUE_MEASUREMENT_PYTHON_END@

        # @IMU_MEASUREMENT_PYTHON@
        imu_angular_positions_measure = np.empty([robot.num_bodies(), 3])
        imu_angular_velocities_measure = np.empty([robot.num_bodies(), 3])
        imu_linear_acceleration_measure = np.empty([robot.num_bodies(), 3])
        ct = 0
        for imu_sens in imu_sensors:
            imu_angular_positions_measure[ct,:] = imu_sens.angular_position_vec()
            imu_angular_velocities_measure[ct, :] = imu_sens.angular_velocity()
            imu_linear_acceleration_measure[ct,:] = imu_sens.linear_acceleration()
            ct += 1

        # @IMU_MEASUREMENT_PYTHON_END@

        print("Torque sensors torque: \n ", torques_measure.transpose())
        print("Force-Torque sensors torque: \n ",
              ft_torques_measure.transpose())
        print("Force-Torque sensors force: \n ", ft_forces_measure.transpose())
        print("Force-Torque sensors wrench: \n ",
              ft_wrench_measure.transpose())
        print("IMU sensors Angular Position: \n ",
              imu_angular_positions_measure)
        print("IMU sensors Angular Velocity: \n ",
              imu_angular_velocities_measure)
        print("IMU sensors Linear Acceleration Velocity: \n ",
              imu_linear_acceleration_measure)
        print("=================================")

    # add external force
    time_in_seconds = int(simu.scheduler().next_time())
    if ((time_in_seconds % 3 >= 1) & (time_in_seconds % 3 < 2)):
        external_force = np.ones([3]) * 30.0
        print("Applying force on iiwa_link_4")

    else:
        external_force = np.zeros([3])

    robot.set_external_force("iiwa_link_4", external_force)

# @RGB_SENSOR_PYTHON@
# a nested array (w*h*3) of the last image taken can be retrieved
rgb_image = camera.image()
# @RGB_SENSOR_PYTHON_END@
# @RGB_SENSOR_MEASURE_PYTHON@
# we can also save them to png
rd.gui.save_png_image("camera-small.png", rgb_image)
# convert an rgb image to grayscale (useful in some cases)
gray_image = rd.gui.convert_rgb_to_grayscale(rgb_image)
rd.gui.save_png_image("camera-gray.png", gray_image)
# @RGB_SENSOR_MEASURE_PYTHON_END@

# @RGB_D_SENSOR_PYTHON@
# get the depth image from a camera
# with a version for visualization or bigger differences in the output
rgb_d_image = camera.depth_image()
# and the raw values that can be used along with the camera parameters to transform the image to point-cloud
rgb_d_image_raw = camera.raw_depth_image()
# @RGB_D_SENSOR_PYTHON_END@
# @RGB_D_SENSOR_MEASURE_PYTHON@
rd.gui.save_png_image("camera-depth.png", rgb_d_image)
rd.gui.save_png_image("camera-depth-raw.png", rgb_d_image_raw)
# @RGB_D_SENSOR_MEASURE_PYTHON_END@

end = time.time()
elapsed_seconds = end - start
print("benchmark time: ", elapsed_seconds, "s\n")

robot.reset()
