

# Class robot\_dart::gui::magnum::DrawableObject



[**ClassList**](annotated.md) **>** [**robot\_dart**](namespacerobot__dart.md) **>** [**gui**](namespacerobot__dart_1_1gui.md) **>** [**magnum**](namespacerobot__dart_1_1gui_1_1magnum.md) **>** [**DrawableObject**](classrobot__dart_1_1gui_1_1magnum_1_1DrawableObject.md)








Inherits the following classes: Object3D,  Magnum::SceneGraph::Drawable3D


































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**DrawableObject**](#function-drawableobject) ([**RobotDARTSimu**](classrobot__dart_1_1RobotDARTSimu.md) \* simu, dart::dynamics::ShapeNode \* shape, const std::vector&lt; std::reference\_wrapper&lt; Magnum::GL::Mesh &gt; &gt; & meshes, const std::vector&lt; [**gs::Material**](classrobot__dart_1_1gui_1_1magnum_1_1gs_1_1Material.md) &gt; & materials, [**gs::PhongMultiLight**](classrobot__dart_1_1gui_1_1magnum_1_1gs_1_1PhongMultiLight.md) & color, [**gs::PhongMultiLight**](classrobot__dart_1_1gui_1_1magnum_1_1gs_1_1PhongMultiLight.md) & texture, Object3D \* parent, Magnum::SceneGraph::DrawableGroup3D \* group) <br> |
|  const std::vector&lt; [**gs::Material**](classrobot__dart_1_1gui_1_1magnum_1_1gs_1_1Material.md) &gt; & | [**materials**](#function-materials) () const<br> |
|  [**DrawableObject**](classrobot__dart_1_1gui_1_1magnum_1_1DrawableObject.md) & | [**set\_color\_shader**](#function-set_color_shader) (std::reference\_wrapper&lt; [**gs::PhongMultiLight**](classrobot__dart_1_1gui_1_1magnum_1_1gs_1_1PhongMultiLight.md) &gt; shader) <br> |
|  [**DrawableObject**](classrobot__dart_1_1gui_1_1magnum_1_1DrawableObject.md) & | [**set\_materials**](#function-set_materials) (const std::vector&lt; [**gs::Material**](classrobot__dart_1_1gui_1_1magnum_1_1gs_1_1Material.md) &gt; & materials) <br> |
|  [**DrawableObject**](classrobot__dart_1_1gui_1_1magnum_1_1DrawableObject.md) & | [**set\_meshes**](#function-set_meshes) (const std::vector&lt; std::reference\_wrapper&lt; Magnum::GL::Mesh &gt; &gt; & meshes) <br> |
|  [**DrawableObject**](classrobot__dart_1_1gui_1_1magnum_1_1DrawableObject.md) & | [**set\_scalings**](#function-set_scalings) (const std::vector&lt; Magnum::Vector3 &gt; & scalings) <br> |
|  [**DrawableObject**](classrobot__dart_1_1gui_1_1magnum_1_1DrawableObject.md) & | [**set\_soft\_bodies**](#function-set_soft_bodies) (const std::vector&lt; bool &gt; & softBody) <br> |
|  [**DrawableObject**](classrobot__dart_1_1gui_1_1magnum_1_1DrawableObject.md) & | [**set\_texture\_shader**](#function-set_texture_shader) (std::reference\_wrapper&lt; [**gs::PhongMultiLight**](classrobot__dart_1_1gui_1_1magnum_1_1gs_1_1PhongMultiLight.md) &gt; shader) <br> |
|  [**DrawableObject**](classrobot__dart_1_1gui_1_1magnum_1_1DrawableObject.md) & | [**set\_transparent**](#function-set_transparent) (bool transparent=true) <br> |
|  dart::dynamics::ShapeNode \* | [**shape**](#function-shape) () const<br> |
|  [**RobotDARTSimu**](classrobot__dart_1_1RobotDARTSimu.md) \* | [**simu**](#function-simu) () const<br> |
|  bool | [**transparent**](#function-transparent) () const<br> |




























## Public Functions Documentation




### function DrawableObject 

```C++
explicit robot_dart::gui::magnum::DrawableObject::DrawableObject (
    RobotDARTSimu * simu,
    dart::dynamics::ShapeNode * shape,
    const std::vector< std::reference_wrapper< Magnum::GL::Mesh > > & meshes,
    const std::vector< gs::Material > & materials,
    gs::PhongMultiLight & color,
    gs::PhongMultiLight & texture,
    Object3D * parent,
    Magnum::SceneGraph::DrawableGroup3D * group
) 
```






### function materials 

```C++
inline const std::vector< gs::Material > & robot_dart::gui::magnum::DrawableObject::materials () const
```






### function set\_color\_shader 

```C++
DrawableObject & robot_dart::gui::magnum::DrawableObject::set_color_shader (
    std::reference_wrapper< gs::PhongMultiLight > shader
) 
```






### function set\_materials 

```C++
DrawableObject & robot_dart::gui::magnum::DrawableObject::set_materials (
    const std::vector< gs::Material > & materials
) 
```






### function set\_meshes 

```C++
DrawableObject & robot_dart::gui::magnum::DrawableObject::set_meshes (
    const std::vector< std::reference_wrapper< Magnum::GL::Mesh > > & meshes
) 
```






### function set\_scalings 

```C++
DrawableObject & robot_dart::gui::magnum::DrawableObject::set_scalings (
    const std::vector< Magnum::Vector3 > & scalings
) 
```






### function set\_soft\_bodies 

```C++
DrawableObject & robot_dart::gui::magnum::DrawableObject::set_soft_bodies (
    const std::vector< bool > & softBody
) 
```






### function set\_texture\_shader 

```C++
DrawableObject & robot_dart::gui::magnum::DrawableObject::set_texture_shader (
    std::reference_wrapper< gs::PhongMultiLight > shader
) 
```






### function set\_transparent 

```C++
DrawableObject & robot_dart::gui::magnum::DrawableObject::set_transparent (
    bool transparent=true
) 
```






### function shape 

```C++
inline dart::dynamics::ShapeNode * robot_dart::gui::magnum::DrawableObject::shape () const
```






### function simu 

```C++
inline RobotDARTSimu * robot_dart::gui::magnum::DrawableObject::simu () const
```






### function transparent 

```C++
inline bool robot_dart::gui::magnum::DrawableObject::transparent () const
```




------------------------------
The documentation for this class was generated from the following file `robot_dart/gui/magnum/drawables.hpp`

