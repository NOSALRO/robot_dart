

# Class robot\_dart::gui::magnum::gs::Material



[**ClassList**](annotated.md) **>** [**robot\_dart**](namespacerobot__dart.md) **>** [**gui**](namespacerobot__dart_1_1gui.md) **>** [**magnum**](namespacerobot__dart_1_1gui_1_1magnum.md) **>** [**gs**](namespacerobot__dart_1_1gui_1_1magnum_1_1gs.md) **>** [**Material**](classrobot__dart_1_1gui_1_1magnum_1_1gs_1_1Material.md)










































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**Material**](#function-material-13) () <br> |
|   | [**Material**](#function-material-23) (const Magnum::Color4 & ambient, const Magnum::Color4 & diffuse, const Magnum::Color4 & specular, Magnum::Float shininess) <br> |
|   | [**Material**](#function-material-33) (Magnum::GL::Texture2D \* ambient\_texture, Magnum::GL::Texture2D \* diffuse\_texture, Magnum::GL::Texture2D \* specular\_texture, Magnum::Float shininess) <br> |
|  Magnum::Color4 & | [**ambient\_color**](#function-ambient_color-12) () <br> |
|  Magnum::Color4 | [**ambient\_color**](#function-ambient_color-22) () const<br> |
|  Magnum::GL::Texture2D \* | [**ambient\_texture**](#function-ambient_texture) () <br> |
|  Magnum::Color4 & | [**diffuse\_color**](#function-diffuse_color-12) () <br> |
|  Magnum::Color4 | [**diffuse\_color**](#function-diffuse_color-22) () const<br> |
|  Magnum::GL::Texture2D \* | [**diffuse\_texture**](#function-diffuse_texture) () <br> |
|  bool | [**has\_ambient\_texture**](#function-has_ambient_texture) () const<br> |
|  bool | [**has\_diffuse\_texture**](#function-has_diffuse_texture) () const<br> |
|  bool | [**has\_specular\_texture**](#function-has_specular_texture) () const<br> |
|  [**Material**](classrobot__dart_1_1gui_1_1magnum_1_1gs_1_1Material.md) & | [**set\_ambient\_color**](#function-set_ambient_color) (const Magnum::Color4 & ambient) <br> |
|  [**Material**](classrobot__dart_1_1gui_1_1magnum_1_1gs_1_1Material.md) & | [**set\_ambient\_texture**](#function-set_ambient_texture) (Magnum::GL::Texture2D \* ambient\_texture) <br> |
|  [**Material**](classrobot__dart_1_1gui_1_1magnum_1_1gs_1_1Material.md) & | [**set\_diffuse\_color**](#function-set_diffuse_color) (const Magnum::Color4 & diffuse) <br> |
|  [**Material**](classrobot__dart_1_1gui_1_1magnum_1_1gs_1_1Material.md) & | [**set\_diffuse\_texture**](#function-set_diffuse_texture) (Magnum::GL::Texture2D \* diffuse\_texture) <br> |
|  [**Material**](classrobot__dart_1_1gui_1_1magnum_1_1gs_1_1Material.md) & | [**set\_shininess**](#function-set_shininess) (Magnum::Float shininess) <br> |
|  [**Material**](classrobot__dart_1_1gui_1_1magnum_1_1gs_1_1Material.md) & | [**set\_specular\_color**](#function-set_specular_color) (const Magnum::Color4 & specular) <br> |
|  [**Material**](classrobot__dart_1_1gui_1_1magnum_1_1gs_1_1Material.md) & | [**set\_specular\_texture**](#function-set_specular_texture) (Magnum::GL::Texture2D \* specular\_texture) <br> |
|  Magnum::Float & | [**shininess**](#function-shininess-12) () <br> |
|  Magnum::Float | [**shininess**](#function-shininess-22) () const<br> |
|  Magnum::Color4 & | [**specular\_color**](#function-specular_color-12) () <br> |
|  Magnum::Color4 | [**specular\_color**](#function-specular_color-22) () const<br> |
|  Magnum::GL::Texture2D \* | [**specular\_texture**](#function-specular_texture) () <br> |








## Protected Attributes

| Type | Name |
| ---: | :--- |
|  Magnum::Color4 | [**\_ambient**](#variable-_ambient)  <br> |
|  Magnum::GL::Texture2D \* | [**\_ambient\_texture**](#variable-_ambient_texture)   = = NULL<br> |
|  Magnum::Color4 | [**\_diffuse**](#variable-_diffuse)  <br> |
|  Magnum::GL::Texture2D \* | [**\_diffuse\_texture**](#variable-_diffuse_texture)   = = NULL<br> |
|  Magnum::Float | [**\_shininess**](#variable-_shininess)  <br> |
|  Magnum::Color4 | [**\_specular**](#variable-_specular)  <br> |
|  Magnum::GL::Texture2D \* | [**\_specular\_texture**](#variable-_specular_texture)   = = NULL<br> |




















## Public Functions Documentation




### function Material [1/3]

```C++
robot_dart::gui::magnum::gs::Material::Material () 
```






### function Material [2/3]

```C++
robot_dart::gui::magnum::gs::Material::Material (
    const Magnum::Color4 & ambient,
    const Magnum::Color4 & diffuse,
    const Magnum::Color4 & specular,
    Magnum::Float shininess
) 
```






### function Material [3/3]

```C++
robot_dart::gui::magnum::gs::Material::Material (
    Magnum::GL::Texture2D * ambient_texture,
    Magnum::GL::Texture2D * diffuse_texture,
    Magnum::GL::Texture2D * specular_texture,
    Magnum::Float shininess
) 
```






### function ambient\_color [1/2]

```C++
Magnum::Color4 & robot_dart::gui::magnum::gs::Material::ambient_color () 
```






### function ambient\_color [2/2]

```C++
Magnum::Color4 robot_dart::gui::magnum::gs::Material::ambient_color () const
```






### function ambient\_texture 

```C++
Magnum::GL::Texture2D * robot_dart::gui::magnum::gs::Material::ambient_texture () 
```






### function diffuse\_color [1/2]

```C++
Magnum::Color4 & robot_dart::gui::magnum::gs::Material::diffuse_color () 
```






### function diffuse\_color [2/2]

```C++
Magnum::Color4 robot_dart::gui::magnum::gs::Material::diffuse_color () const
```






### function diffuse\_texture 

```C++
Magnum::GL::Texture2D * robot_dart::gui::magnum::gs::Material::diffuse_texture () 
```






### function has\_ambient\_texture 

```C++
bool robot_dart::gui::magnum::gs::Material::has_ambient_texture () const
```






### function has\_diffuse\_texture 

```C++
bool robot_dart::gui::magnum::gs::Material::has_diffuse_texture () const
```






### function has\_specular\_texture 

```C++
bool robot_dart::gui::magnum::gs::Material::has_specular_texture () const
```






### function set\_ambient\_color 

```C++
Material & robot_dart::gui::magnum::gs::Material::set_ambient_color (
    const Magnum::Color4 & ambient
) 
```






### function set\_ambient\_texture 

```C++
Material & robot_dart::gui::magnum::gs::Material::set_ambient_texture (
    Magnum::GL::Texture2D * ambient_texture
) 
```






### function set\_diffuse\_color 

```C++
Material & robot_dart::gui::magnum::gs::Material::set_diffuse_color (
    const Magnum::Color4 & diffuse
) 
```






### function set\_diffuse\_texture 

```C++
Material & robot_dart::gui::magnum::gs::Material::set_diffuse_texture (
    Magnum::GL::Texture2D * diffuse_texture
) 
```






### function set\_shininess 

```C++
Material & robot_dart::gui::magnum::gs::Material::set_shininess (
    Magnum::Float shininess
) 
```






### function set\_specular\_color 

```C++
Material & robot_dart::gui::magnum::gs::Material::set_specular_color (
    const Magnum::Color4 & specular
) 
```






### function set\_specular\_texture 

```C++
Material & robot_dart::gui::magnum::gs::Material::set_specular_texture (
    Magnum::GL::Texture2D * specular_texture
) 
```






### function shininess [1/2]

```C++
Magnum::Float & robot_dart::gui::magnum::gs::Material::shininess () 
```






### function shininess [2/2]

```C++
Magnum::Float robot_dart::gui::magnum::gs::Material::shininess () const
```






### function specular\_color [1/2]

```C++
Magnum::Color4 & robot_dart::gui::magnum::gs::Material::specular_color () 
```






### function specular\_color [2/2]

```C++
Magnum::Color4 robot_dart::gui::magnum::gs::Material::specular_color () const
```






### function specular\_texture 

```C++
Magnum::GL::Texture2D * robot_dart::gui::magnum::gs::Material::specular_texture () 
```



## Protected Attributes Documentation




### variable \_ambient 

```C++
Magnum::Color4 robot_dart::gui::magnum::gs::Material::_ambient;
```






### variable \_ambient\_texture 

```C++
Magnum::GL::Texture2D* robot_dart::gui::magnum::gs::Material::_ambient_texture;
```






### variable \_diffuse 

```C++
Magnum::Color4 robot_dart::gui::magnum::gs::Material::_diffuse;
```






### variable \_diffuse\_texture 

```C++
Magnum::GL::Texture2D* robot_dart::gui::magnum::gs::Material::_diffuse_texture;
```






### variable \_shininess 

```C++
Magnum::Float robot_dart::gui::magnum::gs::Material::_shininess;
```






### variable \_specular 

```C++
Magnum::Color4 robot_dart::gui::magnum::gs::Material::_specular;
```






### variable \_specular\_texture 

```C++
Magnum::GL::Texture2D* robot_dart::gui::magnum::gs::Material::_specular_texture;
```




------------------------------
The documentation for this class was generated from the following file `robot_dart/gui/magnum/gs/material.hpp`

