

# Class robot\_dart::gui::magnum::gs::CubeMapColor



[**ClassList**](annotated.md) **>** [**robot\_dart**](namespacerobot__dart.md) **>** [**gui**](namespacerobot__dart_1_1gui.md) **>** [**magnum**](namespacerobot__dart_1_1gui_1_1magnum.md) **>** [**gs**](namespacerobot__dart_1_1gui_1_1magnum_1_1gs.md) **>** [**CubeMapColor**](classrobot__dart_1_1gui_1_1magnum_1_1gs_1_1CubeMapColor.md)








Inherits the following classes: Magnum::GL::AbstractShaderProgram














## Public Types

| Type | Name |
| ---: | :--- |
| enum Magnum::UnsignedByte | [**Flag**](#enum-flag)  <br> |
| typedef Magnum::Containers::EnumSet&lt; [**Flag**](classrobot__dart_1_1gui_1_1magnum_1_1gs_1_1CubeMapColor.md#enum-flag) &gt; | [**Flags**](#typedef-flags)  <br> |
| typedef Magnum::Shaders::Generic3D::Position | [**Position**](#typedef-position)  <br> |
| typedef Magnum::Shaders::Generic3D::TextureCoordinates | [**TextureCoordinates**](#typedef-texturecoordinates)  <br> |




















## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**CubeMapColor**](#function-cubemapcolor-12) (Flags flags={}) <br> |
|   | [**CubeMapColor**](#function-cubemapcolor-22) (Magnum::NoCreateT) noexcept<br> |
|  [**CubeMapColor**](classrobot__dart_1_1gui_1_1magnum_1_1gs_1_1CubeMapColor.md) & | [**bind\_cube\_map\_texture**](#function-bind_cube_map_texture) (Magnum::GL::CubeMapTextureArray & texture) <br> |
|  Flags | [**flags**](#function-flags) () const<br> |
|  [**CubeMapColor**](classrobot__dart_1_1gui_1_1magnum_1_1gs_1_1CubeMapColor.md) & | [**set\_far\_plane**](#function-set_far_plane) (Magnum::Float far\_plane) <br> |
|  [**CubeMapColor**](classrobot__dart_1_1gui_1_1magnum_1_1gs_1_1CubeMapColor.md) & | [**set\_light\_index**](#function-set_light_index) (Magnum::Int index) <br> |
|  [**CubeMapColor**](classrobot__dart_1_1gui_1_1magnum_1_1gs_1_1CubeMapColor.md) & | [**set\_light\_position**](#function-set_light_position) (const Magnum::Vector3 & position) <br> |
|  [**CubeMapColor**](classrobot__dart_1_1gui_1_1magnum_1_1gs_1_1CubeMapColor.md) & | [**set\_material**](#function-set_material) ([**Material**](classrobot__dart_1_1gui_1_1magnum_1_1gs_1_1Material.md) & material) <br> |
|  [**CubeMapColor**](classrobot__dart_1_1gui_1_1magnum_1_1gs_1_1CubeMapColor.md) & | [**set\_shadow\_matrices**](#function-set_shadow_matrices) (Magnum::Matrix4 matrices) <br> |
|  [**CubeMapColor**](classrobot__dart_1_1gui_1_1magnum_1_1gs_1_1CubeMapColor.md) & | [**set\_transformation\_matrix**](#function-set_transformation_matrix) (const Magnum::Matrix4 & matrix) <br> |




























## Public Types Documentation




### enum Flag 

```C++
enum robot_dart::gui::magnum::gs::CubeMapColor::Flag {
    DiffuseTexture = 1 << 0
};
```






### typedef Flags 

```C++
using robot_dart::gui::magnum::gs::CubeMapColor::Flags =  Magnum::Containers::EnumSet<Flag>;
```






### typedef Position 

```C++
using robot_dart::gui::magnum::gs::CubeMapColor::Position =  Magnum::Shaders::Generic3D::Position;
```






### typedef TextureCoordinates 

```C++
using robot_dart::gui::magnum::gs::CubeMapColor::TextureCoordinates =  Magnum::Shaders::Generic3D::TextureCoordinates;
```



## Public Functions Documentation




### function CubeMapColor [1/2]

```C++
explicit robot_dart::gui::magnum::gs::CubeMapColor::CubeMapColor (
    Flags flags={}
) 
```






### function CubeMapColor [2/2]

```C++
explicit robot_dart::gui::magnum::gs::CubeMapColor::CubeMapColor (
    Magnum::NoCreateT
) noexcept
```






### function bind\_cube\_map\_texture 

```C++
CubeMapColor & robot_dart::gui::magnum::gs::CubeMapColor::bind_cube_map_texture (
    Magnum::GL::CubeMapTextureArray & texture
) 
```






### function flags 

```C++
Flags robot_dart::gui::magnum::gs::CubeMapColor::flags () const
```






### function set\_far\_plane 

```C++
CubeMapColor & robot_dart::gui::magnum::gs::CubeMapColor::set_far_plane (
    Magnum::Float far_plane
) 
```






### function set\_light\_index 

```C++
CubeMapColor & robot_dart::gui::magnum::gs::CubeMapColor::set_light_index (
    Magnum::Int index
) 
```






### function set\_light\_position 

```C++
CubeMapColor & robot_dart::gui::magnum::gs::CubeMapColor::set_light_position (
    const Magnum::Vector3 & position
) 
```






### function set\_material 

```C++
CubeMapColor & robot_dart::gui::magnum::gs::CubeMapColor::set_material (
    Material & material
) 
```






### function set\_shadow\_matrices 

```C++
CubeMapColor & robot_dart::gui::magnum::gs::CubeMapColor::set_shadow_matrices (
    Magnum::Matrix4 matrices
) 
```






### function set\_transformation\_matrix 

```C++
CubeMapColor & robot_dart::gui::magnum::gs::CubeMapColor::set_transformation_matrix (
    const Magnum::Matrix4 & matrix
) 
```




------------------------------
The documentation for this class was generated from the following file `robot_dart/gui/magnum/gs/cube_map_color.hpp`

