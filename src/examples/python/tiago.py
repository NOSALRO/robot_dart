import RobotDART as rd
import time
import numpy as np
dt = 0.001
simu = rd.RobotDARTSimu(dt)

# this is important for wheel collision; with FCL it does not work well
simu.set_collision_detector("bullet")

freq = int(1//dt)
# @TIAGO_PYTHON@
robot = rd.Tiago()
# @TIAGO_PYTHON_END@
print("the model used is [", robot.model_filename(), "]")

configuration = rd.gui.GraphicsConfiguration()
configuration.width = 1280
configuration.height = 960
configuration.bg_color = [1.0, 1.0, 1.0, 1.0]
graphics = rd.gui.Graphics(configuration)
simu.set_graphics(graphics)
graphics.look_at([0., 3., 2.], [0., 0., 0.25])
graphics.record_video("tiago_dancing.mp4")

simu.add_checkerboard_floor()
simu.add_robot(robot)

simu.set_control_freq(freq)
wheel_dofs = ["wheel_right_joint", "wheel_left_joint"]
arm_dofs = ["arm_1_joint",
            "arm_2_joint",
            "arm_3_joint",
            "arm_4_joint",
            "arm_5_joint",
            "torso_lift_joint"]

init_positions = robot.positions(arm_dofs)
start = time.time()

while (simu.scheduler().next_time() < 20. and not simu.graphics().done()):
    if (simu.schedule(simu.control_freq())):
        delta_pos = [np.sin(simu.scheduler().current_time() * 2) + np.pi/2,
                     np.sin(simu.scheduler().current_time() * 2),
                     np.sin(simu.scheduler().current_time() * 2),
                     np.sin(simu.scheduler().current_time() * 2),
                     np.sin(simu.scheduler().current_time() * 2),
                     np.sin(simu.scheduler().current_time() * 2)]
        commands = (init_positions + delta_pos) - robot.positions(arm_dofs)
        robot.set_commands(commands, arm_dofs)
        cmd = [0, 5]
        robot.set_commands(cmd, wheel_dofs)
    simu.step_world()

end = time.time()
elapsed_seconds = end - start
print("benchmark time: ", elapsed_seconds, " s")
robot.reset()
