

# Namespace robot\_dart::gui::magnum::gs



[**Namespace List**](namespaces.md) **>** [**robot\_dart**](namespacerobot__dart.md) **>** [**gui**](namespacerobot__dart_1_1gui.md) **>** [**magnum**](namespacerobot__dart_1_1gui_1_1magnum.md) **>** [**gs**](namespacerobot__dart_1_1gui_1_1magnum_1_1gs.md)




















## Classes

| Type | Name |
| ---: | :--- |
| class | [**Camera**](classrobot__dart_1_1gui_1_1magnum_1_1gs_1_1Camera.md) <br> |
| class | [**CubeMap**](classrobot__dart_1_1gui_1_1magnum_1_1gs_1_1CubeMap.md) <br> |
| class | [**CubeMapColor**](classrobot__dart_1_1gui_1_1magnum_1_1gs_1_1CubeMapColor.md) <br> |
| class | [**Light**](classrobot__dart_1_1gui_1_1magnum_1_1gs_1_1Light.md) <br> |
| class | [**Material**](classrobot__dart_1_1gui_1_1magnum_1_1gs_1_1Material.md) <br> |
| class | [**PhongMultiLight**](classrobot__dart_1_1gui_1_1magnum_1_1gs_1_1PhongMultiLight.md) <br> |
| class | [**ShadowMap**](classrobot__dart_1_1gui_1_1magnum_1_1gs_1_1ShadowMap.md) <br> |
| class | [**ShadowMapColor**](classrobot__dart_1_1gui_1_1magnum_1_1gs_1_1ShadowMapColor.md) <br> |






















## Public Functions

| Type | Name |
| ---: | :--- |
|  [**Light**](classrobot__dart_1_1gui_1_1magnum_1_1gs_1_1Light.md) | [**create\_directional\_light**](#function-create_directional_light) (const Magnum::Vector3 & direction, const [**Material**](classrobot__dart_1_1gui_1_1magnum_1_1gs_1_1Material.md) & material) <br> |
|  [**Light**](classrobot__dart_1_1gui_1_1magnum_1_1gs_1_1Light.md) | [**create\_point\_light**](#function-create_point_light) (const Magnum::Vector3 & position, const [**Material**](classrobot__dart_1_1gui_1_1magnum_1_1gs_1_1Material.md) & material, Magnum::Float intensity, const Magnum::Vector3 & attenuationTerms) <br> |
|  [**Light**](classrobot__dart_1_1gui_1_1magnum_1_1gs_1_1Light.md) | [**create\_spot\_light**](#function-create_spot_light) (const Magnum::Vector3 & position, const [**Material**](classrobot__dart_1_1gui_1_1magnum_1_1gs_1_1Material.md) & material, const Magnum::Vector3 & spot\_direction, Magnum::Float spot\_exponent, Magnum::Float spot\_cut\_off, Magnum::Float intensity, const Magnum::Vector3 & attenuationTerms) <br> |
|  [**DepthImage**](structrobot__dart_1_1gui_1_1DepthImage.md) | [**depth\_array\_from\_image**](#function-depth_array_from_image) (Magnum::Image2D \* image, Magnum::Float near\_plane, Magnum::Float far\_plane) <br> |
|  [**GrayscaleImage**](structrobot__dart_1_1gui_1_1GrayscaleImage.md) | [**depth\_from\_image**](#function-depth_from_image) (Magnum::Image2D \* image, bool linearize, Magnum::Float near\_plane, Magnum::Float far\_plane) <br> |
|  [**Image**](structrobot__dart_1_1gui_1_1Image.md) | [**rgb\_from\_image**](#function-rgb_from_image) (Magnum::Image2D \* image) <br> |


## Public Static Functions

| Type | Name |
| ---: | :--- |
|  fs::path | [**search\_path**](#function-search_path) (const fs::path & filename) <br> |


























## Public Functions Documentation




### function create\_directional\_light 

```C++
inline Light robot_dart::gui::magnum::gs::create_directional_light (
    const Magnum::Vector3 & direction,
    const Material & material
) 
```




<hr>



### function create\_point\_light 

```C++
inline Light robot_dart::gui::magnum::gs::create_point_light (
    const Magnum::Vector3 & position,
    const Material & material,
    Magnum::Float intensity,
    const Magnum::Vector3 & attenuationTerms
) 
```




<hr>



### function create\_spot\_light 

```C++
inline Light robot_dart::gui::magnum::gs::create_spot_light (
    const Magnum::Vector3 & position,
    const Material & material,
    const Magnum::Vector3 & spot_direction,
    Magnum::Float spot_exponent,
    Magnum::Float spot_cut_off,
    Magnum::Float intensity,
    const Magnum::Vector3 & attenuationTerms
) 
```




<hr>



### function depth\_array\_from\_image 

```C++
DepthImage robot_dart::gui::magnum::gs::depth_array_from_image (
    Magnum::Image2D * image,
    Magnum::Float near_plane,
    Magnum::Float far_plane
) 
```




<hr>



### function depth\_from\_image 

```C++
GrayscaleImage robot_dart::gui::magnum::gs::depth_from_image (
    Magnum::Image2D * image,
    bool linearize,
    Magnum::Float near_plane,
    Magnum::Float far_plane
) 
```




<hr>



### function rgb\_from\_image 

```C++
Image robot_dart::gui::magnum::gs::rgb_from_image (
    Magnum::Image2D * image
) 
```




<hr>
## Public Static Functions Documentation




### function search\_path 

```C++
static fs::path robot_dart::gui::magnum::gs::search_path (
    const fs::path & filename
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `robot_dart/gui/magnum/gs/camera.cpp`

