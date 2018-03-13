# robot_dart guide

In this short guide, a short introduction to the main functionalities of robot\_dart is provided.

## Robot Class

### Load a new robot from file

*Load robot from URDF, sdf or SKEL file:*

```cpp
auto my_robot = std::make_shared<robot_dart::Robot>(
    "relative_or_absolute_path_to_file", [name_to_give_to_skeleton], [vector_of_damages]);
```

*The default skeleton name is "robot".*

**Attention:** *if you want to load a SKEL file, there are two options:*

- The SKEL file contains a skeleton, then the above code should work fine
- The SKEL file contains a full world and you want to load the skeleton by the name "my_robot":

```cpp
auto my_robot = std::make_shared<robot_dart::Robot>("path_to_skel_file", "my_robot", [vector_of_damages]);
```

### Create a robot from an already defined Skeleton


```cpp
dart::dynamics::SkeletonPtr my_skeleton = ...;

auto my_robot = std::make_shared<robot_dart::Robot>(
    my_skeleton, [name_to_give_to_skeleton], [vector_of_damages]);
```

### Controllers

Multiple controllers can be attached to a robot, each with their own weight. Here's the basic functionality:

```cpp
// add new controller with weight
// ctrl_weight defaults to 1.0
void add_controller(const std::shared_ptr<robot_dart::control::RobotControl>& controller, [ctrl_weight]);

// remove controller by pointer
void remove_controller(const std::shared_ptr<robot_dart::control::RobotControl>& controller);

// remove controller by index
void remove_controller(size_t index);

// remove all controllers
void clear_controllers();

// get all controllers
std::vector<std::shared_ptr<robot_dart::control::RobotControl>> controllers() const;

// get controller by index
std::shared_ptr<robot_dart::control::RobotControl> controller(size_t index) const;

// get active controllers
std::vector<std::shared_ptr<robot_dart::control::RobotControl>> active_controllers() const;

// get number of controllers
size_t num_controllers() const;

// re-initialize all controllers
// this is automatically called when
// robot->fix_to_world() or robot->free()
// are called. You should also call it
// when the structure of the robot changed
// (i.e., you added a new DOF)
void reinit_controllers();
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
std::shared_ptr<robot_dart::control::RobotControl>
get_simple(const std::shared_ptr<robot_dart::Robot>& my_robot) {
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

### Helper functionality

**Clone robot**

```cpp
std::shared_ptr<robot_dart::Robot> my_robot = ...;
auto cloned_robot = my_robot->clone();
```

This function will clone the robot using DART's cloning functionality (e.g., it will not clone the full visual shapes).

**Fixing/freeing from world**

```cpp
// fix the robot to the world
// remove the first 6 DOFs
void fix_to_world();

// free from world
// add back the first 6 DOFs
void free_from_world();

// check if robot is fixed
bool fixed() const;

// check if robot can freely move
bool free() const;
```

**Actuators**

```cpp
// change actuator type
// for DOF index dof
void set_actuator_type(size_t dof, dart::dynamics::Joint::ActuatorType type);

// change actuator type
// same for all DOFs
void set_actuator_types(dart::dynamics::Joint::ActuatorType type);

// change actuator type
// one for each DOF
void set_actuator_types(const std::vector<dart::dynamics::Joint::ActuatorType>& types);

// set damping coefficients
// for DOF index dof
void set_damping_coeff(size_t dof, double damp);

// set damping coefficients
// same for all DOFs
void set_damping_coeff(double damp);

// set damping coefficients
// one for each DOF
void set_damping_coeff(const std::vector<double>& damps);
```

**Other functionalities**

```cpp
// get underlying skeleton pointer
std::shared_ptr<dart::dynamics::Skeleton> skeleton();

// get robot name
std::string name() const;

// enforce position limits
void set_position_enforced(size_t dof, bool enforced);
void set_position_enforced(bool enforced);
void set_position_enforced(const std::vector<bool>& enforced);
```

## RobotControl Class

This is an abstract class that should serve as a base when creating controllers in robot\_dart. Examples of already implemented controllers can be found in the [control directory](../src/robot_dart/control).

### Main methods

**Constructor**

```cpp
RobotControl::RobotControl(const std::vector<double>& ctrl, bool full_control = false);
```

- *ctrl* is the parameter vector
- *full_control* defines whether we should control all the DOFs or not; this is only relevant when our robot can freely move around the world (i.e., this gives us the ability to control the first 6 DOFs)

**Parameters**

```cpp
// change/set the parameters
void set_parameters(const std::vector<double>& ctrl);

