

# Class robot\_dart::sensor::IMU



[**ClassList**](annotated.md) **>** [**robot\_dart**](namespacerobot__dart.md) **>** [**sensor**](namespacerobot__dart_1_1sensor.md) **>** [**IMU**](classrobot__dart_1_1sensor_1_1IMU.md)








Inherits the following classes: [robot\_dart::sensor::Sensor](classrobot__dart_1_1sensor_1_1Sensor.md)






















































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**IMU**](#function-imu) (const [**IMUConfig**](structrobot__dart_1_1sensor_1_1IMUConfig.md) & config) <br> |
|  const Eigen::AngleAxisd & | [**angular\_position**](#function-angular_position) () const<br> |
|  Eigen::Vector3d | [**angular\_position\_vec**](#function-angular_position_vec) () const<br> |
|  const Eigen::Vector3d & | [**angular\_velocity**](#function-angular_velocity) () const<br> |
| virtual void | [**attach\_to\_joint**](#function-attach_to_joint) (dart::dynamics::Joint \*, const Eigen::Isometry3d &) override<br> |
| virtual void | [**calculate**](#function-calculate) (double) override<br> |
| virtual void | [**init**](#function-init) () override<br> |
|  const Eigen::Vector3d & | [**linear\_acceleration**](#function-linear_acceleration) () const<br> |
| virtual std::string | [**type**](#function-type) () override const<br> |


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
|  Eigen::AngleAxisd | [**\_angular\_pos**](#variable-_angular_pos)  <br> |
|  Eigen::Vector3d | [**\_angular\_vel**](#variable-_angular_vel)  <br> |
|  [**IMUConfig**](structrobot__dart_1_1sensor_1_1IMUConfig.md) | [**\_config**](#variable-_config)  <br> |
|  Eigen::Vector3d | [**\_linear\_accel**](#variable-_linear_accel)  <br> |


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




### function IMU 

```C++
robot_dart::sensor::IMU::IMU (
    const IMUConfig & config
) 
```




<hr>



### function angular\_position 

```C++
const Eigen::AngleAxisd & robot_dart::sensor::IMU::angular_position () const
```




<hr>



### function angular\_position\_vec 

```C++
Eigen::Vector3d robot_dart::sensor::IMU::angular_position_vec () const
```




<hr>



### function angular\_velocity 

```C++
const Eigen::Vector3d & robot_dart::sensor::IMU::angular_velocity () const
```




<hr>



### function attach\_to\_joint 

```C++
inline virtual void robot_dart::sensor::IMU::attach_to_joint (
    dart::dynamics::Joint *,
    const Eigen::Isometry3d &
) override
```



Implements [*robot\_dart::sensor::Sensor::attach\_to\_joint*](classrobot__dart_1_1sensor_1_1Sensor.md#function-attach_to_joint-12)


<hr>



### function calculate 

```C++
virtual void robot_dart::sensor::IMU::calculate (
    double
) override
```



Implements [*robot\_dart::sensor::Sensor::calculate*](classrobot__dart_1_1sensor_1_1Sensor.md#function-calculate)


<hr>



### function init 

```C++
virtual void robot_dart::sensor::IMU::init () override
```



Implements [*robot\_dart::sensor::Sensor::init*](classrobot__dart_1_1sensor_1_1Sensor.md#function-init)


<hr>



### function linear\_acceleration 

```C++
const Eigen::Vector3d & robot_dart::sensor::IMU::linear_acceleration () const
```




<hr>



### function type 

```C++
virtual std::string robot_dart::sensor::IMU::type () override const
```



Implements [*robot\_dart::sensor::Sensor::type*](classrobot__dart_1_1sensor_1_1Sensor.md#function-type)


<hr>
## Protected Attributes Documentation




### variable \_angular\_pos 

```C++
Eigen::AngleAxisd robot_dart::sensor::IMU::_angular_pos;
```




<hr>



### variable \_angular\_vel 

```C++
Eigen::Vector3d robot_dart::sensor::IMU::_angular_vel;
```




<hr>



### variable \_config 

```C++
IMUConfig robot_dart::sensor::IMU::_config;
```




<hr>



### variable \_linear\_accel 

```C++
Eigen::Vector3d robot_dart::sensor::IMU::_linear_accel;
```




<hr>

------------------------------
The documentation for this class was generated from the following file `robot_dart/sensor/imu.hpp`

