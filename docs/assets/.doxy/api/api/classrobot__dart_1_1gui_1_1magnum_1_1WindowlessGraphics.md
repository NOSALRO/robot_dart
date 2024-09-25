

# Class robot\_dart::gui::magnum::WindowlessGraphics



[**ClassList**](annotated.md) **>** [**robot\_dart**](namespacerobot__dart.md) **>** [**gui**](namespacerobot__dart_1_1gui.md) **>** [**magnum**](namespacerobot__dart_1_1gui_1_1magnum.md) **>** [**WindowlessGraphics**](classrobot__dart_1_1gui_1_1magnum_1_1WindowlessGraphics.md)








Inherits the following classes: [robot\_dart::gui::magnum::BaseGraphics](classrobot__dart_1_1gui_1_1magnum_1_1BaseGraphics.md)










































































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**WindowlessGraphics**](#function-windowlessgraphics) (const [**GraphicsConfiguration**](structrobot__dart_1_1gui_1_1magnum_1_1GraphicsConfiguration.md) & configuration=default\_configuration()) <br> |
| virtual void | [**set\_simu**](#function-set_simu) ([**RobotDARTSimu**](classrobot__dart_1_1RobotDARTSimu.md) \* simu) override<br> |
|   | [**~WindowlessGraphics**](#function-windowlessgraphics) () <br> |


## Public Functions inherited from robot_dart::gui::magnum::BaseGraphics

See [robot\_dart::gui::magnum::BaseGraphics](classrobot__dart_1_1gui_1_1magnum_1_1BaseGraphics.md)

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


## Public Static Functions

| Type | Name |
| ---: | :--- |
|  [**GraphicsConfiguration**](structrobot__dart_1_1gui_1_1magnum_1_1GraphicsConfiguration.md) | [**default\_configuration**](#function-default_configuration) () <br> |




















## Protected Attributes inherited from robot_dart::gui::magnum::BaseGraphics

See [robot\_dart::gui::magnum::BaseGraphics](classrobot__dart_1_1gui_1_1magnum_1_1BaseGraphics.md)

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




### function WindowlessGraphics 

```C++
inline robot_dart::gui::magnum::WindowlessGraphics::WindowlessGraphics (
    const GraphicsConfiguration & configuration=default_configuration()
) 
```




<hr>



### function set\_simu 

```C++
virtual void robot_dart::gui::magnum::WindowlessGraphics::set_simu (
    RobotDARTSimu * simu
) override
```



Implements [*robot\_dart::gui::magnum::BaseGraphics::set\_simu*](classrobot__dart_1_1gui_1_1magnum_1_1BaseGraphics.md#function-set_simu)


<hr>



### function ~WindowlessGraphics 

```C++
inline robot_dart::gui::magnum::WindowlessGraphics::~WindowlessGraphics () 
```




<hr>
## Public Static Functions Documentation




### function default\_configuration 

```C++
static GraphicsConfiguration robot_dart::gui::magnum::WindowlessGraphics::default_configuration () 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `robot_dart/gui/magnum/windowless_graphics.hpp`

