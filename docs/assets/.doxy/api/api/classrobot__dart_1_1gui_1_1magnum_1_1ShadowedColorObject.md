

# Class robot\_dart::gui::magnum::ShadowedColorObject



[**ClassList**](annotated.md) **>** [**robot\_dart**](namespacerobot__dart.md) **>** [**gui**](namespacerobot__dart_1_1gui.md) **>** [**magnum**](namespacerobot__dart_1_1gui_1_1magnum.md) **>** [**ShadowedColorObject**](classrobot__dart_1_1gui_1_1magnum_1_1ShadowedColorObject.md)








Inherits the following classes: Object3D,  Magnum::SceneGraph::Drawable3D


































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**ShadowedColorObject**](#function-shadowedcolorobject) ([**RobotDARTSimu**](classrobot__dart_1_1RobotDARTSimu.md) \* simu, dart::dynamics::ShapeNode \* shape, const std::vector&lt; std::reference\_wrapper&lt; Magnum::GL::Mesh &gt; &gt; & meshes, [**gs::ShadowMapColor**](classrobot__dart_1_1gui_1_1magnum_1_1gs_1_1ShadowMapColor.md) & shader, [**gs::ShadowMapColor**](classrobot__dart_1_1gui_1_1magnum_1_1gs_1_1ShadowMapColor.md) & texture\_shader, Object3D \* parent, Magnum::SceneGraph::DrawableGroup3D \* group) <br> |
|  [**ShadowedColorObject**](classrobot__dart_1_1gui_1_1magnum_1_1ShadowedColorObject.md) & | [**set\_materials**](#function-set_materials) (const std::vector&lt; [**gs::Material**](classrobot__dart_1_1gui_1_1magnum_1_1gs_1_1Material.md) &gt; & materials) <br> |
|  [**ShadowedColorObject**](classrobot__dart_1_1gui_1_1magnum_1_1ShadowedColorObject.md) & | [**set\_meshes**](#function-set_meshes) (const std::vector&lt; std::reference\_wrapper&lt; Magnum::GL::Mesh &gt; &gt; & meshes) <br> |
|  [**ShadowedColorObject**](classrobot__dart_1_1gui_1_1magnum_1_1ShadowedColorObject.md) & | [**set\_scalings**](#function-set_scalings) (const std::vector&lt; Magnum::Vector3 &gt; & scalings) <br> |
|  dart::dynamics::ShapeNode \* | [**shape**](#function-shape) () const<br> |
|  [**RobotDARTSimu**](classrobot__dart_1_1RobotDARTSimu.md) \* | [**simu**](#function-simu) () const<br> |




























## Public Functions Documentation




### function ShadowedColorObject 

```C++
explicit robot_dart::gui::magnum::ShadowedColorObject::ShadowedColorObject (
    RobotDARTSimu * simu,
    dart::dynamics::ShapeNode * shape,
    const std::vector< std::reference_wrapper< Magnum::GL::Mesh > > & meshes,
    gs::ShadowMapColor & shader,
    gs::ShadowMapColor & texture_shader,
    Object3D * parent,
    Magnum::SceneGraph::DrawableGroup3D * group
) 
```




<hr>



### function set\_materials 

```C++
ShadowedColorObject & robot_dart::gui::magnum::ShadowedColorObject::set_materials (
    const std::vector< gs::Material > & materials
) 
```




<hr>



### function set\_meshes 

```C++
ShadowedColorObject & robot_dart::gui::magnum::ShadowedColorObject::set_meshes (
    const std::vector< std::reference_wrapper< Magnum::GL::Mesh > > & meshes
) 
```




<hr>



### function set\_scalings 

```C++
ShadowedColorObject & robot_dart::gui::magnum::ShadowedColorObject::set_scalings (
    const std::vector< Magnum::Vector3 > & scalings
) 
```




<hr>



### function shape 

```C++
inline dart::dynamics::ShapeNode * robot_dart::gui::magnum::ShadowedColorObject::shape () const
```




<hr>



### function simu 

```C++
inline RobotDARTSimu * robot_dart::gui::magnum::ShadowedColorObject::simu () const
```




<hr>

------------------------------
The documentation for this class was generated from the following file `robot_dart/gui/magnum/drawables.hpp`

