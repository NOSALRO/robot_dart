

# Class robot\_dart::gui::magnum::gs::ShadowMapColor



[**ClassList**](annotated.md) **>** [**robot\_dart**](namespacerobot__dart.md) **>** [**gui**](namespacerobot__dart_1_1gui.md) **>** [**magnum**](namespacerobot__dart_1_1gui_1_1magnum.md) **>** [**gs**](namespacerobot__dart_1_1gui_1_1magnum_1_1gs.md) **>** [**ShadowMapColor**](classrobot__dart_1_1gui_1_1magnum_1_1gs_1_1ShadowMapColor.md)








Inherits the following classes: Magnum::GL::AbstractShaderProgram














## Public Types

| Type | Name |
| ---: | :--- |
| enum Magnum::UnsignedByte | [**Flag**](#enum-flag)  <br> |
| typedef Magnum::Containers::EnumSet&lt; [**Flag**](classrobot__dart_1_1gui_1_1magnum_1_1gs_1_1ShadowMapColor.md#enum-flag) &gt; | [**Flags**](#typedef-flags)  <br> |
| typedef Magnum::Shaders::Generic3D::Position | [**Position**](#typedef-position)  <br> |
| typedef Magnum::Shaders::Generic3D::TextureCoordinates | [**TextureCoordinates**](#typedef-texturecoordinates)  <br> |




















## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**ShadowMapColor**](#function-shadowmapcolor-12) (Flags flags={}) <br> |
|   | [**ShadowMapColor**](#function-shadowmapcolor-22) (Magnum::NoCreateT) noexcept<br> |
|  Flags | [**flags**](#function-flags) () const<br> |
|  [**ShadowMapColor**](classrobot__dart_1_1gui_1_1magnum_1_1gs_1_1ShadowMapColor.md) & | [**set\_material**](#function-set_material) ([**Material**](classrobot__dart_1_1gui_1_1magnum_1_1gs_1_1Material.md) & material) <br> |
|  [**ShadowMapColor**](classrobot__dart_1_1gui_1_1magnum_1_1gs_1_1ShadowMapColor.md) & | [**set\_projection\_matrix**](#function-set_projection_matrix) (const Magnum::Matrix4 & matrix) <br> |
|  [**ShadowMapColor**](classrobot__dart_1_1gui_1_1magnum_1_1gs_1_1ShadowMapColor.md) & | [**set\_transformation\_matrix**](#function-set_transformation_matrix) (const Magnum::Matrix4 & matrix) <br> |




























## Public Types Documentation




### enum Flag 

```C++
enum robot_dart::gui::magnum::gs::ShadowMapColor::Flag {
    DiffuseTexture = 1 << 0
};
```






### typedef Flags 

```C++
using robot_dart::gui::magnum::gs::ShadowMapColor::Flags =  Magnum::Containers::EnumSet<Flag>;
```






### typedef Position 

```C++
using robot_dart::gui::magnum::gs::ShadowMapColor::Position =  Magnum::Shaders::Generic3D::Position;
```






### typedef TextureCoordinates 

```C++
using robot_dart::gui::magnum::gs::ShadowMapColor::TextureCoordinates =  Magnum::Shaders::Generic3D::TextureCoordinates;
```



## Public Functions Documentation




### function ShadowMapColor [1/2]

```C++
explicit robot_dart::gui::magnum::gs::ShadowMapColor::ShadowMapColor (
    Flags flags={}
) 
```






### function ShadowMapColor [2/2]

```C++
explicit robot_dart::gui::magnum::gs::ShadowMapColor::ShadowMapColor (
    Magnum::NoCreateT
) noexcept
```






### function flags 

```C++
Flags robot_dart::gui::magnum::gs::ShadowMapColor::flags () const
```






### function set\_material 

```C++
ShadowMapColor & robot_dart::gui::magnum::gs::ShadowMapColor::set_material (
    Material & material
) 
```






### function set\_projection\_matrix 

```C++
ShadowMapColor & robot_dart::gui::magnum::gs::ShadowMapColor::set_projection_matrix (
    const Magnum::Matrix4 & matrix
) 
```






### function set\_transformation\_matrix 

```C++
ShadowMapColor & robot_dart::gui::magnum::gs::ShadowMapColor::set_transformation_matrix (
    const Magnum::Matrix4 & matrix
) 
```




------------------------------
The documentation for this class was generated from the following file `robot_dart/gui/magnum/gs/shadow_map_color.hpp`

