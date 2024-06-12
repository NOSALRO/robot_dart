

# Class robot\_dart::control::PolicyControl

**template &lt;typename Policy&gt;**



[**ClassList**](annotated.md) **>** [**robot\_dart**](namespacerobot__dart.md) **>** [**control**](namespacerobot__dart_1_1control.md) **>** [**PolicyControl**](classrobot__dart_1_1control_1_1PolicyControl.md)








Inherits the following classes: [robot\_dart::control::RobotControl](classrobot__dart_1_1control_1_1RobotControl.md)






















































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**PolicyControl**](#function-policycontrol-15) () <br> |
|   | [**PolicyControl**](#function-policycontrol-25) (double dt, const Eigen::VectorXd & ctrl, bool full\_control=false) <br> |
|   | [**PolicyControl**](#function-policycontrol-35) (const Eigen::VectorXd & ctrl, bool full\_control=false) <br> |
|   | [**PolicyControl**](#function-policycontrol-45) (double dt, const Eigen::VectorXd & ctrl, const std::vector&lt; std::string &gt; & controllable\_dofs) <br> |
|   | [**PolicyControl**](#function-policycontrol-55) (const Eigen::VectorXd & ctrl, const std::vector&lt; std::string &gt; & controllable\_dofs) <br> |
| virtual Eigen::VectorXd | [**calculate**](#function-calculate) (double t) override<br> |
| virtual std::shared\_ptr&lt; [**RobotControl**](classrobot__dart_1_1control_1_1RobotControl.md) &gt; | [**clone**](#function-clone) () override const<br> |
| virtual void | [**configure**](#function-configure) () override<br> |
|  Eigen::VectorXd | [**h\_params**](#function-h_params) () const<br> |
|  void | [**set\_h\_params**](#function-set_h_params) (const Eigen::VectorXd & h\_params) <br> |


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














## Protected Attributes

| Type | Name |
| ---: | :--- |
|  double | [**\_dt**](#variable-_dt)  <br> |
|  bool | [**\_first**](#variable-_first)  <br> |
|  bool | [**\_full\_dt**](#variable-_full_dt)  <br> |
|  int | [**\_i**](#variable-_i)  <br> |
|  Policy | [**\_policy**](#variable-_policy)  <br> |
|  Eigen::VectorXd | [**\_prev\_commands**](#variable-_prev_commands)  <br> |
|  double | [**\_prev\_time**](#variable-_prev_time)  <br> |
|  double | [**\_threshold**](#variable-_threshold)  <br> |


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




### function PolicyControl [1/5]

```C++
inline robot_dart::control::PolicyControl::PolicyControl () 
```




<hr>



### function PolicyControl [2/5]

```C++
inline robot_dart::control::PolicyControl::PolicyControl (
    double dt,
    const Eigen::VectorXd & ctrl,
    bool full_control=false
) 
```




<hr>



### function PolicyControl [3/5]

```C++
inline robot_dart::control::PolicyControl::PolicyControl (
    const Eigen::VectorXd & ctrl,
    bool full_control=false
) 
```




<hr>



### function PolicyControl [4/5]

```C++
inline robot_dart::control::PolicyControl::PolicyControl (
    double dt,
    const Eigen::VectorXd & ctrl,
    const std::vector< std::string > & controllable_dofs
) 
```




<hr>



### function PolicyControl [5/5]

```C++
inline robot_dart::control::PolicyControl::PolicyControl (
    const Eigen::VectorXd & ctrl,
    const std::vector< std::string > & controllable_dofs
) 
```




<hr>



### function calculate 

```C++
inline virtual Eigen::VectorXd robot_dart::control::PolicyControl::calculate (
    double t
) override
```



Implements [*robot\_dart::control::RobotControl::calculate*](classrobot__dart_1_1control_1_1RobotControl.md#function-calculate)


<hr>



### function clone 

```C++
inline virtual std::shared_ptr< RobotControl > robot_dart::control::PolicyControl::clone () override const
```



Implements [*robot\_dart::control::RobotControl::clone*](classrobot__dart_1_1control_1_1RobotControl.md#function-clone)


<hr>



### function configure 

```C++
inline virtual void robot_dart::control::PolicyControl::configure () override
```



Implements [*robot\_dart::control::RobotControl::configure*](classrobot__dart_1_1control_1_1RobotControl.md#function-configure)


<hr>



### function h\_params 

```C++
inline Eigen::VectorXd robot_dart::control::PolicyControl::h_params () const
```




<hr>



### function set\_h\_params 

```C++
inline void robot_dart::control::PolicyControl::set_h_params (
    const Eigen::VectorXd & h_params
) 
```




<hr>
## Protected Attributes Documentation




### variable \_dt 

```C++
double robot_dart::control::PolicyControl< Policy >::_dt;
```




<hr>



### variable \_first 

```C++
bool robot_dart::control::PolicyControl< Policy >::_first;
```




<hr>



### variable \_full\_dt 

```C++
bool robot_dart::control::PolicyControl< Policy >::_full_dt;
```




<hr>



### variable \_i 

```C++
int robot_dart::control::PolicyControl< Policy >::_i;
```




<hr>



### variable \_policy 

```C++
Policy robot_dart::control::PolicyControl< Policy >::_policy;
```




<hr>



### variable \_prev\_commands 

```C++
Eigen::VectorXd robot_dart::control::PolicyControl< Policy >::_prev_commands;
```




<hr>



### variable \_prev\_time 

```C++
double robot_dart::control::PolicyControl< Policy >::_prev_time;
```




<hr>



### variable \_threshold 

```C++
double robot_dart::control::PolicyControl< Policy >::_threshold;
```




<hr>

------------------------------
The documentation for this class was generated from the following file `robot_dart/control/policy_control.hpp`

