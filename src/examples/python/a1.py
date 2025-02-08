import RobotDART as rd

# @A1_PYTHON@
robot = rd.A1()
# @A1_PYTHON_END@
robot.set_actuator_types("servo")
simu = rd.RobotDARTSimu(0.001)
simu.set_collision_detector("fcl")

graphics = rd.gui.Graphics()
simu.set_graphics(graphics)

graphics.look_at([0., 3.5, 2.], [0., 0., 0.25])
simu.add_checkerboard_floor()
simu.add_robot(robot)
    
while (simu.scheduler().next_time() < 20 and not simu.graphics().done()):
    simu.step_world()
    # Print IMU measurements
    if (simu.schedule(robot.imu().frequency())):
        # @A1_PRINT_IMU_PYTHON@
        print( "Angular    Position: ", robot.imu().angular_position_vec().transpose())
        print( "Angular    Velocity: ", robot.imu().angular_velocity().transpose())
        print( "Linear Acceleration: ", robot.imu().linear_acceleration().transpose())
        print( "=================================")
        # @A1_PRINT_IMU_PYTHON_END@

robot.reset()

