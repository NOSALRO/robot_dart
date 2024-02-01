

# Class robot\_dart::control::RobotControl



[**ClassList**](annotated.md) **>** [**robot\_dart**](namespacerobot__dart.md) **>** [**control**](namespacerobot__dart_1_1control.md) **>** [**RobotControl**](classrobot__dart_1_1control_1_1RobotControl.md)










Inherited by the following classes: [robot\_dart::control::PDControl](classrobot__dart_1_1control_1_1PDControl.md),  [robot\_dart::control::PolicyControl](classrobot__dart_1_1control_1_1PolicyControl.md),  [robot\_dart::control::SimpleControl](classrobot__dart_1_1control_1_1SimpleControl.md)
































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**RobotControl**](#function-robotcontrol-13) () <br> |
|   | [**RobotControl**](#function-robotcontrol-23) (const Eigen::VectorXd & ctrl, bool full\_control=false) <br> |
|   | [**RobotControl**](#function-robotcontrol-33) (const Eigen::VectorXd & ctrl, const std::vector&lt; std::string &gt; & controllable\_dofs) <br> |
|  void | [**activate**](#function-activate) (bool enable=true) <br> |
|  bool | [**active**](#function-active) () const<br> |
| virtual Eigen::VectorXd | [**calculate**](#function-calculate) (double t) = 0<br> |
| virtual std::shared\_ptr&lt; [**RobotControl**](classrobot__dart_1_1control_1_1RobotControl.md) &gt; | [**clone**](#function-clone) () const = 0<br> |
| virtual void | [**configure**](#function-configure) () = 0<br> |
|  const std::vector&lt; std::string &gt; & | [**controllable\_dofs**](#function-controllable_dofs) () const<br> |
|  void | [**init**](#function-init) () <br> |
|  const Eigen::VectorXd & | [**parameters**](#function-parameters) () const<br> |
|  std::shared\_ptr&lt; [**Robot**](classrobot__dart_1_1Robot.md) &gt; | [**robot**](#function-robot) () const<br> |
|  void | [**set\_parameters**](#function-set_parameters) (const Eigen::VectorXd & ctrl) <br> |
|  void | [**set\_robot**](#function-set_robot) (const std::shared\_ptr&lt; [**Robot**](classrobot__dart_1_1Robot.md) &gt; & robot) <br> |
|  void | [**set\_weight**](#function-set_weight) (double weight) <br> |
|  double | [**weight**](#function-weight) () const<br> |
| virtual  | [**~RobotControl**](#function-robotcontrol) () <br> |








## Protected Attributes

| Type | Name |
| ---: | :--- |
|  bool | [**\_active**](#variable-_active)  <br> |
|  bool | [**\_check\_free**](#variable-_check_free)   = = false<br> |
|  int | [**\_control\_dof**](#variable-_control_dof)  <br> |
|  std::vector&lt; std::string &gt; | [**\_controllable\_dofs**](#variable-_controllable_dofs)  <br> |
|  Eigen::VectorXd | [**\_ctrl**](#variable-_ctrl)  <br> |
|  int | [**\_dof**](#variable-_dof)  <br> |
|  std::weak\_ptr&lt; [**Robot**](classrobot__dart_1_1Robot.md) &gt; | [**\_robot**](#variable-_robot)  <br> |
|  double | [**\_weight**](#variable-_weight)  <br> |




















## Public Functions Documentation




### function RobotControl [1/3]

```C++
robot_dart::control::RobotControl::RobotControl () 
```






### function RobotControl [2/3]

```C++
robot_dart::control::RobotControl::RobotControl (
    const Eigen::VectorXd & ctrl,
    bool full_control=false
) 
```






### function RobotControl [3/3]

```C++
robot_dart::control::RobotControl::RobotControl (
    const Eigen::VectorXd & ctrl,
    const std::vector< std::string > & controllable_dofs
) 
```






### function activate 

```C++
void robot_dart::control::RobotControl::activate (
    bool enable=true
) 
```






### function active 

```C++
bool robot_dart::control::RobotControl::active () const
```






### function calculate 

```C++
virtual Eigen::VectorXd robot_dart::control::RobotControl::calculate (
    double t
) = 0
```






### function clone 

```C++
virtual std::shared_ptr< RobotControl > robot_dart::control::RobotControl::clone () const = 0
```






### function configure 

```C++
virtual void robot_dart::control::RobotControl::configure () = 0
```






### function controllable\_dofs 

```C++
const std::vector< std::string > & robot_dart::control::RobotControl::controllable_dofs () const
```






### function init 

```C++
void robot_dart::control::RobotControl::init () 
```






### function parameters 

```C++
const Eigen::VectorXd & robot_dart::control::RobotControl::parameters () const
```






### function robot 

```C++
std::shared_ptr< Robot > robot_dart::control::RobotControl::robot () const
```






### function set\_parameters 

```C++
void robot_dart::control::RobotControl::set_parameters (
    const Eigen::VectorXd & ctrl
) 
```






### function set\_robot 

```C++
void robot_dart::control::RobotControl::set_robot (
    const std::shared_ptr< Robot > & robot
) 
```






### function set\_weight 

```C++
void robot_dart::control::RobotControl::set_weight (
    double weight
) 
```






### function weight 

```C++
double robot_dart::control::RobotControl::weight () const
```






### function ~RobotControl 

```C++
inline virtual robot_dart::control::RobotControl::~RobotControl () 
```



## Protected Attributes Documentation




### variable \_active 

```C++
bool robot_dart::control::RobotControl::_active;
```






### variable \_check\_free 

```C++
bool robot_dart::control::RobotControl::_check_free;
```






### variable \_control\_dof 

```C++
int robot_dart::control::RobotControl::_control_dof;
```






### variable \_controllable\_dofs 

```C++
std::vector<std::string> robot_dart::control::RobotControl::_controllable_dofs;
```






### variable \_ctrl 

```C++
Eigen::VectorXd robot_dart::control::RobotControl::_ctrl;
```






### variable \_dof 

```C++
int robot_dart::control::RobotControl::_dof;
```






### variable \_robot 

```C++
std::weak_ptr<Robot> robot_dart::control::RobotControl::_robot;
```






### variable \_weight 

```C++
double robot_dart::control::RobotControl::_weight;
```




------------------------------
The documentation for this class was generated from the following file `robot_dart/control/robot_control.hpp`

