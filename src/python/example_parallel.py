import numpy as np
import RobotDART as rd
import dartpy # OSX breaks if this is imported before RobotDART
from multiprocessing import Process, Lock
from timeit import default_timer as timer
import os

robot = rd.Robot("arm.urdf", "arm", False)
robot.fix_to_world()

def test():
    pid = os.getpid()
    ii = pid%15

    # create the controller
    pdcontrol = rd.PDControl([0.0, 1.0, -1.5, 1.0], False)

    # clone the robot
    grobot = robot.clone()

    # add the controller to the robot
    grobot.add_controller(pdcontrol, 1.)
    pdcontrol.set_pd(200., 20.)

    # create the simulation object
    simu = rd.RobotDARTSimu(0.001)

    # set the graphics
    graphics = rd.gui.WindowlessGraphics()
    simu.set_graphics(graphics)

    graphics.look_at([0.4 * ii, 3.5 - ii * 0.1, 2.], [0., 0., 0.25], [0., 0., 1.])

    # add the robot and the floor
    simu.add_robot(grobot)
    simu.add_checkerboard_floor(10., 0.1, 1., np.zeros((6,1)), "floor")

    # run
    simu.run(20.)

    # save last frame for visualization purposes
    img = graphics.image()
    rd.gui.save_png_image('camera-' + str(ii) + '.png', img)

# helper function to run in parallel
def runInParallel(N):
  proc = []
  for i in range(N):
    # rd.gui.run_with_gl_context accepts 2 parameters:
    #    (func, wait_time_in_ms)
    #    the func needs to be of the following format: void(), aka no return, no arguments
    p = Process(target=rd.gui.run_with_gl_context, args=(test, 20))
    p.start()
    proc.append(p)
  for p in proc:
    p.join()

print('Running parallel evaluations')
N = 15
start = timer()
runInParallel(N)
end = timer()
print('Time:', end-start)

# print('Running sequential evaluations')
# start = timer()
# for i in range(N):
#     test()
# end = timer()
# print('Time:', end-start)
