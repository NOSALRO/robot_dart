---
  hide:
    -navigation
---
<style>
  .md-typeset h1,
  .md-content__button {
    display: none;
  }
</style>
# **Frequently Asked Questions**

This pages provides a user guide of the library through Frequently Asked Questions (FAQ).

## **What is a minimal working example of RobotDART?**

You can find a minimal working example at [hello_world.cpp](https://github.com/resibots/robot_dart/blob/master/src/examples/hello_world.cpp). This example is creating a world where a hexapod robot is placed just above a floor and left to fall. The robot has no actuation, and there is the simplest graphics configuration. Let's split it down.

- We first include the appropriate files:


=== "C++"
{{HELLO_WORLD_INCLUDE}}
=== "Python"
{{HELLO_WORLD_INCLUDE_PYTHON}}

- We then load our hexapod robot:

=== "C++"
{{HELLO_WORLD_ROBOT_CREATION}}
=== "Python"
{{HELLO_WORLD_ROBOT_CREATION_PYTHON}}
- We need to place it above the floor to avoid collision (we can use RobotDART's helpers ;)):

=== "C++"
{{HELLO_WORLD_ROBOT_PLACING}}
=== "Python"
{{HELLO_WORLD_ROBOT_PLACING_PYTHON}}
- We can now create the simulation object and add the robot and the floor:
=== "C++"
{{HELLO_WORLD_ROBOT_SIMU}}
=== "Python"
{{HELLO_WORLD_ROBOT_SIMU_PYTHON}}
- If needed or wanted, we can add a graphics component to visualize the scene:
=== "C++"
{{HELLO_WORLD_ROBOT_GRAPHIC}}
=== "Python"
{{HELLO_WORLD_ROBOT_GRAPHIC_PYTHON}}

- Once everything is configured, we can run our simulation for a few seconds:

=== "C++"
{{HELLO_WORLD_ROBOT_RUN}}
=== "Python"
{{HELLO_WORLD_ROBOT_RUN_PYTHON}}
- Here's how it looks:

![Hello World example](images/FAQ/hello_world.png){: style="display: block;margin-left: auto;margin-right: auto;width:70%"}
</details>

## **What robots are supported in RobotDART?**

RobotDART supports any robot that can be described by a URDF, SDF, SKEL or MJCF file. Nevertheless, we have a curated list of robots with edited and optimized models to be used with RobotDART (see the [robots page](robots.md) for details and examples).

## **How can I load my own URDF/SDF/SKEL/MJCF file?**

See the [robots page](robots.md) for details.

## **How do I enable graphics in my code?**

To enable graphics in your code, you need to do the following:

- Install [Magnum](http://magnum.graphics). See the [installation page](quick_install.md) for details.
- Create and set a graphics object in the simulation object. Here's an example:

=== "C++"
{{HELLO_WORLD_ROBOT_GRAPHIC}}
=== "Python"
{{HELLO_WORLD_ROBOT_GRAPHIC_PYTHON}}

## **I want to have multiple camera sensors. Is it possible?**

Having multiple camera sensors is indeed possible. We can add as many cameras as we wish along the main camera defined in [How do I record a video](#how-do-i-record-a-video):
=== "C++"
{{ADD_NEW_CAMERA}}
=== "Python"
{{ADD_NEW_CAMERA_PYTHON}}
## **How do I record a video?**

In order to record a video (1) of what the main or any other camera "sees", you need to call the function `record_video(path)` of the graphics class:
{.annotate}

1.  :man_raising_hand: Make sure that you have `ffmpeg` installed on your system.
=== "C++"
{{RECORD_VIDEO_ROBOT_GRAPHICS_PARAMS}}
=== "Python"
    ```python
    graphics.record_video("talos_dancing.mp4")
    ```

<!-- example not yet implemented^^  -->

Or the camera class:
=== "C++"
{{RECORD_VIDEO_CAMERA}}
=== "Python"
{{RECORD_VIDEO_CAMERA_PYTHON}}

## **How can I position a camera to the environment?**

In order to position a camera inside the world, we need to use the `lookAt` method of the camera/graphics object:
=== "C++"
{{CAM_POSITION}}
=== "Python"
{{CAM_POSITION_PYTHON}}
## **How can I attach a camera to a moving link?**
Cameras can be easily attached to a moving link:
=== "C++"
{{CAM_ATTACH}}
=== "Python"
{{CAM_ATTACH_PYTHON}}

## **How can I manipulate the camera object?**
Every camera has its own parameters, i.e a Near plane, a far plane, a Field Of View (FOV), a width and a height (that define the aspect ratio), you can manipulate each one separately:
=== "C++"
{{MANIPULATE_CAM_SEP}}
=== "Python"
{{MANIPULATE_CAM_SEP_PYTHON}}
or all at once:
=== "C++"
{{MANIPULATE_CAM}}
=== "Python"
{{MANIPULATE_CAM_PYTHON}}
You can find a complete example at [cameras.cpp](https://github.com/resibots/robot_dart/blob/master/src/examples/cameras.cpp) and [cameras.py](https://github.com/resibots/robot_dart/blob/master/src/examples/python/cameras.py).

## **How can I interact with the camera?**

We can move translate the cameras with the `WASD` keys, zoom in and out using the `mouse wheel` and rotate the camera with holding the `left mouse key` and moving the mouse.

## **What do the numbers in the status bar mean?**

The status bar looks like this:

![Status Bar](images/FAQ/bar.png){: style="display: block;margin-left: auto;margin-right: auto;width:70%"}

Where **simulation time** gives us the total simulated time (in seconds), **wall time** gives us the total time (in seconds) that has passed in real-time once we have started simulating. The next number **X.Xx** gives us the real-time factor: for example, **1.1x** means that the simulation runs 1.1 times faster than real-time, whereas **0.7x** means that the simulation runs slower than real-time. The value **it: XX ms** reports the time it took the last iteration (in milliseconds). The last part gives us whether the simulation tries to adhere to real-time or not. **sync** means that RobotDART will slow down the simulation in order for it to be in real-time, whereas **no-sync** means that RobotDART will try to run the simulation as fast as possible.


## **How can I alter the graphics scene (e.g., change lighting conditions)?**
When creating a graphics object, you can pass a `GraphicsConfiguration` object that changes the default values:
=== "C++"
{{GRAPHICS_PARAMS}}
=== "Python"
{{GRAPHICS_PARAMS_PYTHON}}
You can disable or enable shadows on the fly as well:
=== "C++"
{{SHADOWS_GRAPHICS}}
=== "Python"
{{SHADOWS_GRAPHICS_PYTHON}}
You can also add your own lights. The application by default creates 2 light sources and the maximum number of lights is 3 (you can change this once before the creation of the graphics object via the `GraphicsConfiguration` object). So usually before you add your lights, you have to clear the default lights:
=== "C++"
{{CLR_LIGHT}}
=== "Python"
{{CLR_LIGHT_PYTHON}}
Then you must create a custom light material:
=== "C++"
{{LIGHT_MATERIAL}}
=== "Python"
{{LIGHT_MATERIAL_PYTHON}}
Now you can add on ore more of the following lights:

**Point Light**:
=== "C++"
{{POINT_LIGHT}}
=== "Python"
{{POINT_LIGHT_PYTHON}}
**Spot Light**:
=== "C++"
{{SPOT_LIGHT}}
=== "Python"
{{SPOT_LIGHT_PYTHON}}
**Directional Light**:
=== "C++"
{{DIRECTIONAL_LIGHT}}
=== "Python"
{{DIRECTIONAL_LIGHT_PYTHON}}
## **I want to visualize a target configuration of my robot, is this possible?**

Yes this is possible. RobotDART gives the ability to create a clone of your robot that has no physics, no contacts, just visuals:
=== "C++"
{{ROBOT_GHOST}}
=== "Python"
{{ROBOT_GHOST_PYTHON}}

## **How can I control my robot ?**
*PD control*
=== "C++"
{{PD_CONTROL}}
=== "Python"
{{PD_CONTROL_PYTHON}}
*Simple control*
=== "C++"
{{SIMPLE_CONTROL}}
=== "Python"
{{SIMPLE_CONTROL_PYTHON}}
*Robot control*
=== "C++"
{{ROBOT_CONTROL}}
=== "Python"
{{ROBOT_CONTROL_PYTHON}}
## **Is there a way to control the simulation timestep?**
When creating a RobotDARTSimu object you choose the simulation timestep:
=== "C++"
{{INIT_SIMU}}
=== "Python"
{{INIT_SIMU_PYTHON}}
which can later be modified by:
=== "C++"
{{MODIFY_SIMU_DT}}
=== "Python"
{{MODIFY_SIMU_DT_PYTHON}}
## **I want to simulate a mars environment, is it possible to change the gravitational force of the simulation environment?**

Yes you can modify the gravitational forces 3-dimensional vector of the simulation:
=== "C++"
{{SIMU_GRAVITY}}
=== "Python"
{{SIMU_GRAVITY_PYTHON}}
## **Which collision detectors are available? What are their differences? How can I choose between them?**

| DART | FCL | ODE | Bullet |
|-------------------------------|-----|-----|--------|
| Support only basic shapes     | Full-featured collision detector fully integrated by DART | External collision detector of ODE | External collision detector of Bullet |
| This is building along with DART | This is a required dependency of DART | Needs an external library | Needs an external library |
| Very fast for small scenes | Accurate detailed collisions, but not very fast | Fast collision detection (the integration is not complete) | Fast and accurate collision detection (works well for wheels as well) |

We can easily select one of the available collision detectors using the simulator object:
=== "C++"
{{SET_COLLISION_DETECTOR}}
=== "Python"
{{SET_COLLISION_DETECTOR_PYTHON}}

## **My robot does not self-collide. How can I change this?**

One possible cause may be the fact that self collision is disabled, you can check and change this:
=== "C++"
{{SELF_COLLISIONS}}
=== "Python"
{{SELF_COLLISIONS_PYTHON}}
## **How can I compute kinematic/dynamic properties of my robot (e.g., Jacobians, Mass Matrix)?**

**Kinematic Properties:**
=== "C++"
{{KINEMATICS}}
=== "Python"
{{KINEMATICS_PYTHON}}
**Dynamic Properties:**
=== "C++"
{{DYNAMICS}}
=== "Python"
{{DYNAMICS_PYTHON}}

## **Is there a way to change the joint properties (e.g., actuation, friction)?**

There are 6 types of actuators available, you can set the same actuator to multiple joints at once, or you can set each sensor separately:
=== "C++"
{{SET_ACTUATOR}}
=== "Python"
{{SET_ACTUATOR_PYTHON}}
To enable position and velocity limits for the actuators:
=== "C++"
{{POSITIONS_ENFORCED}}
=== "Python"
{{POSITIONS_ENFORCED_PYTHON}}
Every DOF's limits (position, velocity, acceleration, force) can be modified:
=== "C++"
{{MODIFY_LIMITS}}
=== "Python"
{{MODIFY_LIMITS_PYTHON}}
You can also modify the damping coefficients, coulomb frictions and spring stiffness of every DOF:
=== "C++"
{{MODIFY_COEFFS}}
=== "Python"
{{MODIFY_COEFFS_PYTHON}}
## **What are the supported sensors? How can I use an IMU?**

Sensors in RobotDART can be added only through the simulator object. All of the sensors can be added without being attached to any body or joint but some of them can operate only when attached to something (e.g. `ForceTorque` sensors).

#### **Torque sensor**
Torque sensors can be added to every joint of the robot:
=== "C++"
{{TORQUE_SENSOR}}
=== "Python"
{{TORQUE_SENSOR_PYTHON}}

Torque sensors measure the torque $\tau \in \rm I\!R^n$ of the attached joint (where $n$ is the DOFs of the joint):
=== "C++"
{{TORQUE_MEASUREMENT}}
=== "Python"
{{TORQUE_MEASUREMENT_PYTHON}}
#### **Force-Torque sensor**

Force-Torque sensors can be added to every joint of the robot:
=== "C++"
{{FORCE_TORQUE_SENSOR}}
=== "Python"
{{FORCE_TORQUE_SENSOR_PYTHON}}

Torque sensors measure the force $\boldsymbol{F} \in \rm I\!R^3$, the torque $\boldsymbol{\tau} \in \rm I\!R^3$ and the wrench $\boldsymbol{\mathcal{F}} =\begin{bmatrix} \tau, F\end{bmatrix}\in \rm I\!R^6$ of the attached joint:
=== "C++"
{{FORCE_TORQUE_MEASUREMENT}}
=== "Python"
{{FORCE_TORQUE_MEASUREMENT_PYTHON}}

#### **IMU sensor**

IMU sensors can be added to every link of the robot:
=== "C++"
{{IMU_SENSOR}}
=== "Python"
{{IMU_SENSOR_PYTHON}}

IMU sensors measure the angular position vector $\boldsymbol{\theta} \in \rm I\!R^3$, the angular velocity $\boldsymbol{\omega} \in \rm I\!R^3$  and the linear acceleration $\boldsymbol{\alpha} \in \rm I\!R^3$ of the attached link:
=== "C++"
{{IMU_MEASUREMENT}}
=== "Python"
{{IMU_MEASUREMENT_PYTHON}}
#### **RGB sensor**

Any camera can be used as an RGB sensor:
=== "C++"
{{RGB_SENSOR}}
=== "Python"
{{RGB_SENSOR_PYTHON}}
We can easily save the image and/or transform it to grayscale:
=== "C++"
{{RGB_SENSOR}}
=== "Python"
{{RGB_SENSOR_PYTHON}}

#### **RGB_D sensor**

Any camera can also be configured to also record depth:
=== "C++"
{{CAMERA_SENSOR_RGBD_RECORD_DEPTH}}
=== "Python"
{{CAMERA_SENSOR_RGBD_RECORD_DEPTH_PYTHON}}
We can then read the RGB and depth images:
=== "C++"
{{RGB_D_SENSOR}}
=== "Python"
{{RGB_D_SENSOR_PYTHON}}

We can save the depth images as well:
=== "C++"
{{RGB_D_SENSOR_MEASURE}}
=== "Python"
{{RGB_D_SENSOR_MEASURE_PYTHON}}

## **How can I spawn multiple robots in parallel?**

### Robot Pool (only in C++)

The best way to do so is to create a Robot pool. With a robot pool you:

- Minimize the overhead of loading robots (it happens only once!) or cloning robots (it never happens)
- Make sure that your robots are "clean" once released from each thread
- Focus on the important stuff rather than handling robots and threads

Let's see a more practical example:

- First we need to include the proper header:

=== "C++"
{{ROBOT_POOL_INCLUDE}}

- Then we create a `creator` function and the pool object:

=== "C++"
{{ROBOT_POOL_GLOBAL_NAMESPACE}}

The `creator` function is the function responsible for loading your robot. This should basically look like a standalone code to load or create a robot.

- Next, we create a few threads that utilize the robots (in your code you might be using OpenMP or TBB):

=== "C++"
{{ROBOT_POOL_CREATE_THREADS}}

- An example evaluation function:

=== "C++"
{{ROBOT_POOL_EVAL}}

### Python

We have not implemented this feature in `Python` yet. One can emulate the `RobotPool` behavior or create a custom parallel robot loader.

## **I need to simulate many worlds with camera sensors in parallel. How can I do this?**

Below you can find an example showcasing the use of many worlds with camera sensors in parallel.

=== "C++"
{{CAMERAS_PARALLEL}}
=== "Python"
{{CAMERAS_PARALLEL_PYTHON}}

In C++ you are also able to pre-allocate a custom number of OpenGL contexts so that you can take advantage of stronger GPUs.

## **I do not know how to use waf. How can I detect RobotDART from CMake?**

You need to use `waf` to build RobotDART, but when installing the library a CMake module is installed. Thus it is possible use RobotDART in your code using CMake. You can find a complete example at [cmake/example](https://github.com/resibots/robot_dart/blob/master/cmake/example). In short the CMake would look like this:

### Prerequisites:

- Ensure `RobotDart` and `Magnum` are installed.
- If `RobotDart` and `Magnum` are not installed in standard locations, you can either:
  - Set the `CMAKE_PREFIX_PATH` environment variable:
    ```sh
    export CMAKE_PREFIX_PATH=/opt/robot_dart:/opt/magnum
    ```
  - Or, specify the paths directly when running `cmake`:
    ```sh
    cmake -DRobotDART_DIR=/opt/robot_dart -DMagnum_DIR=/opt/magnum ..
    ```

``` cmake
cmake_minimum_required(VERSION 3.10 FATAL_ERROR)
project(robot_dart_example)
# we ask for Magnum because we want to build the graphics
find_package(RobotDART REQUIRED OPTIONAL_COMPONENTS Magnum)

add_executable(robot_dart_example example.cpp)

target_link_libraries(robot_dart_example
   RobotDART::Simu
)

if(RobotDART_Magnum_FOUND)
  add_executable(robot_dart_example_graphics example.cpp)
  target_link_libraries(robot_dart_example_graphics
    RobotDART::Simu
    RobotDART::Magnum
  )
endif()
```

## **Where can I find complete working examples for either c++ or python?**

!!! note "[C++ examples](https://github.com/NOSALRO/robot_dart/tree/master/src/examples)"

!!! note "[Python examples](https://github.com/NOSALRO/robot_dart/tree/master/src/examples/python)"
