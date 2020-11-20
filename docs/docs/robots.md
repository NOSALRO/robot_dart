# Supported robots

Every robot is a defined as a URDF, which will be installed `$PREFIX/shared/robot_dart/robots`. Some robots (Talos, iCub) ehave predefined "Robot classes" that defines the sensors; for the others, you have to add the sensors to the generic robot class.

The URDF files are loaded using the following rule:
- First check in the current directory
- If not found, check in `current_directory/robots`
- If not found, check in the robot dart installation path/robots (e.g., `/usr/share/robot_dart/robots` or `$HOME/share/robot_dart/robots`)
- Otherwise, report failure


## Talos (PAL Robotics)
![Placeholder](images/talos.png){: style="display: block;margin-left: auto;margin-right: auto;width:70%"}

Talos is a humanoid robot made by [PAL Robotics](https://pal-robotics.com/robots/talos/).

- Datasheet: [pdf](https://pal-robotics.com/wp-content/uploads/2019/07/Datasheet_TALOS.pdf)
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
    * Not compatible the DART collision detector (you need to use FCLe
- `robot/talos/talos_fast.urdf`:
    * no collision except for the feet, which are approximate by boxes
    * grippers are fixed
    * compatible with the DART collision detector

`talos_fast.urdf` is faster because it makes it possible to use the DART collision detector. You should prefer it except if you want to use the grippers or are working on self-collisions.

*Please note that the mesh files (.glb) require assimp 5.x (and not assimp4.x shipped with ROS). If you cannot load the URDF, please check your assimp version.*


## Panda (Franka Emika)
![Placeholder](images/franka.png){: style="display: block;margin-left: auto;margin-right: auto;width:70%"}

    * [Franka Emika Panda](https://www.franka.de/technology)

## LBR Iiwa (KUKA)
![Placeholder](images/iiwa.png){: style="display: block;margin-left: auto;margin-right: auto;width:70%"}

    * [KUKA LBR Iiwa](https://www.kuka.com/en-de/products/robot-systems/industrial-robots/lbr-iiwa) (14kg version)

## Icub (IIT)
![Placeholder](images/icub.png){: style="display: block;margin-left: auto;margin-right: auto;width:70%"}

    * [IIT iCub humanoid](https://icub.iit.it) (without hands)

## Dynamixel-based hexapod robot (Inria and others)
![Placeholder](images/hexapod.png){: style="display: block;margin-left: auto;margin-right: auto;width:70%"}

    * [Pexod hexapod](https://www.resibots.eu/photos.html#pexod-robot) ([ResiBots ERC project](https://www.resibots.eu/index.html))

## Simple arm
![Placeholder](images/arm.png){: style="display: block;margin-left: auto;margin-right: auto;width:70%"}

    * A simple arm for educational purposes

## How to use your own URDF?
