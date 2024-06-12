

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
|   | [**RobotControl**](classrobot__dart_1_1control_1_1RobotControl.md#function-robotcontrol-13) () <br> |
|   | [**RobotControl**](classrobot__dart_1_1control_1_1RobotControl.md#function-robotcontrol-23) (const Eigen::VectorXd & ctrl, bool full\_control=false) <br> |
|   | [**RobotControl**](classrobot__dart_1_1control_1_1RobotControl.md#function-robotcontrol-33) (const Eigen::VectorXd & ctrl, const std::vector&lt; std::string &gt; & controllable\_dofs) <br> |
|  void | [**activate**](classrobot__dart_1_1control_1_1RobotControl.md#function-activate) (bool enable=true) <br> |
|  bool | [**active**](classrobot__dart_1_1control_1_1RobotControl.md#function-active) () const<br> |
| virtual Eigen::VectorXd | [**calculate**](classrobot__dart_1_1control_1_1RobotControl.md#function-calculate) (double t) = 0<br> |
| virtual std::shared\_ptr&lt; [**RobotControl**](classrobot__dart_1_1control_1_1RobotControl.md) &gt; | [**clone**](classrobot__dart_1_1control_1_1RobotControl.md#function-clone) () const = 0<br> |
| virtual void | [**configure**](classrobot__dart_1_1control_1_1RobotControl.md#function-configure) () = 0<br> |
|  const std::vector&lt; std::string &gt; & | [**controllable\_dofs**](classrobot__dart_1_1control_1_1RobotControl.md#function-controllable_dofs) () const<br> |
|  void | [**init**](classrobot__dart_1_1control_1_1RobotControl.md#function-init) () <br> |
|  const Eigen::VectorXd & | [**parameters**](classrobot__dart_1_1control_1_1RobotControl.md#function-parameters) () const<br> |
|  std::shared\_ptr&lt; [**Robot**](classrobot__dart_1_1Robot.md) &gt; | [**robot**](classrobot__dart_1_1control_1_1RobotControl.md#function-robot) () const<br> |
|  void | [**set\_parameters**](classrobot__dart_1_1control_1_1RobotControl.md#function-set_parameters) (const Eigen::VectorXd & ctrl) <br> |
|  void | [**set\_robot**](classrobot__dart_1_1control_1_1RobotControl.md#function-set_robot) (const std::shared\_ptr&lt; [**Robot**](classrobot__dart_1_1Robot.md) &gt; & robot) <br> |
|  void | [**set\_weight**](classrobot__dart_1_1control_1_1RobotControl.md#function-set_weight) (double weight) <br> |
|  double | [**weight**](classrobot__dart_1_1control_1_1RobotControl.md#function-weight) () const<br> |
| virtual  | [**~RobotControl**](classrobot__dart_1_1control_1_1RobotControl.md#function-robotcontrol) () <br> |
















## Protected Attributes inherited from robot_dart::control::RobotControl

See [robot\_dart::control::RobotControl](classrobot__dart_1_1control_1_1RobotControl.md)

| Type | Name |
| ---: | :--- |
|  bool | [**\_active**](classrobot__dart_1_1control_1_1RobotControl.md#variable-_active)  <br> |
|  bool | [**\_check\_free**](classrobot__dart_1_1control_1_1RobotControl.md#variable-_check_free)   = = false<br> |
|  int | [**\_control\_dof**](classrobot__dart_1_1control_1_1RobotControl.md#variable-_control_dof)  <br> |
|  std::vector&lt; std::string &gt; | [**\_controllable\_dofs**](classrobot__dart_1_1control_1_1RobotControl.md#variable-_controllable_dofs)  <br> |
|  Eigen::VectorXd | [**\_ctrl**](classrobot__dart_1_1control_1_1RobotControl.md#variable-_ctrl)  <br> |
|  int | [**\_dof**](classrobot__dart_1_1control_1_1RobotControl.md#variable-_dof)  <br> |
|  std::weak\_ptr&lt; [**Robot**](classrobot__dart_1_1Robot.md) &gt; | [**\_robot**](classrobot__dart_1_1control_1_1RobotControl.md#variable-_robot)  <br> |
|  double | [**\_weight**](classrobot__dart_1_1control_1_1RobotControl.md#variable-_weight)  <br> |






































## Public Functions Documentation




### function SimpleControl [1/3]

```C++
robot_dart::control::SimpleControl::SimpleControl () 
```




<hr>



### function SimpleControl [2/3]

```C++
robot_dart::control::SimpleControl::SimpleControl (
    const Eigen::VectorXd & ctrl,
    bool full_control=false
) 
```




<hr>



### function SimpleControl [3/3]

```C++
robot_dart::control::SimpleControl::SimpleControl (
    const Eigen::VectorXd & ctrl,
    const std::vector< std::string > & controllable_dofs
) 
```




<hr>



### function calculate 

```C++
virtual Eigen::VectorXd robot_dart::control::SimpleControl::calculate (
    double
) override
```



Implements [*robot\_dart::control::RobotControl::calculate*](classrobot__dart_1_1control_1_1RobotControl.md#function-calculate)


<hr>



### function clone 

```C++
virtual std::shared_ptr< RobotControl > robot_dart::control::SimpleControl::clone () override const
```



Implements [*robot\_dart::control::RobotControl::clone*](classrobot__dart_1_1control_1_1RobotControl.md#function-clone)


<hr>



### function configure 

```C++
virtual void robot_dart::control::SimpleControl::configure () override
```



Implements [*robot\_dart::control::RobotControl::configure*](classrobot__dart_1_1control_1_1RobotControl.md#function-configure)


<hr>

------------------------------
The documentation for this class was generated from the following file `robot_dart/control/simple_control.hpp`

