# @HELLO_WORLD_INCLUDE_PYTHON@
import RobotDART as rd
# @HELLO_WORLD_INCLUDE_PYTHON_END@

# @HELLO_WORLD_ROBOT_CREATION_PYTHON@
robot = rd.Robot("pexod.urdf");
# @HELLO_WORLD_ROBOT_CREATION_PYTHON_END@

# @HELLO_WORLD_ROBOT_PLACING_PYTHON@\
# pose is a 6D vector (first 3D orientation in angle-axis and last 3D translation)
robot.set_base_pose([0., 0., 0., 0., 0., 0.2])
# @HELLO_WORLD_ROBOT_PLACING_PYTHON_END@

# @HELLO_WORLD_ROBOT_SIMU_PYTHON@
simu = rd.RobotDARTSimu(0.001); # dt=0.001, 1KHz simulation
simu.add_floor();
simu.add_robot(robot);
# @HELLO_WORLD_ROBOT_SIMU_PYTHON_END@

# @HELLO_WORLD_ROBOT_GRAPHIC_PYTHON@
graphics = rd.gui.Graphics()
simu.set_graphics(graphics)
graphics.look_at([0.5, 3., 0.75], [0.5, 0., 0.2])
# @HELLO_WORLD_ROBOT_GRAPHIC_PYTHON_END@
# @HELLO_WORLD_ROBOT_RUN_PYTHON@
simu.run(10.)
# @HELLO_WORLD_ROBOT_RUN_PYTHON_END@