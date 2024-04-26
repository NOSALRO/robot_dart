

# Class robot\_dart::gui::magnum::WindowlessGLApplication



[**ClassList**](annotated.md) **>** [**robot\_dart**](namespacerobot__dart.md) **>** [**gui**](namespacerobot__dart_1_1gui.md) **>** [**magnum**](namespacerobot__dart_1_1gui_1_1magnum.md) **>** [**WindowlessGLApplication**](classrobot__dart_1_1gui_1_1magnum_1_1WindowlessGLApplication.md)








Inherits the following classes: [robot\_dart::gui::magnum::BaseApplication](classrobot__dart_1_1gui_1_1magnum_1_1BaseApplication.md),  Magnum::Platform::WindowlessApplication






















































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**WindowlessGLApplication**](#function-windowlessglapplication) (int argc, char \*\* argv, [**RobotDARTSimu**](classrobot__dart_1_1RobotDARTSimu.md) \* simu, const [**GraphicsConfiguration**](structrobot__dart_1_1gui_1_1magnum_1_1GraphicsConfiguration.md) & configuration=[**GraphicsConfiguration**](structrobot__dart_1_1gui_1_1magnum_1_1GraphicsConfiguration.md)()) <br> |
| virtual void | [**render**](#function-render) () override<br> |
|   | [**~WindowlessGLApplication**](#function-windowlessglapplication) () <br> |


## Public Functions inherited from robot_dart::gui::magnum::BaseApplication

See [robot\_dart::gui::magnum::BaseApplication](classrobot__dart_1_1gui_1_1magnum_1_1BaseApplication.md)

| Type | Name |
| ---: | :--- |
|   | [**BaseApplication**](#function-baseapplication) (const [**GraphicsConfiguration**](structrobot__dart_1_1gui_1_1magnum_1_1GraphicsConfiguration.md) & configuration=[**GraphicsConfiguration**](structrobot__dart_1_1gui_1_1magnum_1_1GraphicsConfiguration.md)()) <br> |
|  void | [**add\_light**](#function-add_light) (const [**gs::Light**](classrobot__dart_1_1gui_1_1magnum_1_1gs_1_1Light.md) & light) <br> |
|  bool | [**attach\_camera**](#function-attach_camera) ([**gs::Camera**](classrobot__dart_1_1gui_1_1magnum_1_1gs_1_1Camera.md) & camera, dart::dynamics::BodyNode \* body) <br> |
|  [**gs::Camera**](classrobot__dart_1_1gui_1_1magnum_1_1gs_1_1Camera.md) & | [**camera**](#function-camera-12) () <br> |
|  const [**gs::Camera**](classrobot__dart_1_1gui_1_1magnum_1_1gs_1_1Camera.md) & | [**camera**](#function-camera-22) () const<br> |
|  void | [**clear\_lights**](#function-clear_lights) () <br> |
|  [**DebugDrawData**](structrobot__dart_1_1gui_1_1magnum_1_1DebugDrawData.md) | [**debug\_draw\_data**](#function-debug_draw_data) () <br> |
|  [**DepthImage**](structrobot__dart_1_1gui_1_1DepthImage.md) | [**depth\_array**](#function-depth_array) () <br> |
|  [**GrayscaleImage**](structrobot__dart_1_1gui_1_1GrayscaleImage.md) | [**depth\_image**](#function-depth_image) () <br> |
|  bool | [**done**](#function-done) () const<br> |
|  Magnum::SceneGraph::DrawableGroup3D & | [**drawables**](#function-drawables) () <br> |
|  void | [**enable\_shadows**](#function-enable_shadows) (bool enable=true, bool drawTransparentShadows=false) <br> |
|  Corrade::Containers::Optional&lt; Magnum::Image2D &gt; & | [**image**](#function-image) () <br> |
|  void | [**init**](#function-init) ([**RobotDARTSimu**](classrobot__dart_1_1RobotDARTSimu.md) \* simu, const [**GraphicsConfiguration**](structrobot__dart_1_1gui_1_1magnum_1_1GraphicsConfiguration.md) & configuration) <br> |
|  [**gs::Light**](classrobot__dart_1_1gui_1_1magnum_1_1gs_1_1Light.md) & | [**light**](#function-light) (size\_t i) <br> |
|  std::vector&lt; [**gs::Light**](classrobot__dart_1_1gui_1_1magnum_1_1gs_1_1Light.md) &gt; & | [**lights**](#function-lights) () <br> |
|  void | [**look\_at**](#function-look_at) (const Eigen::Vector3d & camera\_pos, const Eigen::Vector3d & look\_at, const Eigen::Vector3d & up) <br> |
|  size\_t | [**num\_lights**](#function-num_lights) () const<br> |
|  [**GrayscaleImage**](structrobot__dart_1_1gui_1_1GrayscaleImage.md) | [**raw\_depth\_image**](#function-raw_depth_image) () <br> |
|  void | [**record\_video**](#function-record_video) (const std::string & video\_fname, int fps) <br> |
| virtual void | [**render**](#function-render) () <br> |
|  void | [**render\_shadows**](#function-render_shadows) () <br> |
|  Scene3D & | [**scene**](#function-scene) () <br> |
|  bool | [**shadowed**](#function-shadowed) () const<br> |
|  bool | [**transparent\_shadows**](#function-transparent_shadows) () const<br> |
|  void | [**update\_graphics**](#function-update_graphics) () <br> |
|  void | [**update\_lights**](#function-update_lights) (const [**gs::Camera**](classrobot__dart_1_1gui_1_1magnum_1_1gs_1_1Camera.md) & camera) <br> |
| virtual  | [**~BaseApplication**](#function-baseapplication) () <br> |














## Protected Attributes

| Type | Name |
| ---: | :--- |
|  Magnum::Color4 | [**\_bg\_color**](#variable-_bg_color)  <br> |
|  Magnum::GL::Renderbuffer | [**\_color**](#variable-_color)   = {Magnum::NoCreate}<br> |
|  Magnum::GL::Renderbuffer | [**\_depth**](#variable-_depth)   = {Magnum::NoCreate}<br> |
|  bool | [**\_draw\_debug**](#variable-_draw_debug)  <br> |
|  bool | [**\_draw\_main\_camera**](#variable-_draw_main_camera)  <br> |
|  Magnum::PixelFormat | [**\_format**](#variable-_format)  <br> |
|  Magnum::GL::Framebuffer | [**\_framebuffer**](#variable-_framebuffer)   = {Magnum::NoCreate}<br> |
|  [**RobotDARTSimu**](classrobot__dart_1_1RobotDARTSimu.md) \* | [**\_simu**](#variable-_simu)  <br> |


## Protected Attributes inherited from robot_dart::gui::magnum::BaseApplication

See [robot\_dart::gui::magnum::BaseApplication](classrobot__dart_1_1gui_1_1magnum_1_1BaseApplication.md)

| Type | Name |
| ---: | :--- |
|  std::unique\_ptr&lt; Magnum::GL::Mesh &gt; | [**\_3D\_axis\_mesh**](#variable-_3d_axis_mesh)  <br> |
|  std::unique\_ptr&lt; Magnum::Shaders::VertexColorGL3D &gt; | [**\_3D\_axis\_shader**](#variable-_3d_axis_shader)  <br> |
|  std::unique\_ptr&lt; Magnum::GL::Mesh &gt; | [**\_background\_mesh**](#variable-_background_mesh)  <br> |
|  std::unique\_ptr&lt; Magnum::Shaders::FlatGL2D &gt; | [**\_background\_shader**](#variable-_background_shader)  <br> |
|  std::unique\_ptr&lt; [**gs::Camera**](classrobot__dart_1_1gui_1_1magnum_1_1gs_1_1Camera.md) &gt; | [**\_camera**](#variable-_camera)  <br> |
|  std::unique\_ptr&lt; [**gs::PhongMultiLight**](classrobot__dart_1_1gui_1_1magnum_1_1gs_1_1PhongMultiLight.md) &gt; | [**\_color\_shader**](#variable-_color_shader)  <br> |
|  [**GraphicsConfiguration**](structrobot__dart_1_1gui_1_1magnum_1_1GraphicsConfiguration.md) | [**\_configuration**](#variable-_configuration)  <br> |
|  Magnum::SceneGraph::DrawableGroup3D | [**\_cubemap\_color\_drawables**](#variable-_cubemap_color_drawables)  <br> |
|  std::unique\_ptr&lt; [**gs::CubeMapColor**](classrobot__dart_1_1gui_1_1magnum_1_1gs_1_1CubeMapColor.md) &gt; | [**\_cubemap\_color\_shader**](#variable-_cubemap_color_shader)  <br> |
|  Magnum::SceneGraph::DrawableGroup3D | [**\_cubemap\_drawables**](#variable-_cubemap_drawables)  <br> |
|  std::unique\_ptr&lt; [**gs::CubeMap**](classrobot__dart_1_1gui_1_1magnum_1_1gs_1_1CubeMap.md) &gt; | [**\_cubemap\_shader**](#variable-_cubemap_shader)  <br> |
|  std::unique\_ptr&lt; [**gs::CubeMapColor**](classrobot__dart_1_1gui_1_1magnum_1_1gs_1_1CubeMapColor.md) &gt; | [**\_cubemap\_texture\_color\_shader**](#variable-_cubemap_texture_color_shader)  <br> |
|  std::unique\_ptr&lt; [**gs::CubeMap**](classrobot__dart_1_1gui_1_1magnum_1_1gs_1_1CubeMap.md) &gt; | [**\_cubemap\_texture\_shader**](#variable-_cubemap_texture_shader)  <br> |
|  std::unique\_ptr&lt; Magnum::DartIntegration::World &gt; | [**\_dart\_world**](#variable-_dart_world)  <br> |
|  bool | [**\_done**](#variable-_done)   = = false<br> |
|  std::unordered\_map&lt; Magnum::DartIntegration::Object \*, [**ObjectStruct**](structrobot__dart_1_1gui_1_1magnum_1_1ObjectStruct.md) \* &gt; | [**\_drawable\_objects**](#variable-_drawable_objects)  <br> |
|  Magnum::SceneGraph::DrawableGroup3D | [**\_drawables**](#variable-_drawables)  <br> |
|  Corrade::Containers::Pointer&lt; Magnum::Text::AbstractFont &gt; | [**\_font**](#variable-_font)  <br> |
|  Corrade::PluginManager::Manager&lt; Magnum::Text::AbstractFont &gt; | [**\_font\_manager**](#variable-_font_manager)  <br> |
|  Corrade::Containers::Pointer&lt; Magnum::Text::DistanceFieldGlyphCache &gt; | [**\_glyph\_cache**](#variable-_glyph_cache)  <br> |
|  Corrade::PluginManager::Manager&lt; Magnum::Trade::AbstractImporter &gt; | [**\_importer\_manager**](#variable-_importer_manager)  <br> |
|  std::vector&lt; [**gs::Light**](classrobot__dart_1_1gui_1_1magnum_1_1gs_1_1Light.md) &gt; | [**\_lights**](#variable-_lights)  <br> |
|  int | [**\_max\_lights**](#variable-_max_lights)   = = 5<br> |
|  Scene3D | [**\_scene**](#variable-_scene)  <br> |
|  std::unique\_ptr&lt; Camera3D &gt; | [**\_shadow\_camera**](#variable-_shadow_camera)  <br> |
|  Object3D \* | [**\_shadow\_camera\_object**](#variable-_shadow_camera_object)  <br> |
|  std::unique\_ptr&lt; Magnum::GL::CubeMapTextureArray &gt; | [**\_shadow\_color\_cube\_map**](#variable-_shadow_color_cube_map)  <br> |
|  std::unique\_ptr&lt; [**gs::ShadowMapColor**](classrobot__dart_1_1gui_1_1magnum_1_1gs_1_1ShadowMapColor.md) &gt; | [**\_shadow\_color\_shader**](#variable-_shadow_color_shader)  <br> |
|  std::unique\_ptr&lt; Magnum::GL::Texture2DArray &gt; | [**\_shadow\_color\_texture**](#variable-_shadow_color_texture)  <br> |
|  std::unique\_ptr&lt; Magnum::GL::CubeMapTextureArray &gt; | [**\_shadow\_cube\_map**](#variable-_shadow_cube_map)  <br> |
|  std::vector&lt; [**ShadowData**](structrobot__dart_1_1gui_1_1magnum_1_1ShadowData.md) &gt; | [**\_shadow\_data**](#variable-_shadow_data)  <br> |
|  int | [**\_shadow\_map\_size**](#variable-_shadow_map_size)   = = 512<br> |
|  std::unique\_ptr&lt; [**gs::ShadowMap**](classrobot__dart_1_1gui_1_1magnum_1_1gs_1_1ShadowMap.md) &gt; | [**\_shadow\_shader**](#variable-_shadow_shader)  <br> |
|  std::unique\_ptr&lt; Magnum::GL::Texture2DArray &gt; | [**\_shadow\_texture**](#variable-_shadow_texture)  <br> |
|  std::unique\_ptr&lt; [**gs::ShadowMapColor**](classrobot__dart_1_1gui_1_1magnum_1_1gs_1_1ShadowMapColor.md) &gt; | [**\_shadow\_texture\_color\_shader**](#variable-_shadow_texture_color_shader)  <br> |
|  std::unique\_ptr&lt; [**gs::ShadowMap**](classrobot__dart_1_1gui_1_1magnum_1_1gs_1_1ShadowMap.md) &gt; | [**\_shadow\_texture\_shader**](#variable-_shadow_texture_shader)  <br> |
|  bool | [**\_shadowed**](#variable-_shadowed)   = = true<br> |
|  Magnum::SceneGraph::DrawableGroup3D | [**\_shadowed\_color\_drawables**](#variable-_shadowed_color_drawables)  <br> |
|  Magnum::SceneGraph::DrawableGroup3D | [**\_shadowed\_drawables**](#variable-_shadowed_drawables)  <br> |
|  [**RobotDARTSimu**](classrobot__dart_1_1RobotDARTSimu.md) \* | [**\_simu**](#variable-_simu)  <br> |
|  Corrade::Containers::Pointer&lt; Magnum::GL::Buffer &gt; | [**\_text\_indices**](#variable-_text_indices)  <br> |
|  std::unique\_ptr&lt; Magnum::Shaders::DistanceFieldVectorGL2D &gt; | [**\_text\_shader**](#variable-_text_shader)  <br> |
|  Corrade::Containers::Pointer&lt; Magnum::GL::Buffer &gt; | [**\_text\_vertices**](#variable-_text_vertices)  <br> |
|  std::unique\_ptr&lt; [**gs::PhongMultiLight**](classrobot__dart_1_1gui_1_1magnum_1_1gs_1_1PhongMultiLight.md) &gt; | [**\_texture\_shader**](#variable-_texture_shader)  <br> |
|  int | [**\_transparentSize**](#variable-_transparentsize)   = = 0<br> |
|  bool | [**\_transparent\_shadows**](#variable-_transparent_shadows)   = = false<br> |






























## Protected Functions

| Type | Name |
| ---: | :--- |
| virtual int | [**exec**](#function-exec) () override<br> |


## Protected Functions inherited from robot_dart::gui::magnum::BaseApplication

See [robot\_dart::gui::magnum::BaseApplication](classrobot__dart_1_1gui_1_1magnum_1_1BaseApplication.md)

| Type | Name |
| ---: | :--- |
|  void | [**\_gl\_clean\_up**](#function-_gl_clean_up) () <br> |
|  void | [**\_prepare\_shadows**](#function-_prepare_shadows) () <br> |






## Public Functions Documentation




### function WindowlessGLApplication 

```C++
explicit robot_dart::gui::magnum::WindowlessGLApplication::WindowlessGLApplication (
    int argc,
    char ** argv,
    RobotDARTSimu * simu,
    const GraphicsConfiguration & configuration=GraphicsConfiguration ()
) 
```




<hr>



### function render 

```C++
virtual void robot_dart::gui::magnum::WindowlessGLApplication::render () override
```



Implements [*robot\_dart::gui::magnum::BaseApplication::render*](classrobot__dart_1_1gui_1_1magnum_1_1BaseApplication.md#function-render)


<hr>



### function ~WindowlessGLApplication 

```C++
robot_dart::gui::magnum::WindowlessGLApplication::~WindowlessGLApplication () 
```




<hr>
## Protected Attributes Documentation




### variable \_bg\_color 

```C++
Magnum::Color4 robot_dart::gui::magnum::WindowlessGLApplication::_bg_color;
```




<hr>



### variable \_color 

```C++
Magnum::GL::Renderbuffer robot_dart::gui::magnum::WindowlessGLApplication::_color;
```




<hr>



### variable \_depth 

```C++
Magnum::GL::Renderbuffer robot_dart::gui::magnum::WindowlessGLApplication::_depth;
```




<hr>



### variable \_draw\_debug 

```C++
bool robot_dart::gui::magnum::WindowlessGLApplication::_draw_debug;
```




<hr>



### variable \_draw\_main\_camera 

```C++
bool robot_dart::gui::magnum::WindowlessGLApplication::_draw_main_camera;
```




<hr>



### variable \_format 

```C++
Magnum::PixelFormat robot_dart::gui::magnum::WindowlessGLApplication::_format;
```




<hr>



### variable \_framebuffer 

```C++
Magnum::GL::Framebuffer robot_dart::gui::magnum::WindowlessGLApplication::_framebuffer;
```




<hr>



### variable \_simu 

```C++
RobotDARTSimu* robot_dart::gui::magnum::WindowlessGLApplication::_simu;
```




<hr>
## Protected Functions Documentation




### function exec 

```C++
inline virtual int robot_dart::gui::magnum::WindowlessGLApplication::exec () override
```




<hr>

------------------------------
The documentation for this class was generated from the following file `robot_dart/gui/magnum/windowless_gl_application.hpp`

