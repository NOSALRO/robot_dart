

# Class robot\_dart::sensor::Sensor



[**ClassList**](annotated.md) **>** [**robot\_dart**](namespacerobot__dart.md) **>** [**sensor**](namespacerobot__dart_1_1sensor.md) **>** [**Sensor**](classrobot__dart_1_1sensor_1_1Sensor.md)










Inherited by the following classes: [robot\_dart::gui::magnum::sensor::Camera](classrobot__dart_1_1gui_1_1magnum_1_1sensor_1_1Camera.md),  [robot\_dart::sensor::ForceTorque](classrobot__dart_1_1sensor_1_1ForceTorque.md),  [robot\_dart::sensor::IMU](classrobot__dart_1_1sensor_1_1IMU.md),  [robot\_dart::sensor::Torque](classrobot__dart_1_1sensor_1_1Torque.md)
































## Public Functions

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




### function Sensor 

```C++
robot_dart::sensor::Sensor::Sensor (
    size_t freq=40
) 
```




<hr>



### function activate 

```C++
void robot_dart::sensor::Sensor::activate (
    bool enable=true
) 
```




<hr>



### function active 

```C++
bool robot_dart::sensor::Sensor::active () const
```




<hr>



### function attach\_to\_body [1/2]

```C++
virtual void robot_dart::sensor::Sensor::attach_to_body (
    dart::dynamics::BodyNode * body,
    const Eigen::Isometry3d & tf=Eigen::Isometry3d::Identity()
) 
```




<hr>



### function attach\_to\_body [2/2]

```C++
inline void robot_dart::sensor::Sensor::attach_to_body (
    const std::shared_ptr< Robot > & robot,
    const std::string & body_name,
    const Eigen::Isometry3d & tf=Eigen::Isometry3d::Identity()
) 
```




<hr>



### function attach\_to\_joint [1/2]

```C++
virtual void robot_dart::sensor::Sensor::attach_to_joint (
    dart::dynamics::Joint * joint,
    const Eigen::Isometry3d & tf=Eigen::Isometry3d::Identity()
) 
```




<hr>



### function attach\_to\_joint [2/2]

```C++
inline void robot_dart::sensor::Sensor::attach_to_joint (
    const std::shared_ptr< Robot > & robot,
    const std::string & joint_name,
    const Eigen::Isometry3d & tf=Eigen::Isometry3d::Identity()
) 
```




<hr>



### function attached\_to 

```C++
const std::string & robot_dart::sensor::Sensor::attached_to () const
```




<hr>



### function calculate 

```C++
virtual void robot_dart::sensor::Sensor::calculate (
    double
) = 0
```




<hr>



### function detach 

```C++
void robot_dart::sensor::Sensor::detach () 
```




<hr>



### function frequency 

```C++
size_t robot_dart::sensor::Sensor::frequency () const
```




<hr>



### function init 

```C++
virtual void robot_dart::sensor::Sensor::init () = 0
```




<hr>



### function pose 

```C++
const Eigen::Isometry3d & robot_dart::sensor::Sensor::pose () const
```




<hr>



### function refresh 

```C++
void robot_dart::sensor::Sensor::refresh (
    double t
) 
```




<hr>



### function set\_frequency 

```C++
void robot_dart::sensor::Sensor::set_frequency (
    size_t freq
) 
```




<hr>



### function set\_pose 

```C++
void robot_dart::sensor::Sensor::set_pose (
    const Eigen::Isometry3d & tf
) 
```




<hr>



### function set\_simu 

```C++
void robot_dart::sensor::Sensor::set_simu (
    RobotDARTSimu * simu
) 
```




<hr>



### function simu 

```C++
const RobotDARTSimu * robot_dart::sensor::Sensor::simu () const
```




<hr>



### function type 

```C++
virtual std::string robot_dart::sensor::Sensor::type () const = 0
```




<hr>



### function ~Sensor 

```C++
inline virtual robot_dart::sensor::Sensor::~Sensor () 
```




<hr>
## Protected Attributes Documentation




### variable \_active 

```C++
bool robot_dart::sensor::Sensor::_active;
```




<hr>



### variable \_attached\_tf 

```C++
Eigen::Isometry3d robot_dart::sensor::Sensor::_attached_tf;
```




<hr>



### variable \_attached\_to\_body 

```C++
bool robot_dart::sensor::Sensor::_attached_to_body;
```




<hr>



### variable \_attached\_to\_joint 

```C++
bool robot_dart::sensor::Sensor::_attached_to_joint;
```




<hr>



### variable \_attaching\_to\_body 

```C++
bool robot_dart::sensor::Sensor::_attaching_to_body;
```




<hr>



### variable \_attaching\_to\_joint 

```C++
bool robot_dart::sensor::Sensor::_attaching_to_joint;
```




<hr>



### variable \_body\_attached 

```C++
dart::dynamics::BodyNode* robot_dart::sensor::Sensor::_body_attached;
```




<hr>



### variable \_frequency 

```C++
size_t robot_dart::sensor::Sensor::_frequency;
```




<hr>



### variable \_joint\_attached 

```C++
dart::dynamics::Joint* robot_dart::sensor::Sensor::_joint_attached;
```




<hr>



### variable \_simu 

```C++
RobotDARTSimu* robot_dart::sensor::Sensor::_simu;
```




<hr>



### variable \_world\_pose 

```C++
Eigen::Isometry3d robot_dart::sensor::Sensor::_world_pose;
```




<hr>

------------------------------
The documentation for this class was generated from the following file `robot_dart/sensor/sensor.hpp`

