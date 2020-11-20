# Welcome to robot\_dart's Documentation

robot\_dart is a **C++11 robot simulator** built on top of the [DART] physics engine. The robot\_dart simulator is **intended to be used by *Robotics and Machine Learning researchers*** that want to quickly write controllers or test learning algorithms **without delays and overhead** that usually comes with other simulators (e.g., [Gazebo]). For this reason, **the simulator runs headless by default**, and there is also the possibility of *rendering the scene (e.g., through a camera sensor) without opening a graphics window*. All robot\_dart's code is **thread-safe** (including *graphics and camera sensors*), and thus enables its users to **use their code in parallel jobs in clusters** or any other container.

![Placeholder](images/iiwa.png){: style="display: block;margin-left: auto;margin-right: auto;width:70%"}

## Main Features

* Modern C++ code that makes it easy to develop environments and applications
* **Fast and reliable simulation** of robotic mechanisms and their interactions (through the [DART] physics engine)
* A structured `Robot` class that enables a unified creation and access to all important values: in robot\_dart you can load any robot description file (URDF, SDF, SKEL, and MuJoCo files) with the same command, and all robot measurements can be queried without using any DART code
* A generic `RobotControl` class that enables fast prototyping of any type of controller
* A generic `Sensor` class that allows the creation of any kind of sensor
* A growing list of already implemented **sensors**, that includes 6-axis `ForceTorque`, `IMU`, `RGB`, and `RGB-D` sensors
* A simulation class (`RobotDARTSimu`) that handles multiple robots and sensors, and allows for step-by-step simulation
* A growing list of supported robots along with edited and optimized models to be used with robot\_dart (see the [robots page](robots)):
    * [PAL Talos humanoid](https://pal-robotics.com/robots/talos/) (with and without hands)
    * [Franka Emika Panda](https://www.franka.de/technology)
    * [KUKA LBR Iiwa](https://www.kuka.com/en-de/products/robot-systems/industrial-robots/lbr-iiwa) (14kg version)
    * [IIT iCub humanoid](https://icub.iit.it) (without hands)
    * [Pexod hexapod](https://www.resibots.eu/photos.html#pexod-robot) ([ResiBots ERC project](https://www.resibots.eu/index.html))
    * A simple arm for educational purposes
    * **Easy** to add new robots...
* A **custom graphical interface** built on top of [Magnum] that allows generic customization
* Support for **windowless OpenGL context creation** (even in parallel threads!) to allow for camera sensor usage even in parallel jobs running on clusters
* Support for **video recording in simulation time** (i.e., not affected by delays of simulator and/or graphics) for visualization or debugging purposes
* Full-featured **Python bindings** for fast prototyping
* robot\_dart runs on any Linux distribution and Mac OS

[DART]: http://dartsim.github.io/
[Magnum]: http://magnum.graphics
[Gazebo]: http://gazebosim.org/
