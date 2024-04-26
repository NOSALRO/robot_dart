

# Class robot\_dart::sensor::ForceTorque



[**ClassList**](annotated.md) **>** [**robot\_dart**](namespacerobot__dart.md) **>** [**sensor**](namespacerobot__dart_1_1sensor.md) **>** [**ForceTorque**](classrobot__dart_1_1sensor_1_1ForceTorque.md)








Inherits the following classes: [robot\_dart::sensor::Sensor](classrobot__dart_1_1sensor_1_1Sensor.md)






















































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**ForceTorque**](#function-forcetorque-12) (dart::dynamics::Joint \* joint, size\_t frequency=1000, const std::string & direction="child\_to\_parent") <br> |
|   | [**ForceTorque**](#function-forcetorque-22) (const std::shared\_ptr&lt; [**Robot**](classrobot__dart_1_1Robot.md) &gt; & robot, const std::string & joint\_name, size\_t frequency=1000, const std::string & direction="child\_to\_parent") <br> |
| virtual void | [**attach\_to\_body**](#function-attach_to_body) (dart::dynamics::BodyNode \*, const Eigen::Isometry3d &) override<br> |
| virtual void | [**calculate**](#function-calculate) (double) override<br> |
|  Eigen::Vector3d | [**force**](#function-force) () const<br> |
| virtual void | [**init**](#function-init) () override<br> |
|  Eigen::Vector3d | [**torque**](#function-torque) () const<br> |
| virtual std::string | [**type**](#function-type) () override const<br> |
|  const Eigen::Vector6d & | [**wrench**](#function-wrench) () const<br> |


## Public Functions inherited from robot_dart::sensor::Sensor

See [robot\_dart::sensor::Sensor](classrobot__dart_1_1sensor_1_1Sensor.md)

| Type | Name |
| ---: | :--- |
|   | [**Sensor**](#function-sensor) (size\_t freq=40) <br> |
|  void | [**activate**](#function-activate) (bool enable=true) <br> |
|  bool | [**active**](#function-active) () const<br> |
| virtual void | [**attach\_to\_body**](#function-attach_to_body-12) (dart::dynamics::BodyNode \* body, const Eigen::Isometry3d & tf=Eigen::Isometry3d::Identity()) <br> |
|  void | [**attach\_to\_body**](#function-attach_to_body-22) (const std::shared\_ptr&lt; [**Robot**](classrobot__dart_1_1Robot.md) &gt; & robot, const std::string & body\_name, const Eigen::Isometry3d & tf=Eigen::Isometry3d::Identity()) <br> |
| virtual void | [**attach\_to\_joint**](#function-attach_to_joint-12) (dart::dynamics::Joint \* joint, const Eigen::Isometry3d & tf=Eigen::Isometry3d::Identity()) <br> |
|  void | [**attach\_to\_joint**](#function-attach_to_joint-22) (const std::shared\_ptr&lt; [**Robot**](classrobot__dart_1_1Robot.md) &gt; & robot, const std::string & joint\_name, const Eigen::Isometry3d & tf=Eigen::Isometry3d::Identity()) <br> |
|  const std::string & | [**attached\_to**](#function-attached_to) () const<br> |
| virtual void | [**calculate**](#function-calculate) (double) = 0<br> |
|  void | [**detach**](#function-detach) () <br> |
|  size\_t | [**frequency**](#function-frequency) () const<br> |
| virtual void | [**init**](#function-init) () = 0<br> |
|  const Eigen::Isometry3d & | [**pose**](#function-pose) () const<br> |
|  void | [**refresh**](#function-refresh) (double t) <br> |
|  void | [**set\_frequency**](#function-set_frequency) (size\_t freq) <br> |
|  void | [**set\_pose**](#function-set_pose) (const Eigen::Isometry3d & tf) <br> |
|  void | [**set\_simu**](#function-set_simu) ([**RobotDARTSimu**](classrobot__dart_1_1RobotDARTSimu.md) \* simu) <br> |
|  const [**RobotDARTSimu**](classrobot__dart_1_1RobotDARTSimu.md) \* | [**simu**](#function-simu) () const<br> |
| virtual std::string | [**type**](#function-type) () const = 0<br> |
| virtual  | [**~Sensor**](#function-sensor) () <br> |














## Protected Attributes

| Type | Name |
| ---: | :--- |
|  std::string | [**\_direction**](#variable-_direction)  <br> |
|  Eigen::Vector6d | [**\_wrench**](#variable-_wrench)  <br> |


## Protected Attributes inherited from robot_dart::sensor::Sensor

See [robot\_dart::sensor::Sensor](classrobot__dart_1_1sensor_1_1Sensor.md)

| Type | Name |
| ---: | :--- |
|  bool | [**\_active**](#variable-_active)  <br> |
|  Eigen::Isometry3d | [**\_attached\_tf**](#variable-_attached_tf)  <br> |
|  bool | [**\_attached\_to\_body**](#variable-_attached_to_body)   = = false<br> |
|  bool | [**\_attached\_to\_joint**](#variable-_attached_to_joint)   = = false<br> |
|  bool | [**\_attaching\_to\_body**](#variable-_attaching_to_body)   = = false<br> |
|  bool | [**\_attaching\_to\_joint**](#variable-_attaching_to_joint)   = = false<br> |
|  dart::dynamics::BodyNode \* | [**\_body\_attached**](#variable-_body_attached)  <br> |
|  size\_t | [**\_frequency**](#variable-_frequency)  <br> |
|  dart::dynamics::Joint \* | [**\_joint\_attached**](#variable-_joint_attached)  <br> |
|  [**RobotDARTSimu**](classrobot__dart_1_1RobotDARTSimu.md) \* | [**\_simu**](#variable-_simu)   = = nullptr<br> |
|  Eigen::Isometry3d | [**\_world\_pose**](#variable-_world_pose)  <br> |






































## Public Functions Documentation




### function ForceTorque [1/2]

```C++
robot_dart::sensor::ForceTorque::ForceTorque (
    dart::dynamics::Joint * joint,
    size_t frequency=1000,
    const std::string & direction="child_to_parent"
) 
```




<hr>



### function ForceTorque [2/2]

```C++
inline robot_dart::sensor::ForceTorque::ForceTorque (
    const std::shared_ptr< Robot > & robot,
    const std::string & joint_name,
    size_t frequency=1000,
    const std::string & direction="child_to_parent"
) 
```




<hr>



### function attach\_to\_body 

```C++
inline virtual void robot_dart::sensor::ForceTorque::attach_to_body (
    dart::dynamics::BodyNode *,
    const Eigen::Isometry3d &
) override
```



Implements [*robot\_dart::sensor::Sensor::attach\_to\_body*](classrobot__dart_1_1sensor_1_1Sensor.md#function-attach_to_body-12)


<hr>



### function calculate 

```C++
virtual void robot_dart::sensor::ForceTorque::calculate (
    double
) override
```



Implements [*robot\_dart::sensor::Sensor::calculate*](classrobot__dart_1_1sensor_1_1Sensor.md#function-calculate)


<hr>



### function force 

```C++
Eigen::Vector3d robot_dart::sensor::ForceTorque::force () const
```




<hr>



### function init 

```C++
virtual void robot_dart::sensor::ForceTorque::init () override
```



Implements [*robot\_dart::sensor::Sensor::init*](classrobot__dart_1_1sensor_1_1Sensor.md#function-init)


<hr>



### function torque 

```C++
Eigen::Vector3d robot_dart::sensor::ForceTorque::torque () const
```




<hr>



### function type 

```C++
virtual std::string robot_dart::sensor::ForceTorque::type () override const
```



Implements [*robot\_dart::sensor::Sensor::type*](classrobot__dart_1_1sensor_1_1Sensor.md#function-type)


<hr>



### function wrench 

```C++
const Eigen::Vector6d & robot_dart::sensor::ForceTorque::wrench () const
```




<hr>
## Protected Attributes Documentation




### variable \_direction 

```C++
std::string robot_dart::sensor::ForceTorque::_direction;
```




<hr>



### variable \_wrench 

```C++
Eigen::Vector6d robot_dart::sensor::ForceTorque::_wrench;
```




<hr>

------------------------------
The documentation for this class was generated from the following file `robot_dart/sensor/force_torque.hpp`

