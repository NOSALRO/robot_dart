

# Class robot\_dart::gui::magnum::BaseGraphics

**template &lt;typename T typename T&gt;**



[**ClassList**](annotated.md) **>** [**robot\_dart**](namespacerobot__dart.md) **>** [**gui**](namespacerobot__dart_1_1gui.md) **>** [**magnum**](namespacerobot__dart_1_1gui_1_1magnum.md) **>** [**BaseGraphics**](classrobot__dart_1_1gui_1_1magnum_1_1BaseGraphics.md)








Inherits the following classes: [robot\_dart::gui::Base](classrobot__dart_1_1gui_1_1Base.md)






















































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**BaseGraphics**](#function-basegraphics) (const [**GraphicsConfiguration**](structrobot__dart_1_1gui_1_1magnum_1_1GraphicsConfiguration.md) & configuration=[**GraphicsConfiguration**](structrobot__dart_1_1gui_1_1magnum_1_1GraphicsConfiguration.md)()) <br> |
|  void | [**add\_light**](#function-add_light) (const [**magnum::gs::Light**](classrobot__dart_1_1gui_1_1magnum_1_1gs_1_1Light.md) & light) <br> |
|  [**gs::Camera**](classrobot__dart_1_1gui_1_1magnum_1_1gs_1_1Camera.md) & | [**camera**](#function-camera-12) () <br> |
|  const [**gs::Camera**](classrobot__dart_1_1gui_1_1magnum_1_1gs_1_1Camera.md) & | [**camera**](#function-camera-22) () const<br> |
|  Eigen::Matrix4d | [**camera\_extrinsic\_matrix**](#function-camera_extrinsic_matrix) () const<br> |
|  Eigen::Matrix3d | [**camera\_intrinsic\_matrix**](#function-camera_intrinsic_matrix) () const<br> |
|  void | [**clear\_lights**](#function-clear_lights) () <br> |
| virtual [**DepthImage**](structrobot__dart_1_1gui_1_1DepthImage.md) | [**depth\_array**](#function-depth_array) () override<br> |
| virtual [**GrayscaleImage**](structrobot__dart_1_1gui_1_1GrayscaleImage.md) | [**depth\_image**](#function-depth_image) () override<br> |
| virtual bool | [**done**](#function-done) () override const<br> |
|  void | [**enable\_shadows**](#function-enable_shadows) (bool enable=true, bool transparent=true) <br> |
| virtual size\_t | [**height**](#function-height) () override const<br> |
| virtual [**Image**](structrobot__dart_1_1gui_1_1Image.md) | [**image**](#function-image) () override<br> |
|  [**magnum::gs::Light**](classrobot__dart_1_1gui_1_1magnum_1_1gs_1_1Light.md) & | [**light**](#function-light) (size\_t i) <br> |
|  std::vector&lt; [**gs::Light**](classrobot__dart_1_1gui_1_1magnum_1_1gs_1_1Light.md) &gt; & | [**lights**](#function-lights) () <br> |
|  void | [**look\_at**](#function-look_at) (const Eigen::Vector3d & camera\_pos, const Eigen::Vector3d & look\_at=Eigen::Vector3d(0, 0, 0), const Eigen::Vector3d & up=Eigen::Vector3d(0, 0, 1)) <br> |
|  [**BaseApplication**](classrobot__dart_1_1gui_1_1magnum_1_1BaseApplication.md) \* | [**magnum\_app**](#function-magnum_app-12) () <br> |
|  const [**BaseApplication**](classrobot__dart_1_1gui_1_1magnum_1_1BaseApplication.md) \* | [**magnum\_app**](#function-magnum_app-22) () const<br> |
|  Magnum::Image2D \* | [**magnum\_image**](#function-magnum_image) () <br> |
|  size\_t | [**num\_lights**](#function-num_lights) () const<br> |
| virtual [**GrayscaleImage**](structrobot__dart_1_1gui_1_1GrayscaleImage.md) | [**raw\_depth\_image**](#function-raw_depth_image) () override<br> |
|  void | [**record\_video**](#function-record_video) (const std::string & video\_fname, int fps=-1) <br> |
| virtual void | [**refresh**](#function-refresh) () override<br> |
| virtual void | [**set\_enable**](#function-set_enable) (bool enable) override<br> |
| virtual void | [**set\_fps**](#function-set_fps) (int fps) override<br> |
| virtual void | [**set\_simu**](#function-set_simu) ([**RobotDARTSimu**](classrobot__dart_1_1RobotDARTSimu.md) \* simu) override<br> |
|  bool | [**shadowed**](#function-shadowed) () const<br> |
|  bool | [**transparent\_shadows**](#function-transparent_shadows) () const<br> |
| virtual size\_t | [**width**](#function-width) () override const<br> |
| virtual  | [**~BaseGraphics**](#function-basegraphics) () <br> |


## Public Functions inherited from robot_dart::gui::Base

See [robot\_dart::gui::Base](classrobot__dart_1_1gui_1_1Base.md)

| Type | Name |
| ---: | :--- |
|   | [**Base**](#function-base) () <br> |
| virtual [**DepthImage**](structrobot__dart_1_1gui_1_1DepthImage.md) | [**depth\_array**](#function-depth_array) () <br> |
| virtual [**GrayscaleImage**](structrobot__dart_1_1gui_1_1GrayscaleImage.md) | [**depth\_image**](#function-depth_image) () <br> |
| virtual bool | [**done**](#function-done) () const<br> |
| virtual size\_t | [**height**](#function-height) () const<br> |
| virtual [**Image**](structrobot__dart_1_1gui_1_1Image.md) | [**image**](#function-image) () <br> |
| virtual [**GrayscaleImage**](structrobot__dart_1_1gui_1_1GrayscaleImage.md) | [**raw\_depth\_image**](#function-raw_depth_image) () <br> |
| virtual void | [**refresh**](#function-refresh) () <br> |
| virtual void | [**set\_enable**](#function-set_enable) (bool) <br> |
| virtual void | [**set\_fps**](#function-set_fps) (int) <br> |
| virtual void | [**set\_render\_period**](#function-set_render_period) (double) <br> |
| virtual void | [**set\_simu**](#function-set_simu) ([**RobotDARTSimu**](classrobot__dart_1_1RobotDARTSimu.md) \* simu) <br> |
|  const [**RobotDARTSimu**](classrobot__dart_1_1RobotDARTSimu.md) \* | [**simu**](#function-simu) () const<br> |
| virtual size\_t | [**width**](#function-width) () const<br> |
| virtual  | [**~Base**](#function-base) () <br> |














## Protected Attributes

| Type | Name |
| ---: | :--- |
|  [**GraphicsConfiguration**](structrobot__dart_1_1gui_1_1magnum_1_1GraphicsConfiguration.md) | [**\_configuration**](#variable-_configuration)  <br> |
|  bool | [**\_enabled**](#variable-_enabled)  <br> |
|  int | [**\_fps**](#variable-_fps)  <br> |
|  std::unique\_ptr&lt; [**BaseApplication**](classrobot__dart_1_1gui_1_1magnum_1_1BaseApplication.md) &gt; | [**\_magnum\_app**](#variable-_magnum_app)  <br> |
|  Corrade::Utility::Debug | [**\_magnum\_silence\_output**](#variable-_magnum_silence_output)   = {nullptr}<br> |


## Protected Attributes inherited from robot_dart::gui::Base

See [robot\_dart::gui::Base](classrobot__dart_1_1gui_1_1Base.md)

| Type | Name |
| ---: | :--- |
|  [**RobotDARTSimu**](classrobot__dart_1_1RobotDARTSimu.md) \* | [**\_simu**](#variable-_simu)   = = nullptr<br> |






































## Public Functions Documentation




### function BaseGraphics 

```C++
inline robot_dart::gui::magnum::BaseGraphics::BaseGraphics (
    const GraphicsConfiguration & configuration=GraphicsConfiguration ()
) 
```






### function add\_light 

```C++
inline void robot_dart::gui::magnum::BaseGraphics::add_light (
    const magnum::gs::Light & light
) 
```






### function camera [1/2]

```C++
inline gs::Camera & robot_dart::gui::magnum::BaseGraphics::camera () 
```






### function camera [2/2]

```C++
inline const gs::Camera & robot_dart::gui::magnum::BaseGraphics::camera () const
```






### function camera\_extrinsic\_matrix 

```C++
inline Eigen::Matrix4d robot_dart::gui::magnum::BaseGraphics::camera_extrinsic_matrix () const
```






### function camera\_intrinsic\_matrix 

```C++
inline Eigen::Matrix3d robot_dart::gui::magnum::BaseGraphics::camera_intrinsic_matrix () const
```






### function clear\_lights 

```C++
inline void robot_dart::gui::magnum::BaseGraphics::clear_lights () 
```






### function depth\_array 

```C++
inline virtual DepthImage robot_dart::gui::magnum::BaseGraphics::depth_array () override
```



Implements [*robot\_dart::gui::Base::depth\_array*](classrobot__dart_1_1gui_1_1Base.md#function-depth_array)




### function depth\_image 

```C++
inline virtual GrayscaleImage robot_dart::gui::magnum::BaseGraphics::depth_image () override
```



Implements [*robot\_dart::gui::Base::depth\_image*](classrobot__dart_1_1gui_1_1Base.md#function-depth_image)




### function done 

```C++
inline virtual bool robot_dart::gui::magnum::BaseGraphics::done () override const
```



Implements [*robot\_dart::gui::Base::done*](classrobot__dart_1_1gui_1_1Base.md#function-done)




### function enable\_shadows 

```C++
inline void robot_dart::gui::magnum::BaseGraphics::enable_shadows (
    bool enable=true,
    bool transparent=true
) 
```






### function height 

```C++
inline virtual size_t robot_dart::gui::magnum::BaseGraphics::height () override const
```



Implements [*robot\_dart::gui::Base::height*](classrobot__dart_1_1gui_1_1Base.md#function-height)




### function image 

```C++
inline virtual Image robot_dart::gui::magnum::BaseGraphics::image () override
```



Implements [*robot\_dart::gui::Base::image*](classrobot__dart_1_1gui_1_1Base.md#function-image)




### function light 

```C++
inline magnum::gs::Light & robot_dart::gui::magnum::BaseGraphics::light (
    size_t i
) 
```






### function lights 

```C++
inline std::vector< gs::Light > & robot_dart::gui::magnum::BaseGraphics::lights () 
```






### function look\_at 

```C++
inline void robot_dart::gui::magnum::BaseGraphics::look_at (
    const Eigen::Vector3d & camera_pos,
    const Eigen::Vector3d & look_at=Eigen::Vector3d(0, 0, 0),
    const Eigen::Vector3d & up=Eigen::Vector3d(0, 0, 1)
) 
```






### function magnum\_app [1/2]

```C++
inline BaseApplication * robot_dart::gui::magnum::BaseGraphics::magnum_app () 
```






### function magnum\_app [2/2]

```C++
inline const BaseApplication * robot_dart::gui::magnum::BaseGraphics::magnum_app () const
```






### function magnum\_image 

```C++
inline Magnum::Image2D * robot_dart::gui::magnum::BaseGraphics::magnum_image () 
```






### function num\_lights 

```C++
inline size_t robot_dart::gui::magnum::BaseGraphics::num_lights () const
```






### function raw\_depth\_image 

```C++
inline virtual GrayscaleImage robot_dart::gui::magnum::BaseGraphics::raw_depth_image () override
```



Implements [*robot\_dart::gui::Base::raw\_depth\_image*](classrobot__dart_1_1gui_1_1Base.md#function-raw_depth_image)




### function record\_video 

```C++
inline void robot_dart::gui::magnum::BaseGraphics::record_video (
    const std::string & video_fname,
    int fps=-1
) 
```






### function refresh 

```C++
inline virtual void robot_dart::gui::magnum::BaseGraphics::refresh () override
```



Implements [*robot\_dart::gui::Base::refresh*](classrobot__dart_1_1gui_1_1Base.md#function-refresh)




### function set\_enable 

```C++
inline virtual void robot_dart::gui::magnum::BaseGraphics::set_enable (
    bool enable
) override
```



Implements [*robot\_dart::gui::Base::set\_enable*](classrobot__dart_1_1gui_1_1Base.md#function-set_enable)




### function set\_fps 

```C++
inline virtual void robot_dart::gui::magnum::BaseGraphics::set_fps (
    int fps
) override
```



Implements [*robot\_dart::gui::Base::set\_fps*](classrobot__dart_1_1gui_1_1Base.md#function-set_fps)




### function set\_simu 

```C++
inline virtual void robot_dart::gui::magnum::BaseGraphics::set_simu (
    RobotDARTSimu * simu
) override
```



Implements [*robot\_dart::gui::Base::set\_simu*](classrobot__dart_1_1gui_1_1Base.md#function-set_simu)




### function shadowed 

```C++
inline bool robot_dart::gui::magnum::BaseGraphics::shadowed () const
```






### function transparent\_shadows 

```C++
inline bool robot_dart::gui::magnum::BaseGraphics::transparent_shadows () const
```






### function width 

```C++
inline virtual size_t robot_dart::gui::magnum::BaseGraphics::width () override const
```



Implements [*robot\_dart::gui::Base::width*](classrobot__dart_1_1gui_1_1Base.md#function-width)




### function ~BaseGraphics 

```C++
inline virtual robot_dart::gui::magnum::BaseGraphics::~BaseGraphics () 
```



## Protected Attributes Documentation




### variable \_configuration 

```C++
GraphicsConfiguration robot_dart::gui::magnum::BaseGraphics< T >::_configuration;
```






### variable \_enabled 

```C++
bool robot_dart::gui::magnum::BaseGraphics< T >::_enabled;
```






### variable \_fps 

```C++
int robot_dart::gui::magnum::BaseGraphics< T >::_fps;
```






### variable \_magnum\_app 

```C++
std::unique_ptr<BaseApplication> robot_dart::gui::magnum::BaseGraphics< T >::_magnum_app;
```






### variable \_magnum\_silence\_output 

```C++
Corrade::Utility::Debug robot_dart::gui::magnum::BaseGraphics< T >::_magnum_silence_output;
```




------------------------------
The documentation for this class was generated from the following file `robot_dart/gui/magnum/base_graphics.hpp`

