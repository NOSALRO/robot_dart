

# Class robot\_dart::gui::magnum::gs::Camera



[**ClassList**](annotated.md) **>** [**robot\_dart**](namespacerobot__dart.md) **>** [**gui**](namespacerobot__dart_1_1gui.md) **>** [**magnum**](namespacerobot__dart_1_1gui_1_1magnum.md) **>** [**gs**](namespacerobot__dart_1_1gui_1_1magnum_1_1gs.md) **>** [**Camera**](classrobot__dart_1_1gui_1_1magnum_1_1gs_1_1Camera.md)








Inherits the following classes: Object3D


































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**Camera**](#function-camera) (Object3D & object, Magnum::Int width, Magnum::Int height) <br> |
|  Camera3D & | [**camera**](#function-camera) () const<br> |
|  Object3D & | [**camera\_object**](#function-camera_object) () const<br> |
|  Corrade::Containers::Optional&lt; Magnum::Image2D &gt; & | [**depth\_image**](#function-depth_image) () <br> |
|  void | [**draw**](#function-draw) (Magnum::SceneGraph::DrawableGroup3D & drawables, Magnum::GL::AbstractFramebuffer & framebuffer, Magnum::PixelFormat format, [**RobotDARTSimu**](classrobot__dart_1_1RobotDARTSimu.md) \* simu, const [**DebugDrawData**](structrobot__dart_1_1gui_1_1magnum_1_1DebugDrawData.md) & debug\_data, bool draw\_debug=true) <br> |
|  Magnum::Matrix4 | [**extrinsic\_matrix**](#function-extrinsic_matrix) () const<br> |
|  Magnum::Float | [**far\_plane**](#function-far_plane) () const<br> |
|  [**Camera**](classrobot__dart_1_1gui_1_1magnum_1_1gs_1_1Camera.md) & | [**forward**](#function-forward) (Magnum::Float speed) <br> |
|  Magnum::Float | [**fov**](#function-fov) () const<br> |
|  Magnum::Int | [**height**](#function-height) () const<br> |
|  Corrade::Containers::Optional&lt; Magnum::Image2D &gt; & | [**image**](#function-image) () <br> |
|  Magnum::Matrix3 | [**intrinsic\_matrix**](#function-intrinsic_matrix) () const<br> |
|  [**Camera**](classrobot__dart_1_1gui_1_1magnum_1_1gs_1_1Camera.md) & | [**look\_at**](#function-look_at) (const Magnum::Vector3 & camera, const Magnum::Vector3 & center, const Magnum::Vector3 & up=Magnum::Vector3::zAxis()) <br> |
|  [**Camera**](classrobot__dart_1_1gui_1_1magnum_1_1gs_1_1Camera.md) & | [**move**](#function-move) (const Magnum::Vector2i & shift) <br> |
|  Magnum::Float | [**near\_plane**](#function-near_plane) () const<br> |
|  void | [**record**](#function-record) (bool recording, bool recording\_depth=false) <br> |
|  void | [**record\_video**](#function-record_video) (const std::string & video\_fname, int fps) <br> |
|  bool | [**recording**](#function-recording) () <br> |
|  bool | [**recording\_depth**](#function-recording_depth) () <br> |
|  Object3D & | [**root\_object**](#function-root_object) () <br> |
|  [**Camera**](classrobot__dart_1_1gui_1_1magnum_1_1gs_1_1Camera.md) & | [**set\_camera\_params**](#function-set_camera_params) (Magnum::Float near\_plane, Magnum::Float far\_plane, Magnum::Float fov, Magnum::Int width, Magnum::Int height) <br> |
|  [**Camera**](classrobot__dart_1_1gui_1_1magnum_1_1gs_1_1Camera.md) & | [**set\_far\_plane**](#function-set_far_plane) (Magnum::Float far\_plane) <br> |
|  [**Camera**](classrobot__dart_1_1gui_1_1magnum_1_1gs_1_1Camera.md) & | [**set\_fov**](#function-set_fov) (Magnum::Float fov) <br> |
|  [**Camera**](classrobot__dart_1_1gui_1_1magnum_1_1gs_1_1Camera.md) & | [**set\_near\_plane**](#function-set_near_plane) (Magnum::Float near\_plane) <br> |
|  [**Camera**](classrobot__dart_1_1gui_1_1magnum_1_1gs_1_1Camera.md) & | [**set\_speed**](#function-set_speed) (const Magnum::Vector2 & speed) <br> |
|  [**Camera**](classrobot__dart_1_1gui_1_1magnum_1_1gs_1_1Camera.md) & | [**set\_viewport**](#function-set_viewport) (const Magnum::Vector2i & size) <br> |
|  Magnum::Vector2 | [**speed**](#function-speed) () const<br> |
|  [**Camera**](classrobot__dart_1_1gui_1_1magnum_1_1gs_1_1Camera.md) & | [**strafe**](#function-strafe) (Magnum::Float speed) <br> |
|  void | [**transform\_lights**](#function-transform_lights) (std::vector&lt; [**gs::Light**](classrobot__dart_1_1gui_1_1magnum_1_1gs_1_1Light.md) &gt; & lights) const<br> |
|  Magnum::Int | [**width**](#function-width) () const<br> |
|   | [**~Camera**](#function-camera) () <br> |




























## Public Functions Documentation




### function Camera 

```C++
explicit robot_dart::gui::magnum::gs::Camera::Camera (
    Object3D & object,
    Magnum::Int width,
    Magnum::Int height
) 
```




<hr>



### function camera 

```C++
Camera3D & robot_dart::gui::magnum::gs::Camera::camera () const
```




<hr>



### function camera\_object 

```C++
Object3D & robot_dart::gui::magnum::gs::Camera::camera_object () const
```




<hr>



### function depth\_image 

```C++
inline Corrade::Containers::Optional< Magnum::Image2D > & robot_dart::gui::magnum::gs::Camera::depth_image () 
```




<hr>



### function draw 

```C++
void robot_dart::gui::magnum::gs::Camera::draw (
    Magnum::SceneGraph::DrawableGroup3D & drawables,
    Magnum::GL::AbstractFramebuffer & framebuffer,
    Magnum::PixelFormat format,
    RobotDARTSimu * simu,
    const DebugDrawData & debug_data,
    bool draw_debug=true
) 
```




<hr>



### function extrinsic\_matrix 

```C++
Magnum::Matrix4 robot_dart::gui::magnum::gs::Camera::extrinsic_matrix () const
```




<hr>



### function far\_plane 

```C++
inline Magnum::Float robot_dart::gui::magnum::gs::Camera::far_plane () const
```




<hr>



### function forward 

```C++
Camera & robot_dart::gui::magnum::gs::Camera::forward (
    Magnum::Float speed
) 
```




<hr>



### function fov 

```C++
inline Magnum::Float robot_dart::gui::magnum::gs::Camera::fov () const
```




<hr>



### function height 

```C++
inline Magnum::Int robot_dart::gui::magnum::gs::Camera::height () const
```




<hr>



### function image 

```C++
inline Corrade::Containers::Optional< Magnum::Image2D > & robot_dart::gui::magnum::gs::Camera::image () 
```




<hr>



### function intrinsic\_matrix 

```C++
Magnum::Matrix3 robot_dart::gui::magnum::gs::Camera::intrinsic_matrix () const
```




<hr>



### function look\_at 

```C++
Camera & robot_dart::gui::magnum::gs::Camera::look_at (
    const Magnum::Vector3 & camera,
    const Magnum::Vector3 & center,
    const Magnum::Vector3 & up=Magnum::Vector3::zAxis()
) 
```




<hr>



### function move 

```C++
Camera & robot_dart::gui::magnum::gs::Camera::move (
    const Magnum::Vector2i & shift
) 
```




<hr>



### function near\_plane 

```C++
inline Magnum::Float robot_dart::gui::magnum::gs::Camera::near_plane () const
```




<hr>



### function record 

```C++
inline void robot_dart::gui::magnum::gs::Camera::record (
    bool recording,
    bool recording_depth=false
) 
```




<hr>



### function record\_video 

```C++
void robot_dart::gui::magnum::gs::Camera::record_video (
    const std::string & video_fname,
    int fps
) 
```




<hr>



### function recording 

```C++
inline bool robot_dart::gui::magnum::gs::Camera::recording () 
```




<hr>



### function recording\_depth 

```C++
inline bool robot_dart::gui::magnum::gs::Camera::recording_depth () 
```




<hr>



### function root\_object 

```C++
Object3D & robot_dart::gui::magnum::gs::Camera::root_object () 
```




<hr>



### function set\_camera\_params 

```C++
Camera & robot_dart::gui::magnum::gs::Camera::set_camera_params (
    Magnum::Float near_plane,
    Magnum::Float far_plane,
    Magnum::Float fov,
    Magnum::Int width,
    Magnum::Int height
) 
```




<hr>



### function set\_far\_plane 

```C++
Camera & robot_dart::gui::magnum::gs::Camera::set_far_plane (
    Magnum::Float far_plane
) 
```




<hr>



### function set\_fov 

```C++
Camera & robot_dart::gui::magnum::gs::Camera::set_fov (
    Magnum::Float fov
) 
```




<hr>



### function set\_near\_plane 

```C++
Camera & robot_dart::gui::magnum::gs::Camera::set_near_plane (
    Magnum::Float near_plane
) 
```




<hr>



### function set\_speed 

```C++
Camera & robot_dart::gui::magnum::gs::Camera::set_speed (
    const Magnum::Vector2 & speed
) 
```




<hr>



### function set\_viewport 

```C++
Camera & robot_dart::gui::magnum::gs::Camera::set_viewport (
    const Magnum::Vector2i & size
) 
```




<hr>



### function speed 

```C++
inline Magnum::Vector2 robot_dart::gui::magnum::gs::Camera::speed () const
```




<hr>



### function strafe 

```C++
Camera & robot_dart::gui::magnum::gs::Camera::strafe (
    Magnum::Float speed
) 
```




<hr>



### function transform\_lights 

```C++
void robot_dart::gui::magnum::gs::Camera::transform_lights (
    std::vector< gs::Light > & lights
) const
```




<hr>



### function width 

```C++
inline Magnum::Int robot_dart::gui::magnum::gs::Camera::width () const
```




<hr>



### function ~Camera 

```C++
robot_dart::gui::magnum::gs::Camera::~Camera () 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `robot_dart/gui/magnum/gs/camera.hpp`

