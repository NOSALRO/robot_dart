

# Class robot\_dart::gui::Base



[**ClassList**](annotated.md) **>** [**robot\_dart**](namespacerobot__dart.md) **>** [**gui**](namespacerobot__dart_1_1gui.md) **>** [**Base**](classrobot__dart_1_1gui_1_1Base.md)










Inherited by the following classes: [robot\_dart::gui::magnum::BaseGraphics](classrobot__dart_1_1gui_1_1magnum_1_1BaseGraphics.md),  [robot\_dart::gui::magnum::BaseGraphics](classrobot__dart_1_1gui_1_1magnum_1_1BaseGraphics.md),  [robot\_dart::gui::magnum::BaseGraphics](classrobot__dart_1_1gui_1_1magnum_1_1BaseGraphics.md)
































## Public Functions

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
|  [**RobotDARTSimu**](classrobot__dart_1_1RobotDARTSimu.md) \* | [**\_simu**](#variable-_simu)   = = nullptr<br> |




















## Public Functions Documentation




### function Base 

```C++
inline robot_dart::gui::Base::Base () 
```




<hr>



### function depth\_array 

```C++
inline virtual DepthImage robot_dart::gui::Base::depth_array () 
```




<hr>



### function depth\_image 

```C++
inline virtual GrayscaleImage robot_dart::gui::Base::depth_image () 
```




<hr>



### function done 

```C++
inline virtual bool robot_dart::gui::Base::done () const
```




<hr>



### function height 

```C++
inline virtual size_t robot_dart::gui::Base::height () const
```




<hr>



### function image 

```C++
inline virtual Image robot_dart::gui::Base::image () 
```




<hr>



### function raw\_depth\_image 

```C++
inline virtual GrayscaleImage robot_dart::gui::Base::raw_depth_image () 
```




<hr>



### function refresh 

```C++
inline virtual void robot_dart::gui::Base::refresh () 
```




<hr>



### function set\_enable 

```C++
inline virtual void robot_dart::gui::Base::set_enable (
    bool
) 
```




<hr>



### function set\_fps 

```C++
inline virtual void robot_dart::gui::Base::set_fps (
    int
) 
```




<hr>



### function set\_render\_period 

```C++
inline virtual void robot_dart::gui::Base::set_render_period (
    double
) 
```




<hr>



### function set\_simu 

```C++
inline virtual void robot_dart::gui::Base::set_simu (
    RobotDARTSimu * simu
) 
```




<hr>



### function simu 

```C++
inline const RobotDARTSimu * robot_dart::gui::Base::simu () const
```




<hr>



### function width 

```C++
inline virtual size_t robot_dart::gui::Base::width () const
```




<hr>



### function ~Base 

```C++
inline virtual robot_dart::gui::Base::~Base () 
```




<hr>
## Protected Attributes Documentation




### variable \_simu 

```C++
RobotDARTSimu* robot_dart::gui::Base::_simu;
```




<hr>

------------------------------
The documentation for this class was generated from the following file `robot_dart/gui/base.hpp`