// get parameters
std::vector<double> parameters() const;
```

**Other functionality**

```cpp
// set the parent robot
// this is automatically done when you add a controller to a robot
void set_robot(const std::shared_ptr<robot_dart::Robot>& robot);
// get the parent robot
std::shared_ptr<robot_dart::Robot> robot() const;

// activate or deactivate the controller
// enable == true, then it tries to activate
// the controller (i.e., tries to initialize it)
void activate(bool enable = true);

// get if controller is active
// if it's not active, you should update the parameters
// and re-initialize
bool active() const;

// helper functions for full control
bool fully_controlled() const;
void set_full_control(bool enable);

// helper functions for controller's weight
double weight() const;
void set_weight(double weight);
```

### Abstract methods

All of the following methods, you **need** to override them when you are creating a new controller.

```cpp
// method that initializes the controller
// you should make sure that you set _active
// to true if everything went ok (i.e., the
// controller parameters are well-behaved)
void configure();

// method that computes the actual commands
// you should return an Eigen::VectorXd
// with a size equal to _control_dof
Eigen::VectorXd calculate(double t);

// method that properly clones the controller
std::shared_ptr<RobotControl> clone() const;
```

## RobotDARTSimu Class

This is the simulator class. It handles and aids in configuration of a DART simulation in robot\_dart.

**Constructor**

```cpp
// initialize a simulation with a certain time step
RobotDARTSimu::RobotDARTSimu(double time_step = 0.015);
```

**Running a simulation**

```cpp
// run a simulation for max_duration seconds
void run(double max_duration);
```

**Robots**

```cpp
// add a robot
void add_robot(const std::shared_ptr<robot_dart::Robot>& robot);

// remove a robot by pointer
void remove_robot(const std::shared_ptr<robot_dart::Robot>& robot);

// remove a robot by index
void remove_robot(size_t index);

// remove all robots
void clear_robots();

// number of robots
size_t num_robots() const;

// get vector of robots
std::vector<td::shared_ptr<robot_dart::Robot>> robots() const;

// get robot by index
std::shared_ptr<robot_dart::Robot> robot(size_t index) const;
```

**Descriptors**

In *RobotDARTSimu* there's the possibility of attaching **descriptors**. *Descriptors* are functors that can be used to log information/data (e.g., state trajectories) or to stop the simulation (e.g., a NaN value was produced or for safety reasons). All descriptors must **inherit** from `robot_dart::descriptor::BaseDescriptor`:

```cpp
struct BaseDescriptor {
public:
    BaseDescriptor(const RobotDARTSimu& simu, size_t desc_dump = 1);

    virtual void operator()() = 0;

    size_t desc_dump() const;
    void set_desc_dump(size_t desc_dump);

protected:
    const RobotDARTSimu& _simu;
    size_t _desc_period;
};
```

**The descriptor will be called every `desc_dump` steps of the simulation.**

You need to overload the functor operator to add your functionality. If you want to create a safety mechanism, you can do the following:

```cpp
struct MySafety : public robot_dart::descriptor::BaseDescriptor {
    MySafety(const robot_dart::RobotDARTSimu& simu, size_t desc_dump = 1)
        : robot_dart::descriptor::BaseDescriptor(simu, desc_dump) {}

    void operator()()
    {
        if (_simu.robots().size() > 0) {
            auto my_robot = _simu.robot(0);
            if (my_robot->is_in_unsafe_state())
                _simu.stop_sim();
        }
    }
};
```

Retrieving, removing and adding descriptors to a simulation can be done with the following `RobotDARTSimu` methods:

```cpp
// add a descriptor with type Descriptor
template <typename Descriptor>
void add_descriptor(size_t desc_dump = 1);

// add a descriptor by passing a pointer
void add_descriptor(const std::shared_ptr<descriptor::BaseDescriptor>& desc);

// get all descriptors
std::vector<std::shared_ptr<descriptor::BaseDescriptor>> descriptors() const;

// get descriptor by index
std::shared_ptr<descriptor::BaseDescriptor> descriptor(size_t index) const;

// remove descriptor by pointer
void remove_descriptor(const std::shared_ptr<descriptor::BaseDescriptor>& desc);

// remove descriptor by index
void remove_descriptor(size_t index);

// remove all descriptors
void clear_descriptors();
```

**Other functionality**

```cpp
// get pointer to the underlying DART world
dart::simulation::WorldPtr world();

// get time step
double step() const;

// set a new time step
void set_step(double step);

// add a floor to the world
void add_floor(double floor_width, double floor_height, const Eigen::Vector6d& pose,
    const std::string& floor_name);
```