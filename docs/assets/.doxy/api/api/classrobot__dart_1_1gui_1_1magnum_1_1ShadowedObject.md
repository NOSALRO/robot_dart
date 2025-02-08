

# Class robot\_dart::gui::magnum::ShadowedObject



[**ClassList**](annotated.md) **>** [**robot\_dart**](namespacerobot__dart.md) **>** [**gui**](namespacerobot__dart_1_1gui.md) **>** [**magnum**](namespacerobot__dart_1_1gui_1_1magnum.md) **>** [**ShadowedObject**](classrobot__dart_1_1gui_1_1magnum_1_1ShadowedObject.md)








Inherits the following classes: Object3D,  Magnum::SceneGraph::Drawable3D


































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**ShadowedObject**](#function-shadowedobject) ([**RobotDARTSimu**](classrobot__dart_1_1RobotDARTSimu.md) \* simu, dart::dynamics::ShapeNode \* shape, const std::vector&lt; std::reference\_wrapper&lt; Magnum::GL::Mesh &gt; &gt; & meshes, [**gs::ShadowMap**](classrobot__dart_1_1gui_1_1magnum_1_1gs_1_1ShadowMap.md) & shader, [**gs::ShadowMap**](classrobot__dart_1_1gui_1_1magnum_1_1gs_1_1ShadowMap.md) & texture\_shader, Object3D \* parent, Magnum::SceneGraph::DrawableGroup3D \* group) <br> |
|  [**ShadowedObject**](classrobot__dart_1_1gui_1_1magnum_1_1ShadowedObject.md) & | [**set\_materials**](#function-set_materials) (const std::vector&lt; [**gs::Material**](classrobot__dart_1_1gui_1_1magnum_1_1gs_1_1Material.md) &gt; & materials) <br> |
|  [**ShadowedObject**](classrobot__dart_1_1gui_1_1magnum_1_1ShadowedObject.md) & | [**set\_meshes**](#function-set_meshes) (const std::vector&lt; std::reference\_wrapper&lt; Magnum::GL::Mesh &gt; &gt; & meshes) <br> |
|  [**ShadowedObject**](classrobot__dart_1_1gui_1_1magnum_1_1ShadowedObject.md) & | [**set\_scalings**](#function-set_scalings) (const std::vector&lt; Magnum::Vector3 &gt; & scalings) <br> |
|  dart::dynamics::ShapeNode \* | [**shape**](#function-shape) () const<br> |
|  [**RobotDARTSimu**](classrobot__dart_1_1RobotDARTSimu.md) \* | [**simu**](#function-simu) () const<br> |




























## Public Functions Documentation




### function ShadowedObject 

```C++
explicit robot_dart::gui::magnum::ShadowedObject::ShadowedObject (
    RobotDARTSimu * simu,
    dart::dynamics::ShapeNode * shape,
    const std::vector< std::reference_wrapper< Magnum::GL::Mesh > > & meshes,
    gs::ShadowMap & shader,
    gs::ShadowMap & texture_shader,
    Object3D * parent,
    Magnum::SceneGraph::DrawableGroup3D * group
) 
```




<hr>



### function set\_materials 

```C++
ShadowedObject & robot_dart::gui::magnum::ShadowedObject::set_materials (
    const std::vector< gs::Material > & materials
) 
```




<hr>



### function set\_meshes 

```C++
ShadowedObject & robot_dart::gui::magnum::ShadowedObject::set_meshes (
    const std::vector< std::reference_wrapper< Magnum::GL::Mesh > > & meshes
) 
```




<hr>



### function set\_scalings 

```C++
ShadowedObject & robot_dart::gui::magnum::ShadowedObject::set_scalings (
    const std::vector< Magnum::Vector3 > & scalings
) 
```




<hr>



### function shape 

```C++
inline dart::dynamics::ShapeNode * robot_dart::gui::magnum::ShadowedObject::shape () const
```




<hr>



### function simu 

```C++
inline RobotDARTSimu * robot_dart::gui::magnum::ShadowedObject::simu () const
```




<hr>

------------------------------
The documentation for this class was generated from the following file `robot_dart/gui/magnum/drawables.hpp`

