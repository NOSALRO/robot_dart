

# Namespace robot\_dart::gui



[**Namespace List**](namespaces.md) **>** [**robot\_dart**](namespacerobot__dart.md) **>** [**gui**](namespacerobot__dart_1_1gui.md)


















## Namespaces

| Type | Name |
| ---: | :--- |
| namespace | [**magnum**](namespacerobot__dart_1_1gui_1_1magnum.md) <br> |


## Classes

| Type | Name |
| ---: | :--- |
| class | [**Base**](classrobot__dart_1_1gui_1_1Base.md) <br> |
| struct | [**DepthImage**](structrobot__dart_1_1gui_1_1DepthImage.md) <br> |
| struct | [**GrayscaleImage**](structrobot__dart_1_1gui_1_1GrayscaleImage.md) <br> |
| struct | [**Image**](structrobot__dart_1_1gui_1_1Image.md) <br> |






















## Public Functions

| Type | Name |
| ---: | :--- |
|  [**GrayscaleImage**](structrobot__dart_1_1gui_1_1GrayscaleImage.md) | [**convert\_rgb\_to\_grayscale**](#function-convert_rgb_to_grayscale) (const [**Image**](structrobot__dart_1_1gui_1_1Image.md) & rgb) <br> |
|  std::vector&lt; Eigen::Vector3d &gt; | [**point\_cloud\_from\_depth\_array**](#function-point_cloud_from_depth_array) (const [**DepthImage**](structrobot__dart_1_1gui_1_1DepthImage.md) & depth\_image, const Eigen::Matrix3d & intrinsic\_matrix, const Eigen::Matrix4d & tf, double far\_plane) <br> |
|  void | [**save\_png\_image**](#function-save_png_image) (const std::string & filename, const [**Image**](structrobot__dart_1_1gui_1_1Image.md) & rgb) <br> |
|  void | [**save\_png\_image**](#function-save_png_image) (const std::string & filename, const [**GrayscaleImage**](structrobot__dart_1_1gui_1_1GrayscaleImage.md) & gray) <br> |




























## Public Functions Documentation




### function convert\_rgb\_to\_grayscale 

```C++
GrayscaleImage robot_dart::gui::convert_rgb_to_grayscale (
    const Image & rgb
) 
```






### function point\_cloud\_from\_depth\_array 

```C++
std::vector< Eigen::Vector3d > robot_dart::gui::point_cloud_from_depth_array (
    const DepthImage & depth_image,
    const Eigen::Matrix3d & intrinsic_matrix,
    const Eigen::Matrix4d & tf,
    double far_plane
) 
```






### function save\_png\_image 

```C++
void robot_dart::gui::save_png_image (
    const std::string & filename,
    const Image & rgb
) 
```






### function save\_png\_image 

```C++
void robot_dart::gui::save_png_image (
    const std::string & filename,
    const GrayscaleImage & gray
) 
```




------------------------------
The documentation for this class was generated from the following file `robot_dart/gui/base.hpp`

