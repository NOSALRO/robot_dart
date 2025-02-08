

# Class robot\_dart::gui::magnum::CubeMapShadowedObject



[**ClassList**](annotated.md) **>** [**robot\_dart**](namespacerobot__dart.md) **>** [**gui**](namespacerobot__dart_1_1gui.md) **>** [**magnum**](namespacerobot__dart_1_1gui_1_1magnum.md) **>** [**CubeMapShadowedObject**](classrobot__dart_1_1gui_1_1magnum_1_1CubeMapShadowedObject.md)








Inherits the following classes: Object3D,  Magnum::SceneGraph::Drawable3D


































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**CubeMapShadowedObject**](#function-cubemapshadowedobject) ([**RobotDARTSimu**](classrobot__dart_1_1RobotDARTSimu.md) \* simu, dart::dynamics::ShapeNode \* shape, const std::vector&lt; std::reference\_wrapper&lt; Magnum::GL::Mesh &gt; &gt; & meshes, [**gs::CubeMap**](classrobot__dart_1_1gui_1_1magnum_1_1gs_1_1CubeMap.md) & shader, [**gs::CubeMap**](classrobot__dart_1_1gui_1_1magnum_1_1gs_1_1CubeMap.md) & texture\_shader, Object3D \* parent, Magnum::SceneGraph::DrawableGroup3D \* group) <br> |
|  [**CubeMapShadowedObject**](classrobot__dart_1_1gui_1_1magnum_1_1CubeMapShadowedObject.md) & | [**set\_materials**](#function-set_materials) (const std::vector&lt; [**gs::Material**](classrobot__dart_1_1gui_1_1magnum_1_1gs_1_1Material.md) &gt; & materials) <br> |
|  [**CubeMapShadowedObject**](classrobot__dart_1_1gui_1_1magnum_1_1CubeMapShadowedObject.md) & | [**set\_meshes**](#function-set_meshes) (const std::vector&lt; std::reference\_wrapper&lt; Magnum::GL::Mesh &gt; &gt; & meshes) <br> |
|  [**CubeMapShadowedObject**](classrobot__dart_1_1gui_1_1magnum_1_1CubeMapShadowedObject.md) & | [**set\_scalings**](#function-set_scalings) (const std::vector&lt; Magnum::Vector3 &gt; & scalings) <br> |
|  dart::dynamics::ShapeNode \* | [**shape**](#function-shape) () const<br> |
|  [**RobotDARTSimu**](classrobot__dart_1_1RobotDARTSimu.md) \* | [**simu**](#function-simu) () const<br> |




























## Public Functions Documentation




### function CubeMapShadowedObject 

```C++
explicit robot_dart::gui::magnum::CubeMapShadowedObject::CubeMapShadowedObject (
    RobotDARTSimu * simu,
    dart::dynamics::ShapeNode * shape,
    const std::vector< std::reference_wrapper< Magnum::GL::Mesh > > & meshes,
    gs::CubeMap & shader,
    gs::CubeMap & texture_shader,
    Object3D * parent,
    Magnum::SceneGraph::DrawableGroup3D * group
) 
```




<hr>



### function set\_materials 

```C++
CubeMapShadowedObject & robot_dart::gui::magnum::CubeMapShadowedObject::set_materials (
    const std::vector< gs::Material > & materials
) 
```




<hr>



### function set\_meshes 

```C++
CubeMapShadowedObject & robot_dart::gui::magnum::CubeMapShadowedObject::set_meshes (
    const std::vector< std::reference_wrapper< Magnum::GL::Mesh > > & meshes
) 
```




<hr>



### function set\_scalings 

```C++
CubeMapShadowedObject & robot_dart::gui::magnum::CubeMapShadowedObject::set_scalings (
    const std::vector< Magnum::Vector3 > & scalings
) 
```




<hr>



### function shape 

```C++
inline dart::dynamics::ShapeNode * robot_dart::gui::magnum::CubeMapShadowedObject::shape () const
```




<hr>



### function simu 

```C++
inline RobotDARTSimu * robot_dart::gui::magnum::CubeMapShadowedObject::simu () const
```




<hr>

------------------------------
The documentation for this class was generated from the following file `robot_dart/gui/magnum/drawables.hpp`

