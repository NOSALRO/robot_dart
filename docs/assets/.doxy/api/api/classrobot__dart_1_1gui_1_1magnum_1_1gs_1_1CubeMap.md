

# Class robot\_dart::gui::magnum::gs::CubeMap



[**ClassList**](annotated.md) **>** [**robot\_dart**](namespacerobot__dart.md) **>** [**gui**](namespacerobot__dart_1_1gui.md) **>** [**magnum**](namespacerobot__dart_1_1gui_1_1magnum.md) **>** [**gs**](namespacerobot__dart_1_1gui_1_1magnum_1_1gs.md) **>** [**CubeMap**](classrobot__dart_1_1gui_1_1magnum_1_1gs_1_1CubeMap.md)








Inherits the following classes: Magnum::GL::AbstractShaderProgram














## Public Types

| Type | Name |
| ---: | :--- |
| enum Magnum::UnsignedByte | [**Flag**](#enum-flag)  <br> |
| typedef Magnum::Containers::EnumSet&lt; [**Flag**](classrobot__dart_1_1gui_1_1magnum_1_1gs_1_1CubeMap.md#enum-flag) &gt; | [**Flags**](#typedef-flags)  <br> |
| typedef Magnum::Shaders::Generic3D::Position | [**Position**](#typedef-position)  <br> |
| typedef Magnum::Shaders::Generic3D::TextureCoordinates | [**TextureCoordinates**](#typedef-texturecoordinates)  <br> |




















## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**CubeMap**](#function-cubemap-12) (Flags flags={}) <br> |
|   | [**CubeMap**](#function-cubemap-22) (Magnum::NoCreateT) noexcept<br> |
|  Flags | [**flags**](#function-flags) () const<br> |
|  [**CubeMap**](classrobot__dart_1_1gui_1_1magnum_1_1gs_1_1CubeMap.md) & | [**set\_far\_plane**](#function-set_far_plane) (Magnum::Float far\_plane) <br> |
|  [**CubeMap**](classrobot__dart_1_1gui_1_1magnum_1_1gs_1_1CubeMap.md) & | [**set\_light\_index**](#function-set_light_index) (Magnum::Int index) <br> |
|  [**CubeMap**](classrobot__dart_1_1gui_1_1magnum_1_1gs_1_1CubeMap.md) & | [**set\_light\_position**](#function-set_light_position) (const Magnum::Vector3 & position) <br> |
|  [**CubeMap**](classrobot__dart_1_1gui_1_1magnum_1_1gs_1_1CubeMap.md) & | [**set\_material**](#function-set_material) ([**Material**](classrobot__dart_1_1gui_1_1magnum_1_1gs_1_1Material.md) & material) <br> |
|  [**CubeMap**](classrobot__dart_1_1gui_1_1magnum_1_1gs_1_1CubeMap.md) & | [**set\_shadow\_matrices**](#function-set_shadow_matrices) (Magnum::Matrix4 matrices) <br> |
|  [**CubeMap**](classrobot__dart_1_1gui_1_1magnum_1_1gs_1_1CubeMap.md) & | [**set\_transformation\_matrix**](#function-set_transformation_matrix) (const Magnum::Matrix4 & matrix) <br> |




























## Public Types Documentation




### enum Flag 

```C++
enum robot_dart::gui::magnum::gs::CubeMap::Flag {
    DiffuseTexture = 1 << 0
};
```






### typedef Flags 

```C++
using robot_dart::gui::magnum::gs::CubeMap::Flags =  Magnum::Containers::EnumSet<Flag>;
```






### typedef Position 

```C++
using robot_dart::gui::magnum::gs::CubeMap::Position =  Magnum::Shaders::Generic3D::Position;
```






### typedef TextureCoordinates 

```C++
using robot_dart::gui::magnum::gs::CubeMap::TextureCoordinates =  Magnum::Shaders::Generic3D::TextureCoordinates;
```



## Public Functions Documentation




### function CubeMap [1/2]

```C++
explicit robot_dart::gui::magnum::gs::CubeMap::CubeMap (
    Flags flags={}
) 
```






### function CubeMap [2/2]

```C++
explicit robot_dart::gui::magnum::gs::CubeMap::CubeMap (
    Magnum::NoCreateT
) noexcept
```






### function flags 

```C++
Flags robot_dart::gui::magnum::gs::CubeMap::flags () const
```






### function set\_far\_plane 

```C++
CubeMap & robot_dart::gui::magnum::gs::CubeMap::set_far_plane (
    Magnum::Float far_plane
) 
```






### function set\_light\_index 

```C++
CubeMap & robot_dart::gui::magnum::gs::CubeMap::set_light_index (
    Magnum::Int index
) 
```






### function set\_light\_position 

```C++
CubeMap & robot_dart::gui::magnum::gs::CubeMap::set_light_position (
    const Magnum::Vector3 & position
) 
```






### function set\_material 

```C++
CubeMap & robot_dart::gui::magnum::gs::CubeMap::set_material (
    Material & material
) 
```






### function set\_shadow\_matrices 

```C++
CubeMap & robot_dart::gui::magnum::gs::CubeMap::set_shadow_matrices (
    Magnum::Matrix4 matrices
) 
```






### function set\_transformation\_matrix 

```C++
CubeMap & robot_dart::gui::magnum::gs::CubeMap::set_transformation_matrix (
    const Magnum::Matrix4 & matrix
) 
```




------------------------------
The documentation for this class was generated from the following file `robot_dart/gui/magnum/gs/cube_map.hpp`

