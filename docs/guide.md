## robot_dart tutorial

This tutorial aims at providing a gentle introduction to the robot\_dart library.

### Base skeleton of the code

Here's the basic skeleton of the code:

```cpp
// basic includes
#include <robot_dart/robot_dart_simu.hpp>

// if we want graphics, include the appropriate file
#ifdef GRAPHIC
#include <robot_dart/graphics/graphics.hpp>
#endif

// ....

int main()
{
    // create the robot_dart simulator

    // add some random boxes/spheres

    // add one robot with a controller

    // simulate the world
    return 0;
}
```

### Create box or ellipsoid shapes

In robot\_dart there are some helper functions to create boxes and ellipsoid shapes. By default, robot\_dart uses the DART collision detector that supports collision detection only for basic shapes (boxes, spheres, etc.); for that reason, we will create only boxes and spheres. Of course you can change the collision detector to another one supported by DART (you can check the [DART documentation](http://dartsim.github.io/) and our `hexapod` example to see how to do it).

```cpp
std::shared_ptr<robot_dart::Robot> random_box(size_t num = 0)
{
    // random pose
    Eigen::Vector6d pose = Eigen::Vector6d::Random();
    // make sure it is above the ground
    pose(5) += 1.5;
    // random size
    Eigen::Vector3d size = Eigen::Vector3d::Random().array() * Eigen::Vector3d(0.1, 0.2, 0.1).array() + 0.3;
    return robot_dart::Robot::create_box(size, pose, "free", 1., dart::Color::Red(1.0), "box_" + std::to_string(num));
}

std::shared_ptr<robot_dart::Robot> random_sphere(size_t num = 0)
{
    // random pose
    Eigen::Vector6d pose = Eigen::Vector6d::Random();
    // make sure it is above the ground
    pose(5) += 1.5;
    // random size
    Eigen::Vector3d size = Eigen::Vector3d::Random()[0] * Eigen::Vector3d(0.2, 0.2, 0.2).array() + 0.3;
    return robot_dart::Robot::create_ellipsoid(size, pose, "free", 1., dart::Color::Blue(1.0), "sphere_" + std::to_string(num));
}
```

### Instantiate the simulator

```cpp
std::srand(std::time(NULL));
// choose time step of 0.001 seconds
robot_dart::RobotDARTSimu simu(0.001);
#ifdef GRAPHIC
simu.set_graphics(std::make_shared<robot_dart::graphics::Graphics>(simu.world()));
// set the camera at position (0, 3, 1) looking at the center (0, 0, 0)
std::static_pointer_cast<robot_dart::graphics::Graphics>(simu.graphics())->look_at({0., 3., 1.});
#endif

// add floor
simu.add_floor(10., 0.2);
```

### Create some random objects

```cpp
// add random objects to the world
for (size_t i = 0; i < 10; i++) {
    simu.add_robot(random_box(i));
    simu.add_robot(random_sphere(i));
}
```

### Add a robot from a URDF file

```cpp
// add a simple arm
auto arm_robot = std::make_shared<robot_dart::Robot>("res/models/arm.urdf");
// pin the arm to world
arm_robot->fix_to_world();
arm_robot->set_position_enforced(true);
```

### Create a PD controller to control the arm

In order to control our robots, we need to create some controllers. One of the most traditional and basic controllers, is a PD-controller.

First, you need to include the proper file:

```cpp
#include <robot_dart/control/pd_control.hpp>
```

Then we need instantiate the controller, set the desired positions and add it to the robot:

```cpp
// add a PD-controller to the arm
// set desired positions
std::vector<double> ctrl = {0.0, 1.0, -1.5, 1.0};
// add the controller to the robot
arm_robot->add_controller(std::make_shared<robot_dart::control::PDControl>(ctrl));

// add the arm to the simulator
simu.add_robot(arm_robot);
```

### Run the simulation

```cpp
// run the simulator for 5 seconds
simu.run(5.);
```

### The final code

The full code of the tutorial can be found in the [source file](../src/examples/tutorial.cpp).