

# Class robot\_dart::sensor::Torque



[**ClassList**](annotated.md) **>** [**robot\_dart**](namespacerobot__dart.md) **>** [**sensor**](namespacerobot__dart_1_1sensor.md) **>** [**Torque**](classrobot__dart_1_1sensor_1_1Torque.md)








Inherits the following classes: [robot\_dart::sensor::Sensor](classrobot__dart_1_1sensor_1_1Sensor.md)






















































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**Torque**](#function-torque-12) (dart::dynamics::Joint \* joint, size\_t frequency=1000) <br> |
|   | [**Torque**](#function-torque-22) (const std::shared\_ptr&lt; [**Robot**](classrobot__dart_1_1Robot.md) &gt; & robot, const std::string & joint\_name, size\_t frequency=1000) <br> |
| virtual void | [**attach\_to\_body**](#function-attach_to_body) (dart::dynamics::BodyNode \*, const Eigen::Isometry3d &) override<br> |
| virtual void | [**calculate**](#function-calculate) (double) override<br> |
| virtual void | [**init**](#function-init) () override<br> |
|  const Eigen::VectorXd & | [**torques**](#function-torques) () const<br> |
| virtual std::string | [**type**](#function-type) () override const<br> |


## Public Functions inherited from robot_dart::sensor::Sensor

See [robot\_dart::sensor::Sensor](classrobot__dart_1_1sensor_1_1Sensor.md)

| Type | Name |
| ---: | :--- |
|   | [**Sensor**](classrobot__dart_1_1sensor_1_1Sensor.md#function-sensor) (size\_t freq=40) <br> |
|  void | [**activate**](classrobot__dart_1_1sensor_1_1Sensor.md#function-activate) (bool enable=true) <br> |
|  bool | [**active**](classrobot__dart_1_1sensor_1_1Sensor.md#function-active) () const<br> |
| virtual void | [**attach\_to\_body**](classrobot__dart_1_1sensor_1_1Sensor.md#function-attach_to_body-12) (dart::dynamics::BodyNode \* body, const Eigen::Isometry3d & tf=Eigen::Isometry3d::Identity()) <br> |
|  void | [**attach\_to\_body**](classrobot__dart_1_1sensor_1_1Sensor.md#function-attach_to_body-22) (const std::shared\_ptr&lt; [**Robot**](classrobot__dart_1_1Robot.md) &gt; & robot, const std::string & body\_name, const Eigen::Isometry3d & tf=Eigen::Isometry3d::Identity()) <br> |
| virtual void | [**attach\_to\_joint**](classrobot__dart_1_1sensor_1_1Sensor.md#function-attach_to_joint-12) (dart::dynamics::Joint \* joint, const Eigen::Isometry3d & tf=Eigen::Isometry3d::Identity()) <br> |
|  void | [**attach\_to\_joint**](classrobot__dart_1_1sensor_1_1Sensor.md#function-attach_to_joint-22) (const std::shared\_ptr&lt; [**Robot**](classrobot__dart_1_1Robot.md) &gt; & robot, const std::string & joint\_name, const Eigen::Isometry3d & tf=Eigen::Isometry3d::Identity()) <br> |
|  const std::string & | [**attached\_to**](classrobot__dart_1_1sensor_1_1Sensor.md#function-attached_to) () const<br> |
| virtual void | [**calculate**](classrobot__dart_1_1sensor_1_1Sensor.md#function-calculate) (double) = 0<br> |
|  void | [**detach**](classrobot__dart_1_1sensor_1_1Sensor.md#function-detach) () <br> |
|  size\_t | [**frequency**](classrobot__dart_1_1sensor_1_1Sensor.md#function-frequency) () const<br> |
| virtual void | [**init**](classrobot__dart_1_1sensor_1_1Sensor.md#function-init) () = 0<br> |
|  const Eigen::Isometry3d & | [**pose**](classrobot__dart_1_1sensor_1_1Sensor.md#function-pose) () const<br> |
|  void | [**refresh**](classrobot__dart_1_1sensor_1_1Sensor.md#function-refresh) (double t) <br> |
|  void | [**set\_frequency**](classrobot__dart_1_1sensor_1_1Sensor.md#function-set_frequency) (size\_t freq) <br> |
|  void | [**set\_pose**](classrobot__dart_1_1sensor_1_1Sensor.md#function-set_pose) (const Eigen::Isometry3d & tf) <br> |
|  void | [**set\_simu**](classrobot__dart_1_1sensor_1_1Sensor.md#function-set_simu) ([**RobotDARTSimu**](classrobot__dart_1_1RobotDARTSimu.md) \* simu) <br> |
|  const [**RobotDARTSimu**](classrobot__dart_1_1RobotDARTSimu.md) \* | [**simu**](classrobot__dart_1_1sensor_1_1Sensor.md#function-simu) () const<br> |
| virtual std::string | [**type**](classrobot__dart_1_1sensor_1_1Sensor.md#function-type) () const = 0<br> |
| virtual  | [**~Sensor**](classrobot__dart_1_1sensor_1_1Sensor.md#function-sensor) () <br> |














## Protected Attributes

| Type | Name |
| ---: | :--- |
|  Eigen::VectorXd | [**\_torques**](#variable-_torques)  <br> |


## Protected Attributes inherited from robot_dart::sensor::Sensor

See [robot\_dart::sensor::Sensor](classrobot__dart_1_1sensor_1_1Sensor.md)

| Type | Name |
| ---: | :--- |
|  bool | [**\_active**](classrobot__dart_1_1sensor_1_1Sensor.md#variable-_active)  <br> |
|  Eigen::Isometry3d | [**\_attached\_tf**](classrobot__dart_1_1sensor_1_1Sensor.md#variable-_attached_tf)  <br> |
|  bool | [**\_attached\_to\_body**](classrobot__dart_1_1sensor_1_1Sensor.md#variable-_attached_to_body)   = = false<br> |
|  bool | [**\_attached\_to\_joint**](classrobot__dart_1_1sensor_1_1Sensor.md#variable-_attached_to_joint)   = = false<br> |
|  bool | [**\_attaching\_to\_body**](classrobot__dart_1_1sensor_1_1Sensor.md#variable-_attaching_to_body)   = = false<br> |
|  bool | [**\_attaching\_to\_joint**](classrobot__dart_1_1sensor_1_1Sensor.md#variable-_attaching_to_joint)   = = false<br> |
|  dart::dynamics::BodyNode \* | [**\_body\_attached**](classrobot__dart_1_1sensor_1_1Sensor.md#variable-_body_attached)  <br> |
|  size\_t | [**\_frequency**](classrobot__dart_1_1sensor_1_1Sensor.md#variable-_frequency)  <br> |
|  dart::dynamics::Joint \* | [**\_joint\_attached**](classrobot__dart_1_1sensor_1_1Sensor.md#variable-_joint_attached)  <br> |
|  [**RobotDARTSimu**](classrobot__dart_1_1RobotDARTSimu.md) \* | [**\_simu**](classrobot__dart_1_1sensor_1_1Sensor.md#variable-_simu)   = = nullptr<br> |
|  Eigen::Isometry3d | [**\_world\_pose**](classrobot__dart_1_1sensor_1_1Sensor.md#variable-_world_pose)  <br> |






































## Public Functions Documentation




### function Torque [1/2]

```C++
robot_dart::sensor::Torque::Torque (
    dart::dynamics::Joint * joint,
    size_t frequency=1000
) 
```




<hr>



### function Torque [2/2]

```C++
inline robot_dart::sensor::Torque::Torque (
    const std::shared_ptr< Robot > & robot,
    const std::string & joint_name,
    size_t frequency=1000
) 
```




<hr>



### function attach\_to\_body 

```C++
inline virtual void robot_dart::sensor::Torque::attach_to_body (
    dart::dynamics::BodyNode *,
    const Eigen::Isometry3d &
) override
```



Implements [*robot\_dart::sensor::Sensor::attach\_to\_body*](classrobot__dart_1_1sensor_1_1Sensor.md#function-attach_to_body-12)


<hr>



### function calculate 

```C++
virtual void robot_dart::sensor::Torque::calculate (
    double
) override
```



Implements [*robot\_dart::sensor::Sensor::calculate*](classrobot__dart_1_1sensor_1_1Sensor.md#function-calculate)


<hr>



### function init 

```C++
virtual void robot_dart::sensor::Torque::init () override
```



Implements [*robot\_dart::sensor::Sensor::init*](classrobot__dart_1_1sensor_1_1Sensor.md#function-init)


<hr>



### function torques 

```C++
const Eigen::VectorXd & robot_dart::sensor::Torque::torques () const
```




<hr>



### function type 

```C++
virtual std::string robot_dart::sensor::Torque::type () override const
```



Implements [*robot\_dart::sensor::Sensor::type*](classrobot__dart_1_1sensor_1_1Sensor.md#function-type)


<hr>
## Protected Attributes Documentation




### variable \_torques 

```C++
Eigen::VectorXd robot_dart::sensor::Torque::_torques;
```




<hr>

------------------------------
The documentation for this class was generated from the following file `robot_dart/sensor/torque.hpp`

