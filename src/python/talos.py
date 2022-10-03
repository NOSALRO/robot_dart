import RobotDART as rd
import time
import numpy as np

# @TALOS_PYTHON@
# load talos
robot = rd.Talos()

# set actuator types to VELOCITY (for speed)
robot.set_actuator_types("velocity")
# enforce limits (ON by default)
robot.set_position_enforced(True)

dt = 0.001
simu = rd.RobotDARTSimu(dt)
# must use fcl collision detector
simu.set_collision_detector("fcl")
# @TALOS_PYTHON_END@

gconfig = rd.gui.Graphics.default_configuration()
gconfig.width = 1024
gconfig.height = 768
graphics = rd.gui.Graphics(gconfig)

simu.set_graphics(graphics)

# set position of the main camera, and the position where the main camera is looking at
graphics.look_at([0., 3.5, 2.], [0., 0., 0.25])
# @RECORD_VIDEO_ROBOT_GRAPHICS_PARAMS_PYTHON@
graphics.record_video("talos_dancing.mp4")
# @RECORD_VIDEO_ROBOT_GRAPHICS_PARAMS_PYTHON_END@


simu.add_checkerboard_floor()
simu.add_robot(robot)

simu.set_control_freq(100)
dofs = ["arm_left_1_joint",
        "arm_left_2_joint",
        "arm_right_1_joint",
        "arm_right_2_joint",
        "torso_1_joint"]

init_positions = robot.positions(dofs)


start = time.time()
while (simu.scheduler().next_time() < 20. and not simu.graphics().done()):
    if (simu.schedule(simu.control_freq())):
        delta_pos = [np.sin(simu.scheduler().current_time() * 2.),
                     np.sin(simu.scheduler().current_time() * 2.),
                     np.sin(simu.scheduler().current_time() * 2.),
                     np.sin(simu.scheduler().current_time() * 2.),
                     np.sin(simu.scheduler().current_time() * 2.)]
        commands = (init_positions + delta_pos) - robot.positions(dofs)
        robot.set_commands(commands, dofs)

    simu.step_world()


end = time.time()
elapsed_seconds = end - start
print("benchmark time: ", elapsed_seconds, "s")
