

# Class robot\_dart::control::PDControl



[**ClassList**](annotated.md) **>** [**robot\_dart**](namespacerobot__dart.md) **>** [**control**](namespacerobot__dart_1_1control.md) **>** [**PDControl**](classrobot__dart_1_1control_1_1PDControl.md)








Inherits the following classes: [robot\_dart::control::RobotControl](classrobot__dart_1_1control_1_1RobotControl.md)






















































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**PDControl**](#function-pdcontrol-13) () <br> |
|   | [**PDControl**](#function-pdcontrol-23) (const Eigen::VectorXd & ctrl, bool full\_control=false, bool use\_angular\_errors=true) <br> |
|   | [**PDControl**](#function-pdcontrol-33) (const Eigen::VectorXd & ctrl, const std::vector&lt; std::string &gt; & controllable\_dofs, bool use\_angular\_errors=true) <br> |
| virtual Eigen::VectorXd | [**calculate**](#function-calculate) (double) override<br> |
| virtual std::shared\_ptr&lt; [**RobotControl**](classrobot__dart_1_1control_1_1RobotControl.md) &gt; | [**clone**](#function-clone) () override const<br> |
| virtual void | [**configure**](#function-configure) () override<br> |
|  std::pair&lt; Eigen::VectorXd, Eigen::VectorXd &gt; | [**pd**](#function-pd) () const<br> |
|  void | [**set\_pd**](#function-set_pd-12) (double p, double d) <br> |
|  void | [**set\_pd**](#function-set_pd-22) (const Eigen::VectorXd & p, const Eigen::VectorXd & d) <br> |
|  void | [**set\_use\_angular\_errors**](#function-set_use_angular_errors) (bool enable=true) <br> |
|  bool | [**using\_angular\_errors**](#function-using_angular_errors) () const<br> |


## Public Functions inherited from robot_dart::control::RobotControl

See [robot\_dart::control::RobotControl](classrobot__dart_1_1control_1_1RobotControl.md)

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
|  Eigen::VectorXd | [**\_Kd**](#variable-_kd)  <br> |
|  Eigen::VectorXd | [**\_Kp**](#variable-_kp)  <br> |
|  bool | [**\_use\_angular\_errors**](#variable-_use_angular_errors)  <br> |


## Protected Attributes inherited from robot_dart::control::RobotControl

See [robot\_dart::control::RobotControl](classrobot__dart_1_1control_1_1RobotControl.md)

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


































## Protected Static Functions

| Type | Name |
| ---: | :--- |
|  double | [**\_angle\_dist**](#function-_angle_dist) (double target, double current) <br> |




## Public Functions Documentation




### function PDControl [1/3]

```C++
robot_dart::control::PDControl::PDControl () 
```






### function PDControl [2/3]

```C++
robot_dart::control::PDControl::PDControl (
    const Eigen::VectorXd & ctrl,
    bool full_control=false,
    bool use_angular_errors=true
) 
```






### function PDControl [3/3]

```C++
robot_dart::control::PDControl::PDControl (
    const Eigen::VectorXd & ctrl,
    const std::vector< std::string > & controllable_dofs,
    bool use_angular_errors=true
) 
```






### function calculate 

```C++
virtual Eigen::VectorXd robot_dart::control::PDControl::calculate (
    double
) override
```



Implements [*robot\_dart::control::RobotControl::calculate*](classrobot__dart_1_1control_1_1RobotControl.md#function-calculate)




### function clone 

```C++
virtual std::shared_ptr< RobotControl > robot_dart::control::PDControl::clone () override const
```



Implements [*robot\_dart::control::RobotControl::clone*](classrobot__dart_1_1control_1_1RobotControl.md#function-clone)




### function configure 

```C++
virtual void robot_dart::control::PDControl::configure () override
```



Implements [*robot\_dart::control::RobotControl::configure*](classrobot__dart_1_1control_1_1RobotControl.md#function-configure)




### function pd 

```C++
std::pair< Eigen::VectorXd, Eigen::VectorXd > robot_dart::control::PDControl::pd () const
```






### function set\_pd [1/2]

```C++
void robot_dart::control::PDControl::set_pd (
    double p,
    double d
) 
```






### function set\_pd [2/2]

```C++
void robot_dart::control::PDControl::set_pd (
    const Eigen::VectorXd & p,
    const Eigen::VectorXd & d
) 
```






### function set\_use\_angular\_errors 

```C++
void robot_dart::control::PDControl::set_use_angular_errors (
    bool enable=true
) 
```






### function using\_angular\_errors 

```C++
bool robot_dart::control::PDControl::using_angular_errors () const
```



## Protected Attributes Documentation




### variable \_Kd 

```C++
Eigen::VectorXd robot_dart::control::PDControl::_Kd;
```






### variable \_Kp 

```C++
Eigen::VectorXd robot_dart::control::PDControl::_Kp;
```






### variable \_use\_angular\_errors 

```C++
bool robot_dart::control::PDControl::_use_angular_errors;
```



## Protected Static Functions Documentation




### function \_angle\_dist 

```C++
static double robot_dart::control::PDControl::_angle_dist (
    double target,
    double current
) 
```




------------------------------
The documentation for this class was generated from the following file `robot_dart/control/pd_control.hpp`

