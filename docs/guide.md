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

// re-initialize all controllers
my_robot->reinit_controllers();
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
// for DOF index dof
my_robot->set_actuator_type(size_t dof, dart::dynamics::Joint::ActuatorType type);

// change actuator type
// same for all DOFs
my_robot->set_actuator_types(dart::dynamics::Joint::ActuatorType type);

// change actuator type
// one for each DOF
my_robot->set_actuator_types(const std::vector<dart::dynamics::Joint::ActuatorType>& types);

// set damping coefficients
// for DOF index dof
my_robot->set_damping_coeff(size_t dof, double damp);

// set damping coefficients
// same for all DOFs
my_robot->set_damping_coeff(double damp);

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
my_robot->set_position_enforced(size_t dof, bool enforced);
my_robot->set_position_enforced(bool enforced);
my_robot->set_position_enforced(const std::vector<bool>& enforced);

// get attached damages
my_robot->damages();
```

### RobotControl Class

This is an abstract class that should serve as a base when creating controllers in robot\_dart. Examples of already implemented controllers can be found in the [control directory](../src/robot_dart/control).

#### Main methods

**Constructor**

```cpp
RobotControl::RobotControl(const std::vector<double>& ctrl, bool full_control = false);
```

- *ctrl* is the parameter vector
- *full_control* defines whether we should control all the DOFs or not; this has an actual point only when our robot can freely move around the world (i.e., this gives us the ability to control the first 6 DOFs)

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

// get if controller is active
// if it's not active, you should update the parameters
// and re-initialize
bool active() const;

// helper functions for full control
bool fully_controlled() const;
void set_full_control(bool enable);

// helper functions for weight
double weight() const;
void set_weight(double weight);
```

#### Abstract methods

All of the following methods, you **need** to override them when you are creating a new controller.

```cpp
// method that initializes the controller
void configure();

// method that computes the actual commands
// you should return an Eigen::VectorXd
// of size of _control_dof
Eigen::VectorXd calculate(double t);

// method that properly clones the controller
std::shared_ptr<RobotControl> clone() const;
```

### RobotDARTSimu Class

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

In *RobotDARTSimu* there's the possibility of attaching **descriptors**. *Descriptors* are functors that can be used either to log information/data (e.g., state trajectories) or to trigger a stop in the simulation (e.g., a NaN value or safety reasons). All descriptors must **inherit** from `robot_dart::descriptor::BaseDescriptor`:

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

*desc_dump* defines the number of steps at which the descriptor should be called and you need to overload the functor operator to add your functionality. If you want to create a safety mechanism, you can do the following:

```cpp
struct MySafety : public robot_dart::descriptor::BaseDescriptor {
    MySafety(const robot_dart::RobotDARTSimu& simu, size_t desc_dump = 1) : robot_dart::descriptor::BaseDescriptor(simu, desc_dump) {}

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

*Retrieving, removing and adding descriptors to a simulation can be done with the following `RobotDARTSimu` functions*:

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
void add_floor(double floor_width, double floor_height, const Eigen::Vector6d& pose, const std::string& floor_name);
```