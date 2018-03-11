## robot_dart guide

In this short guide, a short introduction to the main functionalities of robot\_dart is provided.

### Robot Class

#### Load a new robot from file

*Load robot from URDF, sdf or SKEL file:*

```cpp
auto my_robot = std::make_shared<robot_dart::Robot>("relative_or_absolute_path_to_file", [name_to_give_to_skeleton], [vector_of_damages])
```

*The default skeleton name is "robot".*

**Attention:** *if you want to load a SKEL file, there are two options:*

- The SKEL file contains a skeleton, then the above code should work fine
- The SKEL file contains a full world and you want to load the skeleton by the name "my_robot":

```cpp
auto my_robot = std::make_shared<robot_dart::Robot>("path_to_skel_file", "my_robot", [vector_of_damages])
```

#### Create a robot from an already defined Skeleton


```cpp
dart::dynamics::SkeletonPtr my_skeleton = ...;

auto my_robot = std::make_shared<robot_dart::Robot>(my_skeleton, [name_to_give_to_skeleton], [vector_of_damages])
```

#### Controllers

In each robot you can have attached multiple controllers with different weights. Here's the basic functionality:

```cpp
std::shared_ptr<robot_dart::Robot> my_robot = ...;

// add new controller with weight
std::shared_ptr<robot_dart::control::RobotControl> my_controller = ...; // create controller
// ctrl_weight defaults to 1.0
my_robot->add_controller(my_controller, [ctrl_weight]);

// remove controller by pointer
my_robot->remove_controller(my_controller);

// remove controller by index
my_robot->remove_controller(0);

// remove all controllers
my_robot->clear_controllers();

// get all controllers
std::vector<std::shared_ptr<robot_dart::control::RobotControl>> ctrls = my_robot->controllers();

// get controller by index
auto my_controller = my_robot->controller(0);

// get active controllers
std::vector<std::shared_ptr<robot_dart::control::RobotControl>> ctrls = my_robot->active_controllers();

// get number of controllers
size_t num_ctrls = my_robot->num_controllers();
```

**Retrieving controller with specific type**

Most of the times, the controllers will have some additional functionality over the one needed to update them. For example, [robot_dart::control::PDControl](../src/robot_dart/control/pd_control.hpp) provides a method for changing the PD gains. Here's how to retrieve a controller with specific type:

```cpp
std::shared_ptr<robot_dart::Robot> my_robot = ...;

// add a PD-controller
std::vector<double> ctrl = ...;
my_robot->add_controller(std::make_shared<robot_dart::control::PDControl>(ctrl));

// retrieve controller with specific type
auto my_pd_control = std::static_pointer_cast<robot_dart::control::PDControl>(my_robot->controller(0));

// change PD gains
my_pd_control->set_pd(20., 0.);
```

In case you're not sure of the type, you need to use `std::dynamic_pointer_cast`. Here's an example function that returns the first controller with a type of `robot_dart::control:SimpleControl`:

```cpp
std::shared_ptr<robot_dart::control::RobotControl> get_simple(const std::shared_ptr<robot_dart::Robot>& my_robot) {
    // loop through all the controllers
    for(auto ctrl : my_robot->controllers()) {
        auto tmp_ctrl = std::dynamic_pointer_cast<robot_dart::control:SimpleControl>(ctrl);
        // if the type is indeed robot_dart::control:SimpleControl
        if(tmp_ctrl)
            return tmp_ctrl;
    }

    // we did not find a controller with type robot_dart::control:SimpleControl
    return nullptr;
}
```

#### Helper functionality

**Clone robot**

```cpp
std::shared_ptr<robot_dart::Robot> my_robot = ...;
auto cloned_robot = my_robot->clone();
```

This function will clone the robot using DART's cloning functionality (e.g., it will not clone the full visual shapes).

**Fixing/freeing from world**

```cpp
std::shared_ptr<robot_dart::Robot> my_robot = ...;

// fix the robot to the world
// remove the first 6 DOFs
my_robot->fix_to_world();

// free from world
// add back the first 6 DOFs
my_robot->free_from_world();

// check if robot is fixed
my_robot->fixed();

// check if robot can freely move
my_robot->free();
```

**Actuators**

```cpp
std::shared_ptr<robot_dart::Robot> my_robot = ...;

// change actuator type
// same for all DOFs
my_robot->set_actuator_types(dart::dynamics::Joint::ActuatorType type);

// change actuator type
// one for each DOF
my_robot->set_actuator_types(const std::vector<dart::dynamics::Joint::ActuatorType>& types);

// set damping coefficients
// same for all DOFs
my_robot->set_damping_coeff(double damping);

// set damping coefficients
// one for each DOF
my_robot->set_damping_coeff(const std::vector<double>& damps);
```

**Other functionality**

```cpp
std::shared_ptr<robot_dart::Robot> my_robot = ...;

// get underlying skeleton pointer
auto skel_ptr = my_robot->skeleton();

// get robot name
std::string robot_name = my_robot->name();

// enforce position limits
my_robot->set_position_enforced(boolean);

// get attached damages
my_robot->damages();
```