import RobotDART as rd
import time
import numpy as np
# fast TalosFastCollision:
# - use dart for collision detection (instead of FCL) [only handle boxes and spheres]
# - the collisions are detected with boxes, you can have the boxes visual with talos_box.urdf
# - the boxes are totally covering the meshes. There is one box per link
# - the urdf does not have the mimic (used for grippers)

# @TALOS_FAST_PYTHON@
# load talos fast
robot = rd.TalosFastCollision()

# Set actuator types to VELOCITY (for speed)
robot.set_actuator_types("velocity")

dt = 0.001
simu = rd.RobotDARTSimu(dt)
# we can use the DART (fast) collision detector
simu.set_collision_detector("dart")
# @TALOS_FAST_END@
gconfig = rd.gui.Graphics.default_configuration()
gconfig.width = 1024
gconfig.height = 768
graphics = rd.gui.Graphics(gconfig)

simu.set_graphics(graphics)

# set position of the main camera, and the position where the main camera is looking at
graphics.look_at([0., 3.5, 2.], [0., 0., 0.25])
# @RECORD_VIDEO_ROBOT_GRAPHICS_PARAMS_PYTHON@
graphics.record_video("talos_fast.mp4")
# @RECORD_VIDEO_ROBOT_GRAPHICS_PARAMS_PYTHON_END@

simu.add_floor()
simu.add_robot(robot)


simu.set_control_freq(100)
dofs = ["arm_left_1_joint",
        "arm_left_2_joint",
        "arm_right_1_joint",
        "arm_right_2_joint",
        "torso_1_joint"]

init_positions = robot.positions(dofs)
undamaged_joints = [0, 1, 2, 3, 4]


start = time.time()
while (simu.scheduler().next_time() < 20. and not simu.graphics().done()):
    if (simu.scheduler().next_time() > 2. and simu.scheduler().next_time() < 2.0 + dt):
        robot.set_actuator_type("passive", "arm_left_1_joint")
        robot.set_actuator_type("passive", "arm_left_2_joint")
        robot.set_actuator_type("passive", "arm_left_3_joint")
        robot.set_actuator_type("passive", "arm_left_4_joint")
        undamaged_joints = [2, 3, 4]

    if (simu.schedule(simu.control_freq())):

        delta_pos = [np.sin(simu.scheduler().current_time() * 2.),
                     np.sin(simu.scheduler().current_time() * 2.),
                     np.sin(simu.scheduler().current_time() * 2.),
                     np.sin(simu.scheduler().current_time() * 2.),
                     np.sin(simu.scheduler().current_time() * 2.)]
        commands = (init_positions + delta_pos) - robot.positions(dofs)
        commands_to_send = len(undamaged_joints) * [0.]
        dofs_to_send = []
        
        for i in range(len(commands_to_send)):
            commands_to_send[i] = commands[undamaged_joints[i]]
            dofs_to_send.append(dofs[undamaged_joints[i]])

    robot.set_commands(commands_to_send, dofs_to_send)

    simu.step_world()


end = time.time()
elapsed_seconds = end - start
print("benchmark time: ", elapsed_seconds, "s")

robot.reset()
