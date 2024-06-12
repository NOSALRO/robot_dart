

# Class robot\_dart::gui::magnum::sensor::Camera



[**ClassList**](annotated.md) **>** [**robot\_dart**](namespacerobot__dart.md) **>** [**gui**](namespacerobot__dart_1_1gui.md) **>** [**magnum**](namespacerobot__dart_1_1gui_1_1magnum.md) **>** [**sensor**](namespacerobot__dart_1_1gui_1_1magnum_1_1sensor.md) **>** [**Camera**](classrobot__dart_1_1gui_1_1magnum_1_1sensor_1_1Camera.md)








Inherits the following classes: [robot\_dart::sensor::Sensor](classrobot__dart_1_1sensor_1_1Sensor.md)






















































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**Camera**](#function-camera) ([**BaseApplication**](classrobot__dart_1_1gui_1_1magnum_1_1BaseApplication.md) \* app, size\_t width, size\_t height, size\_t freq=30, bool draw\_debug=false) <br> |
| virtual void | [**attach\_to\_body**](#function-attach_to_body) (dart::dynamics::BodyNode \* body, const Eigen::Isometry3d & tf=Eigen::Isometry3d::Identity()) override<br> |
| virtual void | [**attach\_to\_joint**](#function-attach_to_joint) (dart::dynamics::Joint \*, const Eigen::Isometry3d &) override<br> |
| virtual void | [**calculate**](#function-calculate) (double) override<br> |
|  [**gs::Camera**](classrobot__dart_1_1gui_1_1magnum_1_1gs_1_1Camera.md) & | [**camera**](#function-camera-12) () <br> |
|  const [**gs::Camera**](classrobot__dart_1_1gui_1_1magnum_1_1gs_1_1Camera.md) & | [**camera**](#function-camera-22) () const<br> |
|  Eigen::Matrix4d | [**camera\_extrinsic\_matrix**](#function-camera_extrinsic_matrix) () const<br> |
|  Eigen::Matrix3d | [**camera\_intrinsic\_matrix**](#function-camera_intrinsic_matrix) () const<br> |
|  [**DepthImage**](structrobot__dart_1_1gui_1_1DepthImage.md) | [**depth\_array**](#function-depth_array) () <br> |
|  [**GrayscaleImage**](structrobot__dart_1_1gui_1_1GrayscaleImage.md) | [**depth\_image**](#function-depth_image) () <br> |
|  void | [**draw\_debug**](#function-draw_debug) (bool draw=true) <br> |
|  bool | [**drawing\_debug**](#function-drawing_debug) () const<br> |
|  [**Image**](structrobot__dart_1_1gui_1_1Image.md) | [**image**](#function-image) () <br> |
| virtual void | [**init**](#function-init) () override<br> |
|  void | [**look\_at**](#function-look_at) (const Eigen::Vector3d & camera\_pos, const Eigen::Vector3d & look\_at=Eigen::Vector3d(0, 0, 0), const Eigen::Vector3d & up=Eigen::Vector3d(0, 0, 1)) <br> |
|  Magnum::Image2D \* | [**magnum\_depth\_image**](#function-magnum_depth_image) () <br> |
|  Magnum::Image2D \* | [**magnum\_image**](#function-magnum_image) () <br> |
|  [**GrayscaleImage**](structrobot__dart_1_1gui_1_1GrayscaleImage.md) | [**raw\_depth\_image**](#function-raw_depth_image) () <br> |
|  void | [**record\_video**](#function-record_video) (const std::string & video\_fname) <br> |
| virtual std::string | [**type**](#function-type) () override const<br> |
|   | [**~Camera**](#function-camera) () <br> |


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
|  std::unique\_ptr&lt; [**gs::Camera**](classrobot__dart_1_1gui_1_1magnum_1_1gs_1_1Camera.md) &gt; | [**\_camera**](#variable-_camera)  <br> |
|  Magnum::GL::Renderbuffer | [**\_color**](#variable-_color)  <br> |
|  Magnum::GL::Renderbuffer | [**\_depth**](#variable-_depth)  <br> |
|  bool | [**\_draw\_debug**](#variable-_draw_debug)  <br> |
|  Magnum::PixelFormat | [**\_format**](#variable-_format)  <br> |
|  Magnum::GL::Framebuffer | [**\_framebuffer**](#variable-_framebuffer)   = {Magnum::NoCreate}<br> |
|  size\_t | [**\_height**](#variable-_height)  <br> |
|  [**BaseApplication**](classrobot__dart_1_1gui_1_1magnum_1_1BaseApplication.md) \* | [**\_magnum\_app**](#variable-_magnum_app)  <br> |
|  size\_t | [**\_width**](#variable-_width)  <br> |


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




### function Camera 

```C++
robot_dart::gui::magnum::sensor::Camera::Camera (
    BaseApplication * app,
    size_t width,
    size_t height,
    size_t freq=30,
    bool draw_debug=false
) 
```




<hr>



### function attach\_to\_body 

```C++
virtual void robot_dart::gui::magnum::sensor::Camera::attach_to_body (
    dart::dynamics::BodyNode * body,
    const Eigen::Isometry3d & tf=Eigen::Isometry3d::Identity()
) override
```



Implements [*robot\_dart::sensor::Sensor::attach\_to\_body*](classrobot__dart_1_1sensor_1_1Sensor.md#function-attach_to_body-12)


<hr>



### function attach\_to\_joint 

```C++
inline virtual void robot_dart::gui::magnum::sensor::Camera::attach_to_joint (
    dart::dynamics::Joint *,
    const Eigen::Isometry3d &
) override
```



Implements [*robot\_dart::sensor::Sensor::attach\_to\_joint*](classrobot__dart_1_1sensor_1_1Sensor.md#function-attach_to_joint-12)


<hr>



### function calculate 

```C++
virtual void robot_dart::gui::magnum::sensor::Camera::calculate (
    double
) override
```



Implements [*robot\_dart::sensor::Sensor::calculate*](classrobot__dart_1_1sensor_1_1Sensor.md#function-calculate)


<hr>



### function camera [1/2]

```C++
inline gs::Camera & robot_dart::gui::magnum::sensor::Camera::camera () 
```




<hr>



### function camera [2/2]

```C++
inline const gs::Camera & robot_dart::gui::magnum::sensor::Camera::camera () const
```




<hr>



### function camera\_extrinsic\_matrix 

```C++
Eigen::Matrix4d robot_dart::gui::magnum::sensor::Camera::camera_extrinsic_matrix () const
```




<hr>



### function camera\_intrinsic\_matrix 

```C++
Eigen::Matrix3d robot_dart::gui::magnum::sensor::Camera::camera_intrinsic_matrix () const
```




<hr>



### function depth\_array 

```C++
DepthImage robot_dart::gui::magnum::sensor::Camera::depth_array () 
```




<hr>



### function depth\_image 

```C++
GrayscaleImage robot_dart::gui::magnum::sensor::Camera::depth_image () 
```




<hr>



### function draw\_debug 

```C++
inline void robot_dart::gui::magnum::sensor::Camera::draw_debug (
    bool draw=true
) 
```




<hr>



### function drawing\_debug 

```C++
inline bool robot_dart::gui::magnum::sensor::Camera::drawing_debug () const
```




<hr>



### function image 

```C++
inline Image robot_dart::gui::magnum::sensor::Camera::image () 
```




<hr>



### function init 

```C++
virtual void robot_dart::gui::magnum::sensor::Camera::init () override
```



Implements [*robot\_dart::sensor::Sensor::init*](classrobot__dart_1_1sensor_1_1Sensor.md#function-init)


<hr>



### function look\_at 

```C++
void robot_dart::gui::magnum::sensor::Camera::look_at (
    const Eigen::Vector3d & camera_pos,
    const Eigen::Vector3d & look_at=Eigen::Vector3d(0, 0, 0),
    const Eigen::Vector3d & up=Eigen::Vector3d(0, 0, 1)
) 
```




<hr>



### function magnum\_depth\_image 

```C++
inline Magnum::Image2D * robot_dart::gui::magnum::sensor::Camera::magnum_depth_image () 
```




<hr>



### function magnum\_image 

```C++
inline Magnum::Image2D * robot_dart::gui::magnum::sensor::Camera::magnum_image () 
```




<hr>



### function raw\_depth\_image 

```C++
GrayscaleImage robot_dart::gui::magnum::sensor::Camera::raw_depth_image () 
```




<hr>



### function record\_video 

```C++
inline void robot_dart::gui::magnum::sensor::Camera::record_video (
    const std::string & video_fname
) 
```




<hr>



### function type 

```C++
virtual std::string robot_dart::gui::magnum::sensor::Camera::type () override const
```



Implements [*robot\_dart::sensor::Sensor::type*](classrobot__dart_1_1sensor_1_1Sensor.md#function-type)


<hr>



### function ~Camera 

```C++
inline robot_dart::gui::magnum::sensor::Camera::~Camera () 
```




<hr>
## Protected Attributes Documentation




### variable \_camera 

```C++
std::unique_ptr<gs::Camera> robot_dart::gui::magnum::sensor::Camera::_camera;
```




<hr>



### variable \_color 

```C++
Magnum::GL::Renderbuffer robot_dart::gui::magnum::sensor::Camera::_color;
```




<hr>



### variable \_depth 

```C++
Magnum::GL::Renderbuffer robot_dart::gui::magnum::sensor::Camera::_depth;
```




<hr>



### variable \_draw\_debug 

```C++
bool robot_dart::gui::magnum::sensor::Camera::_draw_debug;
```




<hr>



### variable \_format 

```C++
Magnum::PixelFormat robot_dart::gui::magnum::sensor::Camera::_format;
```




<hr>



### variable \_framebuffer 

```C++
Magnum::GL::Framebuffer robot_dart::gui::magnum::sensor::Camera::_framebuffer;
```




<hr>



### variable \_height 

```C++
size_t robot_dart::gui::magnum::sensor::Camera::_height;
```




<hr>



### variable \_magnum\_app 

```C++
BaseApplication* robot_dart::gui::magnum::sensor::Camera::_magnum_app;
```




<hr>



### variable \_width 

```C++
size_t robot_dart::gui::magnum::sensor::Camera::_width;
```




<hr>

------------------------------
The documentation for this class was generated from the following file `robot_dart/gui/magnum/sensor/camera.hpp`

