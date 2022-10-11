import RobotDART as rd
import time
import numpy as np
from numpy import sin
# @LOAD_IICUB_PYTHON@
robot = rd.ICub()

# Set actuator types to VELOCITY motors so that they stay in position without any controller
robot.set_actuator_types("velocity")
simu = rd.RobotDARTSimu(0.001)
simu.set_collision_detector("fcl")
# @LOAD_IICUB_PYTHON_END@

graphics = rd.gui.Graphics()
simu.set_graphics(graphics)
graphics.look_at([0., 3.5, 2.], [0., 0., 0.25])

simu.add_checkerboard_floor()
simu.add_robot(robot)
ghost = robot.clone_ghost()
ghost.skeleton().setPosition(4, -1.57)
ghost.skeleton().setPosition(5, 1.1)
simu.add_robot(ghost)

robot.set_color_mode("material")


simu.set_control_freq(100)  # 100 Hz
dofs = [
    "l_knee",
    "r_knee",
    "l_ankle_pitch",
    "r_ankle_pitch",
    "l_hip_pitch",
    "r_hip_pitch",
    "l_shoulder_pitch",
    "l_shoulder_roll",
    "r_shoulder_pitch",
    "r_shoulder_roll"
]

start = time.time()
while (simu.scheduler().next_time() < 20 and  not simu.graphics().done()):
    if (simu.schedule(simu.control_freq())):
        commands = np.zeros(len(dofs))
        commands[0] = -0.45 * sin(simu.scheduler().current_time() * 2)
        commands[1] = -0.45 * sin(simu.scheduler().current_time() * 2)
        commands[2] = -0.35 * sin(simu.scheduler().current_time() * 2)
        commands[3] = -0.35 * sin(simu.scheduler().current_time() * 2)
        commands[4] = 0.15 * sin(simu.scheduler().current_time() * 2)
        commands[5] = 0.15 * sin(simu.scheduler().current_time() * 2)
        commands[6] = -0.15 * sin(simu.scheduler().current_time() * 2)
        commands[7] = 0.15 * sin(simu.scheduler().current_time() * 2)
        commands[8] = -0.15 * sin(simu.scheduler().current_time() * 2)
        commands[9] = 0.15 * sin(simu.scheduler().current_time() * 2)
        robot.set_commands(commands, dofs)
    simu.step_world()

    # Print IMU measurements
    if (simu.schedule(robot.imu().frequency())):
        # @ICUB_PRINT_IMU_PYTHON@
        print("Angular    Position: ",  robot.imu().angular_position_vec().transpose())
        print("Angular    Velocity: ",  robot.imu().angular_velocity().transpose())
        print("Linear Acceleration: ",  robot.imu().linear_acceleration().transpose())
        print("=================================" )
        # @ICUB_PRINT_IMU_PYTHON_END@

    # Print FT measurements
    if (simu.schedule(robot.ft_foot_left().frequency())):
        # @ICUB_PRINT_FT_PYTHON@
        print("FT ( force): ",  robot.ft_foot_left().force().transpose())
        print("FT (torque): ",  robot.ft_foot_left().torque().transpose())
        print("=================================")
        # @ICUB_PRINT_FT_PYTHON_END@

end = time.time()
elapsed_sec = end - start
print("benchmark time: " << elapsed_sec, "s")

