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
# Supported robots

Every robot is a defined as a URDF, which will be installed `$PREFIX/share/utheque`. All robots have pre-defined "robot classes" that define sensors and other properties; for your custom/new robots, you will have to add the sensors/properties via the generic robot class (or create a new robot class).

The URDF files are loaded using the following rules (see `utheque::path()`):

- First check in the current directory
- If not found, check in `current_directory/utheque`
- If not found, check in `$ROBOT_DART_PATH/utheque`
- If not found, check in the robot dart installation path/robots (e.g., `/usr/share/utheque` or `$HOME/share/utheque`)
- Otherwise, report failure

`utheque` is a separate header-only library that gets installed together with RobotDART (or even alone), that can be used in libraries that do not want to interfere with RobotDART and use the curated URDF files.

## Talos (PAL Robotics)
![Talos humanoid robot](images/talos.png){: style="display: block;margin-left: auto;margin-right: auto;width:70%"}

Talos is a humanoid robot made by [PAL Robotics](https://pal-robotics.com/robots/talos/).

- Datasheet: [[pdf](https://pal-robotics.com/wp-content/uploads/2019/07/Datasheet_TALOS.pdf)]
- 32 degrees of freedom (6 for each leg, 7 for each arm, 2 for the waist, 2 for the neck, 1 for each gripper)
- 175 cm / 95 kg
- IMU in the torso
- Torque sensors in all joints except head, wrist and gripper (22 torque sensors total)
- 1 force/torque sensor in each ankle
- 1 force/torque sensor in each wrist

We have two URDF files:

- `robots/talos/talos.urdf` :
    * accurate (simplified but made of polygons) collision meshes
    * mimic joints for the gripper
    * Not compatible the DART collision detector (you need to use FCL collision detector - shipped with DART)
    * URDF: [[talos.urdf](https://github.com/resibots/robot_dart/blob/master/utheque/talos/talos.urdf)]
    * Example: [[talos.cpp](https://github.com/resibots/robot_dart/blob/master/src/examples/talos.cpp)]

??? "Load Talos"
{{TALOS}}

- `robot/talos/talos_fast.urdf`:
    * no collision except for the feet, which are approximated by boxes
    * grippers are fixed (no movement is allowed)
    * compatible with the DART collision detector
    * URDF: [[talos_fast.urdf](https://github.com/resibots/robot_dart/blob/master/utheque/talos/talos_fast.urdf)]
    * Example: [[talos_fast.cpp](https://github.com/resibots/robot_dart/blob/master/src/examples/talos_fast.cpp)]

`talos_fast.urdf` is faster because it makes it possible to use the DART collision detector (and has much collision shapes). You should prefer it except if you want to use the grippers (e.g., for manipulation) or are working on self-collisions.
??? "Load Talos Fast"
{{TALOS_FAST}}

*Please note that the mesh files (.glb) require assimp 5.x (and not assimp4.x usually shipped with ROS). If you cannot load the URDF, please check your assimp version.*

## Panda (Franka Emika)
![Franka Panda manipulator](images/franka.png){: style="display: block;margin-left: auto;margin-right: auto;width:70%"}

The Franka is a modern manipulator made by [Franka Emika Panda](https://www.franka.de/technology). It is commonly found in many robotics labs.

- Datasheet: [[pdf](https://www.generationrobots.com/media/panda-franka-emika-datasheet.pdf)]
- 7 degrees of freedom
- Can be controlled in torque
- 18 kg
- workspace: 855 mm (horizontal), 1190 mm (vertical)
- URDF: [[franka.urdf](https://github.com/resibots/robot_dart/blob/master/utheque/franka/franka.urdf)]
- Example: [[franka.cpp](https://github.com/resibots/robot_dart/blob/master/src/examples/franka.cpp)]
The URDF includes the gripper.


??? "Load Franka"
{{FRANKA}}
    

## LBR iiwa (KUKA)
![LBR iiwa robot](images/iiwa.png){: style="display: block;margin-left: auto;margin-right: auto;width:70%"}

The LBR iiwa is manufactured by [KUKA](https://www.kuka.com/en-de/products/robot-systems/industrial-robots/lbr-iiwa). It is similar to the Panda and is also very common in robotics labs.

- Datasheet: [[pdf](https://www.kuka.com/-/media/kuka-downloads/imported/6b77eecacfe542d3b736af377562ecaa/db_lbr_iiwa_en.pdf?rev=70ca3521eecc42b9b1a05d0e486119ba&hash=C3DDC2EFE649EFDF75E592892C466D6F)]
- We implement the 14 kg version
- 29.5 kg
- 7 degrees of freedom
- URDF: [[iiwa.urdf](https://github.com/resibots/robot_dart/blob/master/utheque/iiwa/iiwa.urdf)]
- Example: [[iiwa.cpp](https://github.com/resibots/robot_dart/blob/master/src/examples/iiwa.cpp)]

??? "Load LBR iiwa"
{{IIWA}}

## iCub (IIT)
![iCub humanoid robot](images/icub.png){: style="display: block;margin-left: auto;margin-right: auto;width:70%"}

The iCub is a open source humanoid robot made by the [Instituto Italiano di Tecnologia](https://icub.iit.it). There are currently 42 iCUbs in the world, and many versions.

- Datasheet (rev 2.3) [[pdf](https://icub.iit.it/storage/documents/Technical-specs_iCub_robot_Rev_2.3_05082019.pdf)]
- 6 force/torque sensors (upper arms, upper legs, ankles)
- IMU in the head
- We do to simulate the skin
- We do not simulate the hands
- Our model is close to the [Inria's iCub](https://members.loria.fr/JBMouret/robots.html), but it has not been checked in detail.
- URDF: [[icub.urdf](https://github.com/resibots/robot_dart/blob/master/utheque/iiwa/icub.urdf)]
- Example [[icub.cpp](https://github.com/resibots/robot_dart/blob/master/src/examples/icub.cpp)]

*Please note that the mesh files (.glb) require assimp 5.x (and not assimp4.x usually shipped with ROS). If you cannot load the URDF, please check your assimp version.*
??? "Load iCub"
{{LOAD_IICUB}}
??? "Print IMU sensor measurements"
{{ICUB_PRINT_IMU}}
??? "Print Force-Torque sensor measurements"
{{ICUB_PRINT_FT}}
## Unitree A1
![Unitree A1 quadruped robot](images/a1.png){: style="display: block;margin-left: auto;margin-right: auto;width:70%"}

A1 is a quadruped robot made by the [Unitree Robotics](https://www.unitree.com/products/a1/).

<!-- - Depth camera in the head -->
- IMU in the torso
- We do not simulate the foot pressure sensors (yet)
- One can easily add a depth camera on the head
- URDF: [[a1.urdf](https://github.com/resibots/robot_dart/blob/master/utheque/unitree_a1/a1.urdf)]
- Example [[a1.cpp](https://github.com/resibots/robot_dart/blob/master/src/examples/a1.cpp)]
??? "Load A1"
{{A1}}

??? "Print IMU sensor measurements"
{{A1_PRINT_IMU}}

??? "Add a depth camera on the head"
    [How can I attach a camera to a moving link?](../faq/#how-can-i-attach-a-camera-to-a-moving-link)

*Please note that the mesh files (.glb) require assimp 5.x (and not assimp4.x usually shipped with ROS). If you cannot load the URDF, please check your assimp version.*

## Dynamixel-based hexapod robot (Inria and others)
![hexapod robot](images/hexapod.png){: style="display: block;margin-left: auto;margin-right: auto;width:70%"}

This hexapod is a simple 6-legged robot based on dynamixel actuators. It is similar to the robot used in the paper [`Robots that can adapt like animals'](https://www.nature.com/articles/nature14422) (Cully et al., 2015).

- 6 legs, 3 degrees of freedom for each leg (18 degrees of freedom)
- simple URDF (no meshes)
- URDF: [[pexod.urdf](https://github.com/resibots/robot_dart/blob/master/utheque/pexod.urdf)]
- Example: [[hexapod.cpp](https://github.com/resibots/robot_dart/blob/master/src/examples/hexapod.cpp)]

??? "Load Hexapod"
{{HEXAPOD}}

??? "Load Pexod"
{{HELLO_WORLD_ROBOT_CREATION}}

## Simple arm
![simple arm robot](images/arm.png){: style="display: block;margin-left: auto;margin-right: auto;width:70%"}

- A simple arm for educational or debugging purposes
- 5 degrees of freedom
- simple URDF (no meshes)
- URDF: [[arm.urdf](https://github.com/resibots/robot_dart/blob/master/utheque/arm.urdf)]
- Example: [[arm.cpp](https://github.com/resibots/robot_dart/blob/master/src/examples/arm.cpp)]

??? "Load Simple Arm"
{{SIMPLE_ARM}}
## Loading Custom Robots
RobotDART gives you the ability to load custom robots  that are defined in [URDF](http://wiki.ros.org/urdf/Tutorials), [SDF](https://classic.gazebosim.org/tutorials?tut=build_model#ComponentsofSDFModels), [SKEL](https://dartsim.github.io/skel_file_format.html) or [MJCF](https://mujoco.readthedocs.io/en/latest/modeling.html#mjcf-mechanisms) files. For example, you can load a urdf model using:

??? "Load custom Robot"
    ``` c++
        auto your_robot = std::make_shared<robot_dart::Robot>("path/to/model.urdf");
    ```
??? "Load custom Robot with packages (e.g STL, DAE meshes)"
    ``` c++
        std::vector<std::pair<std::string, std::string>> your_model_packages = {{"model", "path/to/model/dir"}};
        auto your_robot = std::make_shared<robot_dart::Robot>("path/to/model.urdf", your_model_packages, "packages");
    ```
