

# Class robot\_dart::gui::magnum::CubeMapShadowedColorObject



[**ClassList**](annotated.md) **>** [**robot\_dart**](namespacerobot__dart.md) **>** [**gui**](namespacerobot__dart_1_1gui.md) **>** [**magnum**](namespacerobot__dart_1_1gui_1_1magnum.md) **>** [**CubeMapShadowedColorObject**](classrobot__dart_1_1gui_1_1magnum_1_1CubeMapShadowedColorObject.md)








Inherits the following classes: Object3D,  Magnum::SceneGraph::Drawable3D


































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**CubeMapShadowedColorObject**](#function-cubemapshadowedcolorobject) ([**RobotDARTSimu**](classrobot__dart_1_1RobotDARTSimu.md) \* simu, dart::dynamics::ShapeNode \* shape, const std::vector&lt; std::reference\_wrapper&lt; Magnum::GL::Mesh &gt; &gt; & meshes, [**gs::CubeMapColor**](classrobot__dart_1_1gui_1_1magnum_1_1gs_1_1CubeMapColor.md) & shader, [**gs::CubeMapColor**](classrobot__dart_1_1gui_1_1magnum_1_1gs_1_1CubeMapColor.md) & texture\_shader, Object3D \* parent, Magnum::SceneGraph::DrawableGroup3D \* group) <br> |
|  [**CubeMapShadowedColorObject**](classrobot__dart_1_1gui_1_1magnum_1_1CubeMapShadowedColorObject.md) & | [**set\_materials**](#function-set_materials) (const std::vector&lt; [**gs::Material**](classrobot__dart_1_1gui_1_1magnum_1_1gs_1_1Material.md) &gt; & materials) <br> |
|  [**CubeMapShadowedColorObject**](classrobot__dart_1_1gui_1_1magnum_1_1CubeMapShadowedColorObject.md) & | [**set\_meshes**](#function-set_meshes) (const std::vector&lt; std::reference\_wrapper&lt; Magnum::GL::Mesh &gt; &gt; & meshes) <br> |
|  [**CubeMapShadowedColorObject**](classrobot__dart_1_1gui_1_1magnum_1_1CubeMapShadowedColorObject.md) & | [**set\_scalings**](#function-set_scalings) (const std::vector&lt; Magnum::Vector3 &gt; & scalings) <br> |
|  dart::dynamics::ShapeNode \* | [**shape**](#function-shape) () const<br> |
|  [**RobotDARTSimu**](classrobot__dart_1_1RobotDARTSimu.md) \* | [**simu**](#function-simu) () const<br> |




























## Public Functions Documentation




### function CubeMapShadowedColorObject 

```C++
explicit robot_dart::gui::magnum::CubeMapShadowedColorObject::CubeMapShadowedColorObject (
    RobotDARTSimu * simu,
    dart::dynamics::ShapeNode * shape,
    const std::vector< std::reference_wrapper< Magnum::GL::Mesh > > & meshes,
    gs::CubeMapColor & shader,
    gs::CubeMapColor & texture_shader,
    Object3D * parent,
    Magnum::SceneGraph::DrawableGroup3D * group
) 
```






### function set\_materials 

```C++
CubeMapShadowedColorObject & robot_dart::gui::magnum::CubeMapShadowedColorObject::set_materials (
    const std::vector< gs::Material > & materials
) 
```






### function set\_meshes 

```C++
CubeMapShadowedColorObject & robot_dart::gui::magnum::CubeMapShadowedColorObject::set_meshes (
    const std::vector< std::reference_wrapper< Magnum::GL::Mesh > > & meshes
) 
```






### function set\_scalings 

```C++
CubeMapShadowedColorObject & robot_dart::gui::magnum::CubeMapShadowedColorObject::set_scalings (
    const std::vector< Magnum::Vector3 > & scalings
) 
```






### function shape 

```C++
inline dart::dynamics::ShapeNode * robot_dart::gui::magnum::CubeMapShadowedColorObject::shape () const
```






### function simu 

```C++
inline RobotDARTSimu * robot_dart::gui::magnum::CubeMapShadowedColorObject::simu () const
```




------------------------------
The documentation for this class was generated from the following file `robot_dart/gui/magnum/drawables.hpp`

