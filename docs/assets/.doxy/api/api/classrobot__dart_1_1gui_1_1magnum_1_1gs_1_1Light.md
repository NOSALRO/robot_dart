

# Class robot\_dart::gui::magnum::gs::Light



[**ClassList**](annotated.md) **>** [**robot\_dart**](namespacerobot__dart.md) **>** [**gui**](namespacerobot__dart_1_1gui.md) **>** [**magnum**](namespacerobot__dart_1_1gui_1_1magnum.md) **>** [**gs**](namespacerobot__dart_1_1gui_1_1magnum_1_1gs.md) **>** [**Light**](classrobot__dart_1_1gui_1_1magnum_1_1gs_1_1Light.md)










































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**Light**](#function-light-12) () <br> |
|   | [**Light**](#function-light-22) (const Magnum::Vector4 & position, const [**Material**](classrobot__dart_1_1gui_1_1magnum_1_1gs_1_1Material.md) & material, const Magnum::Vector3 & spot\_direction, Magnum::Float spot\_exponent, Magnum::Float spot\_cut\_off, const Magnum::Vector4 & attenuation, bool cast\_shadows=true) <br> |
|  Magnum::Vector4 & | [**attenuation**](#function-attenuation-12) () <br> |
|  Magnum::Vector4 | [**attenuation**](#function-attenuation-22) () const<br> |
|  bool | [**casts\_shadows**](#function-casts_shadows) () const<br> |
|  [**Material**](classrobot__dart_1_1gui_1_1magnum_1_1gs_1_1Material.md) & | [**material**](#function-material-12) () <br> |
|  [**Material**](classrobot__dart_1_1gui_1_1magnum_1_1gs_1_1Material.md) | [**material**](#function-material-22) () const<br> |
|  Magnum::Vector4 | [**position**](#function-position) () const<br> |
|  [**Light**](classrobot__dart_1_1gui_1_1magnum_1_1gs_1_1Light.md) & | [**set\_attenuation**](#function-set_attenuation) (const Magnum::Vector4 & attenuation) <br> |
|  [**Light**](classrobot__dart_1_1gui_1_1magnum_1_1gs_1_1Light.md) & | [**set\_casts\_shadows**](#function-set_casts_shadows) (bool cast\_shadows) <br> |
|  [**Light**](classrobot__dart_1_1gui_1_1magnum_1_1gs_1_1Light.md) & | [**set\_material**](#function-set_material) (const [**Material**](classrobot__dart_1_1gui_1_1magnum_1_1gs_1_1Material.md) & material) <br> |
|  [**Light**](classrobot__dart_1_1gui_1_1magnum_1_1gs_1_1Light.md) & | [**set\_position**](#function-set_position) (const Magnum::Vector4 & position) <br> |
|  [**Light**](classrobot__dart_1_1gui_1_1magnum_1_1gs_1_1Light.md) & | [**set\_shadow\_matrix**](#function-set_shadow_matrix) (const Magnum::Matrix4 & shadowTransform) <br> |
|  [**Light**](classrobot__dart_1_1gui_1_1magnum_1_1gs_1_1Light.md) & | [**set\_spot\_cut\_off**](#function-set_spot_cut_off) (Magnum::Float spot\_cut\_off) <br> |
|  [**Light**](classrobot__dart_1_1gui_1_1magnum_1_1gs_1_1Light.md) & | [**set\_spot\_direction**](#function-set_spot_direction) (const Magnum::Vector3 & spot\_direction) <br> |
|  [**Light**](classrobot__dart_1_1gui_1_1magnum_1_1gs_1_1Light.md) & | [**set\_spot\_exponent**](#function-set_spot_exponent) (Magnum::Float spot\_exponent) <br> |
|  [**Light**](classrobot__dart_1_1gui_1_1magnum_1_1gs_1_1Light.md) & | [**set\_transformed\_position**](#function-set_transformed_position) (const Magnum::Vector4 & transformed\_position) <br> |
|  [**Light**](classrobot__dart_1_1gui_1_1magnum_1_1gs_1_1Light.md) & | [**set\_transformed\_spot\_direction**](#function-set_transformed_spot_direction) (const Magnum::Vector3 & transformed\_spot\_direction) <br> |
|  Magnum::Matrix4 | [**shadow\_matrix**](#function-shadow_matrix) () const<br> |
|  Magnum::Float & | [**spot\_cut\_off**](#function-spot_cut_off-12) () <br> |
|  Magnum::Float | [**spot\_cut\_off**](#function-spot_cut_off-22) () const<br> |
|  Magnum::Vector3 | [**spot\_direction**](#function-spot_direction) () const<br> |
|  Magnum::Float & | [**spot\_exponent**](#function-spot_exponent-12) () <br> |
|  Magnum::Float | [**spot\_exponent**](#function-spot_exponent-22) () const<br> |
|  Magnum::Vector4 & | [**transformed\_position**](#function-transformed_position-12) () <br> |
|  Magnum::Vector4 | [**transformed\_position**](#function-transformed_position-22) () const<br> |
|  Magnum::Vector3 & | [**transformed\_spot\_direction**](#function-transformed_spot_direction-12) () <br> |
|  Magnum::Vector3 | [**transformed\_spot\_direction**](#function-transformed_spot_direction-22) () const<br> |








## Protected Attributes

| Type | Name |
| ---: | :--- |
|  Magnum::Vector4 | [**\_attenuation**](#variable-_attenuation)  <br> |
|  bool | [**\_cast\_shadows**](#variable-_cast_shadows)   = = true<br> |
|  [**Material**](classrobot__dart_1_1gui_1_1magnum_1_1gs_1_1Material.md) | [**\_material**](#variable-_material)  <br> |
|  Magnum::Vector4 | [**\_position**](#variable-_position)  <br> |
|  Magnum::Matrix4 | [**\_shadow\_transform**](#variable-_shadow_transform)   = {}<br> |
|  Magnum::Float | [**\_spot\_cut\_off**](#variable-_spot_cut_off)  <br> |
|  Magnum::Vector3 | [**\_spot\_direction**](#variable-_spot_direction)  <br> |
|  Magnum::Float | [**\_spot\_exponent**](#variable-_spot_exponent)  <br> |
|  Magnum::Vector4 | [**\_transformed\_position**](#variable-_transformed_position)  <br> |
|  Magnum::Vector3 | [**\_transformed\_spot\_direction**](#variable-_transformed_spot_direction)  <br> |




















## Public Functions Documentation




### function Light [1/2]

```C++
robot_dart::gui::magnum::gs::Light::Light () 
```




<hr>



### function Light [2/2]

```C++
robot_dart::gui::magnum::gs::Light::Light (
    const Magnum::Vector4 & position,
    const Material & material,
    const Magnum::Vector3 & spot_direction,
    Magnum::Float spot_exponent,
    Magnum::Float spot_cut_off,
    const Magnum::Vector4 & attenuation,
    bool cast_shadows=true
) 
```




<hr>



### function attenuation [1/2]

```C++
Magnum::Vector4 & robot_dart::gui::magnum::gs::Light::attenuation () 
```




<hr>



### function attenuation [2/2]

```C++
Magnum::Vector4 robot_dart::gui::magnum::gs::Light::attenuation () const
```




<hr>



### function casts\_shadows 

```C++
bool robot_dart::gui::magnum::gs::Light::casts_shadows () const
```




<hr>



### function material [1/2]

```C++
Material & robot_dart::gui::magnum::gs::Light::material () 
```




<hr>



### function material [2/2]

```C++
Material robot_dart::gui::magnum::gs::Light::material () const
```




<hr>



### function position 

```C++
Magnum::Vector4 robot_dart::gui::magnum::gs::Light::position () const
```




<hr>



### function set\_attenuation 

```C++
Light & robot_dart::gui::magnum::gs::Light::set_attenuation (
    const Magnum::Vector4 & attenuation
) 
```




<hr>



### function set\_casts\_shadows 

```C++
Light & robot_dart::gui::magnum::gs::Light::set_casts_shadows (
    bool cast_shadows
) 
```




<hr>



### function set\_material 

```C++
Light & robot_dart::gui::magnum::gs::Light::set_material (
    const Material & material
) 
```




<hr>



### function set\_position 

```C++
Light & robot_dart::gui::magnum::gs::Light::set_position (
    const Magnum::Vector4 & position
) 
```




<hr>



### function set\_shadow\_matrix 

```C++
Light & robot_dart::gui::magnum::gs::Light::set_shadow_matrix (
    const Magnum::Matrix4 & shadowTransform
) 
```




<hr>



### function set\_spot\_cut\_off 

```C++
Light & robot_dart::gui::magnum::gs::Light::set_spot_cut_off (
    Magnum::Float spot_cut_off
) 
```




<hr>



### function set\_spot\_direction 

```C++
Light & robot_dart::gui::magnum::gs::Light::set_spot_direction (
    const Magnum::Vector3 & spot_direction
) 
```




<hr>



### function set\_spot\_exponent 

```C++
Light & robot_dart::gui::magnum::gs::Light::set_spot_exponent (
    Magnum::Float spot_exponent
) 
```




<hr>



### function set\_transformed\_position 

```C++
Light & robot_dart::gui::magnum::gs::Light::set_transformed_position (
    const Magnum::Vector4 & transformed_position
) 
```




<hr>



### function set\_transformed\_spot\_direction 

```C++
Light & robot_dart::gui::magnum::gs::Light::set_transformed_spot_direction (
    const Magnum::Vector3 & transformed_spot_direction
) 
```




<hr>



### function shadow\_matrix 

```C++
Magnum::Matrix4 robot_dart::gui::magnum::gs::Light::shadow_matrix () const
```




<hr>



### function spot\_cut\_off [1/2]

```C++
Magnum::Float & robot_dart::gui::magnum::gs::Light::spot_cut_off () 
```




<hr>



### function spot\_cut\_off [2/2]

```C++
Magnum::Float robot_dart::gui::magnum::gs::Light::spot_cut_off () const
```




<hr>



### function spot\_direction 

```C++
Magnum::Vector3 robot_dart::gui::magnum::gs::Light::spot_direction () const
```




<hr>



### function spot\_exponent [1/2]

```C++
Magnum::Float & robot_dart::gui::magnum::gs::Light::spot_exponent () 
```




<hr>



### function spot\_exponent [2/2]

```C++
Magnum::Float robot_dart::gui::magnum::gs::Light::spot_exponent () const
```




<hr>



### function transformed\_position [1/2]

```C++
Magnum::Vector4 & robot_dart::gui::magnum::gs::Light::transformed_position () 
```




<hr>



### function transformed\_position [2/2]

```C++
Magnum::Vector4 robot_dart::gui::magnum::gs::Light::transformed_position () const
```




<hr>



### function transformed\_spot\_direction [1/2]

```C++
Magnum::Vector3 & robot_dart::gui::magnum::gs::Light::transformed_spot_direction () 
```




<hr>



### function transformed\_spot\_direction [2/2]

```C++
Magnum::Vector3 robot_dart::gui::magnum::gs::Light::transformed_spot_direction () const
```




<hr>
## Protected Attributes Documentation




### variable \_attenuation 

```C++
Magnum::Vector4 robot_dart::gui::magnum::gs::Light::_attenuation;
```




<hr>



### variable \_cast\_shadows 

```C++
bool robot_dart::gui::magnum::gs::Light::_cast_shadows;
```




<hr>



### variable \_material 

```C++
Material robot_dart::gui::magnum::gs::Light::_material;
```




<hr>



### variable \_position 

```C++
Magnum::Vector4 robot_dart::gui::magnum::gs::Light::_position;
```




<hr>



### variable \_shadow\_transform 

```C++
Magnum::Matrix4 robot_dart::gui::magnum::gs::Light::_shadow_transform;
```




<hr>



### variable \_spot\_cut\_off 

```C++
Magnum::Float robot_dart::gui::magnum::gs::Light::_spot_cut_off;
```




<hr>



### variable \_spot\_direction 

```C++
Magnum::Vector3 robot_dart::gui::magnum::gs::Light::_spot_direction;
```




<hr>



### variable \_spot\_exponent 

```C++
Magnum::Float robot_dart::gui::magnum::gs::Light::_spot_exponent;
```




<hr>



### variable \_transformed\_position 

```C++
Magnum::Vector4 robot_dart::gui::magnum::gs::Light::_transformed_position;
```




<hr>



### variable \_transformed\_spot\_direction 

```C++
Magnum::Vector3 robot_dart::gui::magnum::gs::Light::_transformed_spot_direction;
```




<hr>

------------------------------
The documentation for this class was generated from the following file `robot_dart/gui/magnum/gs/light.hpp`

