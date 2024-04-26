

# Class robot\_dart::gui::magnum::gs::PhongMultiLight



[**ClassList**](annotated.md) **>** [**robot\_dart**](namespacerobot__dart.md) **>** [**gui**](namespacerobot__dart_1_1gui.md) **>** [**magnum**](namespacerobot__dart_1_1gui_1_1magnum.md) **>** [**gs**](namespacerobot__dart_1_1gui_1_1magnum_1_1gs.md) **>** [**PhongMultiLight**](classrobot__dart_1_1gui_1_1magnum_1_1gs_1_1PhongMultiLight.md)








Inherits the following classes: Magnum::GL::AbstractShaderProgram














## Public Types

| Type | Name |
| ---: | :--- |
| enum Magnum::UnsignedByte | [**Flag**](#enum-flag)  <br> |
| typedef Magnum::Containers::EnumSet&lt; [**Flag**](classrobot__dart_1_1gui_1_1magnum_1_1gs_1_1PhongMultiLight.md#enum-flag) &gt; | [**Flags**](#typedef-flags)  <br> |
| typedef Magnum::Shaders::Generic3D::Normal | [**Normal**](#typedef-normal)  <br> |
| typedef Magnum::Shaders::Generic3D::Position | [**Position**](#typedef-position)  <br> |
| typedef Magnum::Shaders::Generic3D::TextureCoordinates | [**TextureCoordinates**](#typedef-texturecoordinates)  <br> |




















## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**PhongMultiLight**](#function-phongmultilight-12) (Flags flags={}, Magnum::Int max\_lights=10) <br> |
|   | [**PhongMultiLight**](#function-phongmultilight-22) (Magnum::NoCreateT) noexcept<br> |
|  [**PhongMultiLight**](classrobot__dart_1_1gui_1_1magnum_1_1gs_1_1PhongMultiLight.md) & | [**bind\_cube\_map\_color\_texture**](#function-bind_cube_map_color_texture) (Magnum::GL::CubeMapTextureArray & texture) <br> |
|  [**PhongMultiLight**](classrobot__dart_1_1gui_1_1magnum_1_1gs_1_1PhongMultiLight.md) & | [**bind\_cube\_map\_texture**](#function-bind_cube_map_texture) (Magnum::GL::CubeMapTextureArray & texture) <br> |
|  [**PhongMultiLight**](classrobot__dart_1_1gui_1_1magnum_1_1gs_1_1PhongMultiLight.md) & | [**bind\_shadow\_color\_texture**](#function-bind_shadow_color_texture) (Magnum::GL::Texture2DArray & texture) <br> |
|  [**PhongMultiLight**](classrobot__dart_1_1gui_1_1magnum_1_1gs_1_1PhongMultiLight.md) & | [**bind\_shadow\_texture**](#function-bind_shadow_texture) (Magnum::GL::Texture2DArray & texture) <br> |
|  Flags | [**flags**](#function-flags) () const<br> |
|  Magnum::Int | [**max\_lights**](#function-max_lights) () const<br> |
|  [**PhongMultiLight**](classrobot__dart_1_1gui_1_1magnum_1_1gs_1_1PhongMultiLight.md) & | [**set\_camera\_matrix**](#function-set_camera_matrix) (const Magnum::Matrix4 & matrix) <br> |
|  [**PhongMultiLight**](classrobot__dart_1_1gui_1_1magnum_1_1gs_1_1PhongMultiLight.md) & | [**set\_far\_plane**](#function-set_far_plane) (Magnum::Float far\_plane) <br> |
|  [**PhongMultiLight**](classrobot__dart_1_1gui_1_1magnum_1_1gs_1_1PhongMultiLight.md) & | [**set\_is\_shadowed**](#function-set_is_shadowed) (bool shadows) <br> |
|  [**PhongMultiLight**](classrobot__dart_1_1gui_1_1magnum_1_1gs_1_1PhongMultiLight.md) & | [**set\_light**](#function-set_light) (Magnum::Int i, const [**Light**](classrobot__dart_1_1gui_1_1magnum_1_1gs_1_1Light.md) & light) <br> |
|  [**PhongMultiLight**](classrobot__dart_1_1gui_1_1magnum_1_1gs_1_1PhongMultiLight.md) & | [**set\_material**](#function-set_material) ([**Material**](classrobot__dart_1_1gui_1_1magnum_1_1gs_1_1Material.md) & material) <br> |
|  [**PhongMultiLight**](classrobot__dart_1_1gui_1_1magnum_1_1gs_1_1PhongMultiLight.md) & | [**set\_normal\_matrix**](#function-set_normal_matrix) (const Magnum::Matrix3x3 & matrix) <br> |
|  [**PhongMultiLight**](classrobot__dart_1_1gui_1_1magnum_1_1gs_1_1PhongMultiLight.md) & | [**set\_projection\_matrix**](#function-set_projection_matrix) (const Magnum::Matrix4 & matrix) <br> |
|  [**PhongMultiLight**](classrobot__dart_1_1gui_1_1magnum_1_1gs_1_1PhongMultiLight.md) & | [**set\_specular\_strength**](#function-set_specular_strength) (Magnum::Float specular\_strength) <br> |
|  [**PhongMultiLight**](classrobot__dart_1_1gui_1_1magnum_1_1gs_1_1PhongMultiLight.md) & | [**set\_transformation\_matrix**](#function-set_transformation_matrix) (const Magnum::Matrix4 & matrix) <br> |
|  [**PhongMultiLight**](classrobot__dart_1_1gui_1_1magnum_1_1gs_1_1PhongMultiLight.md) & | [**set\_transparent\_shadows**](#function-set_transparent_shadows) (bool shadows) <br> |




























## Public Types Documentation




### enum Flag 

```C++
enum robot_dart::gui::magnum::gs::PhongMultiLight::Flag {
    AmbientTexture = 1 << 0,
    DiffuseTexture = 1 << 1,
    SpecularTexture = 1 << 2
};
```




<hr>



### typedef Flags 

```C++
using robot_dart::gui::magnum::gs::PhongMultiLight::Flags =  Magnum::Containers::EnumSet<Flag>;
```




<hr>



### typedef Normal 

```C++
using robot_dart::gui::magnum::gs::PhongMultiLight::Normal =  Magnum::Shaders::Generic3D::Normal;
```




<hr>



### typedef Position 

```C++
using robot_dart::gui::magnum::gs::PhongMultiLight::Position =  Magnum::Shaders::Generic3D::Position;
```




<hr>



### typedef TextureCoordinates 

```C++
using robot_dart::gui::magnum::gs::PhongMultiLight::TextureCoordinates =  Magnum::Shaders::Generic3D::TextureCoordinates;
```




<hr>
## Public Functions Documentation




### function PhongMultiLight [1/2]

```C++
explicit robot_dart::gui::magnum::gs::PhongMultiLight::PhongMultiLight (
    Flags flags={},
    Magnum::Int max_lights=10
) 
```




<hr>



### function PhongMultiLight [2/2]

```C++
explicit robot_dart::gui::magnum::gs::PhongMultiLight::PhongMultiLight (
    Magnum::NoCreateT
) noexcept
```




<hr>



### function bind\_cube\_map\_color\_texture 

```C++
PhongMultiLight & robot_dart::gui::magnum::gs::PhongMultiLight::bind_cube_map_color_texture (
    Magnum::GL::CubeMapTextureArray & texture
) 
```




<hr>



### function bind\_cube\_map\_texture 

```C++
PhongMultiLight & robot_dart::gui::magnum::gs::PhongMultiLight::bind_cube_map_texture (
    Magnum::GL::CubeMapTextureArray & texture
) 
```




<hr>



### function bind\_shadow\_color\_texture 

```C++
PhongMultiLight & robot_dart::gui::magnum::gs::PhongMultiLight::bind_shadow_color_texture (
    Magnum::GL::Texture2DArray & texture
) 
```




<hr>



### function bind\_shadow\_texture 

```C++
PhongMultiLight & robot_dart::gui::magnum::gs::PhongMultiLight::bind_shadow_texture (
    Magnum::GL::Texture2DArray & texture
) 
```




<hr>



### function flags 

```C++
Flags robot_dart::gui::magnum::gs::PhongMultiLight::flags () const
```




<hr>



### function max\_lights 

```C++
Magnum::Int robot_dart::gui::magnum::gs::PhongMultiLight::max_lights () const
```




<hr>



### function set\_camera\_matrix 

```C++
PhongMultiLight & robot_dart::gui::magnum::gs::PhongMultiLight::set_camera_matrix (
    const Magnum::Matrix4 & matrix
) 
```




<hr>



### function set\_far\_plane 

```C++
PhongMultiLight & robot_dart::gui::magnum::gs::PhongMultiLight::set_far_plane (
    Magnum::Float far_plane
) 
```




<hr>



### function set\_is\_shadowed 

```C++
PhongMultiLight & robot_dart::gui::magnum::gs::PhongMultiLight::set_is_shadowed (
    bool shadows
) 
```




<hr>



### function set\_light 

```C++
PhongMultiLight & robot_dart::gui::magnum::gs::PhongMultiLight::set_light (
    Magnum::Int i,
    const Light & light
) 
```




<hr>



### function set\_material 

```C++
PhongMultiLight & robot_dart::gui::magnum::gs::PhongMultiLight::set_material (
    Material & material
) 
```




<hr>



### function set\_normal\_matrix 

```C++
PhongMultiLight & robot_dart::gui::magnum::gs::PhongMultiLight::set_normal_matrix (
    const Magnum::Matrix3x3 & matrix
) 
```




<hr>



### function set\_projection\_matrix 

```C++
PhongMultiLight & robot_dart::gui::magnum::gs::PhongMultiLight::set_projection_matrix (
    const Magnum::Matrix4 & matrix
) 
```




<hr>



### function set\_specular\_strength 

```C++
PhongMultiLight & robot_dart::gui::magnum::gs::PhongMultiLight::set_specular_strength (
    Magnum::Float specular_strength
) 
```




<hr>



### function set\_transformation\_matrix 

```C++
PhongMultiLight & robot_dart::gui::magnum::gs::PhongMultiLight::set_transformation_matrix (
    const Magnum::Matrix4 & matrix
) 
```




<hr>



### function set\_transparent\_shadows 

```C++
PhongMultiLight & robot_dart::gui::magnum::gs::PhongMultiLight::set_transparent_shadows (
    bool shadows
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `robot_dart/gui/magnum/gs/phong_multi_light.hpp`

