

# Class robot\_dart::control::SimpleControl



[**ClassList**](annotated.md) **>** [**robot\_dart**](namespacerobot__dart.md) **>** [**control**](namespacerobot__dart_1_1control.md) **>** [**SimpleControl**](classrobot__dart_1_1control_1_1SimpleControl.md)








Inherits the following classes: [robot\_dart::control::RobotControl](classrobot__dart_1_1control_1_1RobotControl.md)






















































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**SimpleControl**](#function-simplecontrol-13) () <br> |
|   | [**SimpleControl**](#function-simplecontrol-23) (const Eigen::VectorXd & ctrl, bool full\_control=false) <br> |
|   | [**SimpleControl**](#function-simplecontrol-33) (const Eigen::VectorXd & ctrl, const std::vector&lt; std::string &gt; & controllable\_dofs) <br> |
| virtual Eigen::VectorXd | [**calculate**](#function-calculate) (double) override<br> |
| virtual std::shared\_ptr&lt; [**RobotControl**](classrobot__dart_1_1control_1_1RobotControl.md) &gt; | [**clone**](#function-clone) () override const<br> |
| virtual void | [**configure**](#function-configure) () override<br> |


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






































## Public Functions Documentation




### function SimpleControl [1/3]

```C++
robot_dart::control::SimpleControl::SimpleControl () 
```






### function SimpleControl [2/3]

```C++
robot_dart::control::SimpleControl::SimpleControl (
    const Eigen::VectorXd & ctrl,
    bool full_control=false
) 
```






### function SimpleControl [3/3]

```C++
robot_dart::control::SimpleControl::SimpleControl (
    const Eigen::VectorXd & ctrl,
    const std::vector< std::string > & controllable_dofs
) 
```






### function calculate 

```C++
virtual Eigen::VectorXd robot_dart::control::SimpleControl::calculate (
    double
) override
```



Implements [*robot\_dart::control::RobotControl::calculate*](classrobot__dart_1_1control_1_1RobotControl.md#function-calculate)




### function clone 

```C++
virtual std::shared_ptr< RobotControl > robot_dart::control::SimpleControl::clone () override const
```



Implements [*robot\_dart::control::RobotControl::clone*](classrobot__dart_1_1control_1_1RobotControl.md#function-clone)




### function configure 

```C++
virtual void robot_dart::control::SimpleControl::configure () override
```



Implements [*robot\_dart::control::RobotControl::configure*](classrobot__dart_1_1control_1_1RobotControl.md#function-configure)


------------------------------
The documentation for this class was generated from the following file `robot_dart/control/simple_control.hpp`

