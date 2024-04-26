

# Struct robot\_dart::gui::magnum::GlobalData



[**ClassList**](annotated.md) **>** [**robot\_dart**](namespacerobot__dart.md) **>** [**gui**](namespacerobot__dart_1_1gui.md) **>** [**magnum**](namespacerobot__dart_1_1gui_1_1magnum.md) **>** [**GlobalData**](structrobot__dart_1_1gui_1_1magnum_1_1GlobalData.md)










































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**GlobalData**](#function-globaldata-12) (const [**GlobalData**](structrobot__dart_1_1gui_1_1magnum_1_1GlobalData.md) &) = delete<br> |
|  void | [**free\_gl\_context**](#function-free_gl_context) (Magnum::Platform::WindowlessGLContext \* context) <br> |
|  Magnum::Platform::WindowlessGLContext \* | [**gl\_context**](#function-gl_context) () <br> |
|  void | [**operator=**](#function-operator) (const [**GlobalData**](structrobot__dart_1_1gui_1_1magnum_1_1GlobalData.md) &) = delete<br> |
|  void | [**set\_max\_contexts**](#function-set_max_contexts) (size\_t N) <br> |


## Public Static Functions

| Type | Name |
| ---: | :--- |
|  [**GlobalData**](structrobot__dart_1_1gui_1_1magnum_1_1GlobalData.md) \* | [**instance**](#function-instance) () <br> |


























## Public Functions Documentation




### function GlobalData [1/2]

```C++
robot_dart::gui::magnum::GlobalData::GlobalData (
    const GlobalData &
) = delete
```




<hr>



### function free\_gl\_context 

```C++
void robot_dart::gui::magnum::GlobalData::free_gl_context (
    Magnum::Platform::WindowlessGLContext * context
) 
```




<hr>



### function gl\_context 

```C++
Magnum::Platform::WindowlessGLContext * robot_dart::gui::magnum::GlobalData::gl_context () 
```




<hr>



### function operator= 

```C++
void robot_dart::gui::magnum::GlobalData::operator= (
    const GlobalData &
) = delete
```




<hr>



### function set\_max\_contexts 

```C++
void robot_dart::gui::magnum::GlobalData::set_max_contexts (
    size_t N
) 
```




<hr>
## Public Static Functions Documentation




### function instance 

```C++
static inline GlobalData * robot_dart::gui::magnum::GlobalData::instance () 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `robot_dart/gui/magnum/base_application.hpp`

