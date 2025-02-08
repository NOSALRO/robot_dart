import RobotDART as rd

simu = rd.RobotDARTSimu(0.001)
robot = rd.Iiwa()
robot.set_actuator_types("servo")
simu.add_robot(robot)

# @SET_COLLISION_DETECTOR_PYTHON@
simu.set_collision_detector("fcl") # collision_detector can be "dart", "fcl", "ode" or "bullet" (case does not matter)
# @SET_COLLISION_DETECTOR_PYTHON_END@

# check if self-collision is enabled
# @SELF_COLLISIONS_PYTHON@
if(not robot.self_colliding()):
    print("Self collision is not enabled")
    # set self collisions to true and adjacent collisions to false
    robot.set_self_collision(True, False)
# @SELF_COLLISIONS_PYTHON_END@