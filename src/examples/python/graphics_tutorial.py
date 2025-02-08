from xml.dom.expatbuilder import InternalSubsetExtractor
import RobotDART as rd
import numpy as np
import magnum


def random_box(num=0):
    pose = np.random.random([6])
    pose[5] += 0.5
    print(pose)
    size = np.random.random([3]) * np.array([0.1, 0.2, 0.1]) + 0.3
    print(size)
    return rd.Robot.create_box(size, pose, "free", 1., [1, 0, 0, 1.], "box_"+str(num))


def random_sphere(num=0):
    pose = np.random.random([6])
    pose[5] += 0.5
    print(pose)
    size = np.random.random() * np.array([0.2, 0.2, 0.2]) + 0.3
    print(size)
    return rd.Robot.create_ellipsoid(size, pose, "free", 1., [0, 0, 1, 1.], "sphere_"+str(num))


# @INIT_SIMU_PYTHON@
# choose time step of 0.001 seconds
simu = rd.RobotDARTSimu(0.001)
# @INIT_SIMU_PYTHON_END@

# @MODIFY_SIMU_DT_PYTHON@
# set timestep to 0.005 and update control frequency(bool)
simu.set_timestep(0.005, True)
# @MODIFY_SIMU_DT_PYTHON_END@

# @SIMU_GRAVITY_PYTHON@
# set gravitational force of mars
mars_gravity = [0., 0., -3.721]
simu.set_gravity(mars_gravity)
# @SIMU_GRAVITY_PYTHON_END@

# @GRAPHICS_PARAMS_PYTHON@
configuration = rd.gui.GraphicsConfiguration()
# We can change the width/height of the window (or camera, dimensions)
configuration.width = 1280
configuration.height = 960
configuration.title = "Graphics Tutorial"  # We can set a title for our window

# We can change the configuration for shadows
configuration.shadowed = True
configuration.transparent_shadows = True
configuration.shadow_map_size = 1024

# We can also alter some specifications for the lighting
configuration.max_lights = 3  # maximum number of lights for our scene
configuration.specular_strength = 0.25  # strength og the specular component

#  Some extra configuration for the main camera
configuration.draw_main_camera = True
configuration.draw_debug = True
configuration.draw_text = True

# We can also change the background color [default = black]
configuration.bg_color = [1., 1., 1., 1.]

# create the graphics object with the configuration as a parameter
graphics = rd.gui.Graphics(configuration)
# @GRAPHICS_PARAMS_PYTHON_END@

simu.set_graphics(graphics)
# set the camera at position (0, 3, 1) looking at the center (0, 0, 0)
graphics.look_at([0., 3., 1.], [0., 0., 0.])

# add floor of square size of 10 meters and height of 0.2 meters
simu.add_floor(10., 0.2)

# add random objects to the world
simu.add_robot(random_box(1))
simu.add_robot(random_sphere(1))

# add a simple arm
arm_robot = rd.Robot("arm.urdf")

# pin the arm to world
arm_robot.fix_to_world()
arm_robot.set_position_enforced(True)
simu.add_robot(arm_robot)

# @SHADOWS_GRAPHICS_PYTHON@
# Disable shadows
graphics.enable_shadows(False, False)
simu.run(1.)
# Enable shadows only for non-transparent objects
graphics.enable_shadows(True, False)
simu.run(1.)
# Enable shadows for transparent objects as well
graphics.enable_shadows(True, True)
simu.run(1.)
# @SHADOWS_GRAPHICS_PYTHON_END@

# @CLR_LIGHT_PYTHON@
# Clear Lights
graphics.clear_lights()
# @CLR_LIGHT_PYTHON_END@

simu.run(.2)

# @LIGHT_MATERIAL_PYTHON@
# Clear Light material
shininess = 1000.
white = magnum.Color4(1., 1., 1., 1.)

# ambient, diffuse, specular
custom_material = rd.gui.Material(white, white, white, shininess)
# @LIGHT_MATERIAL_PYTHON_END@

# @POINT_LIGHT_PYTHON@
# Create point light
position = magnum.Vector3(0., 0., 2.)
intensity = 1.
attenuation_terms = magnum.Vector3(1., 0., 0.)
point_light = rd.gui.create_point_light(position, custom_material, intensity, attenuation_terms)
graphics.add_light(point_light)
# @POINT_LIGHT_PYTHON_END@


simu.run(1.)
graphics.clear_lights()
simu.run(.2)


# @DIRECTIONAL_LIGHT_PYTHON@
# Create directional light
direction = magnum.Vector3(-1, -1, -1)
directional_light = rd.gui.create_directional_light(direction, custom_material)
graphics.add_light(directional_light)
# @DIRECTIONAL_LIGHT_PYTHON_END@

simu.run(1.)
graphics.clear_lights()
simu.run(.2)


# @SPOT_LIGHT_PYTHON@
# Create spot light
position = magnum.Vector3(0., 0., 1.)
direction = magnum.Vector3(-1, -1, -1)
intensity = 1.
attenuation_terms = magnum.Vector3(1., 0., 0.)
spot_exponent = np.pi
spot_cut_off = np.pi / 8
spot_light = rd.gui.create_spot_light(position, custom_material, direction, spot_exponent, spot_cut_off, intensity, attenuation_terms)
graphics.add_light(spot_light)
# @SPOT_LIGHT_PYTHON_END@

simu.run(1.)
