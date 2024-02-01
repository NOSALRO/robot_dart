

# Class robot\_dart::gui::magnum::gs::ShadowMap



[**ClassList**](annotated.md) **>** [**robot\_dart**](namespacerobot__dart.md) **>** [**gui**](namespacerobot__dart_1_1gui.md) **>** [**magnum**](namespacerobot__dart_1_1gui_1_1magnum.md) **>** [**gs**](namespacerobot__dart_1_1gui_1_1magnum_1_1gs.md) **>** [**ShadowMap**](classrobot__dart_1_1gui_1_1magnum_1_1gs_1_1ShadowMap.md)








Inherits the following classes: Magnum::GL::AbstractShaderProgram














## Public Types

| Type | Name |
| ---: | :--- |
| enum Magnum::UnsignedByte | [**Flag**](#enum-flag)  <br> |
| typedef Magnum::Containers::EnumSet&lt; [**Flag**](classrobot__dart_1_1gui_1_1magnum_1_1gs_1_1ShadowMap.md#enum-flag) &gt; | [**Flags**](#typedef-flags)  <br> |
| typedef Magnum::Shaders::Generic3D::Position | [**Position**](#typedef-position)  <br> |
| typedef Magnum::Shaders::Generic3D::TextureCoordinates | [**TextureCoordinates**](#typedef-texturecoordinates)  <br> |




















## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**ShadowMap**](#function-shadowmap-12) (Flags flags={}) <br> |
|   | [**ShadowMap**](#function-shadowmap-22) (Magnum::NoCreateT) noexcept<br> |
|  Flags | [**flags**](#function-flags) () const<br> |
|  [**ShadowMap**](classrobot__dart_1_1gui_1_1magnum_1_1gs_1_1ShadowMap.md) & | [**set\_material**](#function-set_material) ([**Material**](classrobot__dart_1_1gui_1_1magnum_1_1gs_1_1Material.md) & material) <br> |
|  [**ShadowMap**](classrobot__dart_1_1gui_1_1magnum_1_1gs_1_1ShadowMap.md) & | [**set\_projection\_matrix**](#function-set_projection_matrix) (const Magnum::Matrix4 & matrix) <br> |
|  [**ShadowMap**](classrobot__dart_1_1gui_1_1magnum_1_1gs_1_1ShadowMap.md) & | [**set\_transformation\_matrix**](#function-set_transformation_matrix) (const Magnum::Matrix4 & matrix) <br> |




























## Public Types Documentation




### enum Flag 

```C++
enum robot_dart::gui::magnum::gs::ShadowMap::Flag {
    DiffuseTexture = 1 << 0
};
```






### typedef Flags 

```C++
using robot_dart::gui::magnum::gs::ShadowMap::Flags =  Magnum::Containers::EnumSet<Flag>;
```






### typedef Position 

```C++
using robot_dart::gui::magnum::gs::ShadowMap::Position =  Magnum::Shaders::Generic3D::Position;
```






### typedef TextureCoordinates 

```C++
using robot_dart::gui::magnum::gs::ShadowMap::TextureCoordinates =  Magnum::Shaders::Generic3D::TextureCoordinates;
```



## Public Functions Documentation




### function ShadowMap [1/2]

```C++
explicit robot_dart::gui::magnum::gs::ShadowMap::ShadowMap (
    Flags flags={}
) 
```






### function ShadowMap [2/2]

```C++
explicit robot_dart::gui::magnum::gs::ShadowMap::ShadowMap (
    Magnum::NoCreateT
) noexcept
```






### function flags 

```C++
Flags robot_dart::gui::magnum::gs::ShadowMap::flags () const
```






### function set\_material 

```C++
ShadowMap & robot_dart::gui::magnum::gs::ShadowMap::set_material (
    Material & material
) 
```






### function set\_projection\_matrix 

```C++
ShadowMap & robot_dart::gui::magnum::gs::ShadowMap::set_projection_matrix (
    const Magnum::Matrix4 & matrix
) 
```






### function set\_transformation\_matrix 

```C++
ShadowMap & robot_dart::gui::magnum::gs::ShadowMap::set_transformation_matrix (
    const Magnum::Matrix4 & matrix
) 
```




------------------------------
The documentation for this class was generated from the following file `robot_dart/gui/magnum/gs/shadow_map.hpp`

