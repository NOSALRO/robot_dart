

# Class robot\_dart::gui::magnum::BaseApplication



[**ClassList**](annotated.md) **>** [**robot\_dart**](namespacerobot__dart.md) **>** [**gui**](namespacerobot__dart_1_1gui.md) **>** [**magnum**](namespacerobot__dart_1_1gui_1_1magnum.md) **>** [**BaseApplication**](classrobot__dart_1_1gui_1_1magnum_1_1BaseApplication.md)










Inherited by the following classes: [robot\_dart::gui::magnum::GlfwApplication](classrobot__dart_1_1gui_1_1magnum_1_1GlfwApplication.md),  [robot\_dart::gui::magnum::WindowlessGLApplication](classrobot__dart_1_1gui_1_1magnum_1_1WindowlessGLApplication.md)
































## Public Functions

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
|  void | [**\_gl\_clean\_up**](#function-_gl_clean_up) () <br> |
|  void | [**\_prepare\_shadows**](#function-_prepare_shadows) () <br> |




## Public Functions Documentation




### function BaseApplication 

```C++
robot_dart::gui::magnum::BaseApplication::BaseApplication (
    const GraphicsConfiguration & configuration=GraphicsConfiguration ()
) 
```






### function add\_light 

```C++
void robot_dart::gui::magnum::BaseApplication::add_light (
    const gs::Light & light
) 
```






### function attach\_camera 

```C++
bool robot_dart::gui::magnum::BaseApplication::attach_camera (
    gs::Camera & camera,
    dart::dynamics::BodyNode * body
) 
```






### function camera [1/2]

```C++
inline gs::Camera & robot_dart::gui::magnum::BaseApplication::camera () 
```






### function camera [2/2]

```C++
inline const gs::Camera & robot_dart::gui::magnum::BaseApplication::camera () const
```






### function clear\_lights 

```C++
void robot_dart::gui::magnum::BaseApplication::clear_lights () 
```






### function debug\_draw\_data 

```C++
inline DebugDrawData robot_dart::gui::magnum::BaseApplication::debug_draw_data () 
```






### function depth\_array 

```C++
DepthImage robot_dart::gui::magnum::BaseApplication::depth_array () 
```






### function depth\_image 

```C++
GrayscaleImage robot_dart::gui::magnum::BaseApplication::depth_image () 
```






### function done 

```C++
bool robot_dart::gui::magnum::BaseApplication::done () const
```






### function drawables 

```C++
inline Magnum::SceneGraph::DrawableGroup3D & robot_dart::gui::magnum::BaseApplication::drawables () 
```






### function enable\_shadows 

```C++
void robot_dart::gui::magnum::BaseApplication::enable_shadows (
    bool enable=true,
    bool drawTransparentShadows=false
) 
```






### function image 

```C++
inline Corrade::Containers::Optional< Magnum::Image2D > & robot_dart::gui::magnum::BaseApplication::image () 
```






### function init 

```C++
void robot_dart::gui::magnum::BaseApplication::init (
    RobotDARTSimu * simu,
    const GraphicsConfiguration & configuration
) 
```






### function light 

```C++
gs::Light & robot_dart::gui::magnum::BaseApplication::light (
    size_t i
) 
```






### function lights 

```C++
std::vector< gs::Light > & robot_dart::gui::magnum::BaseApplication::lights () 
```






### function look\_at 

```C++
void robot_dart::gui::magnum::BaseApplication::look_at (
    const Eigen::Vector3d & camera_pos,
    const Eigen::Vector3d & look_at,
    const Eigen::Vector3d & up
) 
```






### function num\_lights 

```C++
size_t robot_dart::gui::magnum::BaseApplication::num_lights () const
```






### function raw\_depth\_image 

```C++
GrayscaleImage robot_dart::gui::magnum::BaseApplication::raw_depth_image () 
```






### function record\_video 

```C++
inline void robot_dart::gui::magnum::BaseApplication::record_video (
    const std::string & video_fname,
    int fps
) 
```






### function render 

```C++
inline virtual void robot_dart::gui::magnum::BaseApplication::render () 
```






### function render\_shadows 

```C++
void robot_dart::gui::magnum::BaseApplication::render_shadows () 
```






### function scene 

```C++
inline Scene3D & robot_dart::gui::magnum::BaseApplication::scene () 
```






### function shadowed 

```C++
inline bool robot_dart::gui::magnum::BaseApplication::shadowed () const
```






### function transparent\_shadows 

```C++
inline bool robot_dart::gui::magnum::BaseApplication::transparent_shadows () const
```






### function update\_graphics 

```C++
void robot_dart::gui::magnum::BaseApplication::update_graphics () 
```






### function update\_lights 

```C++
void robot_dart::gui::magnum::BaseApplication::update_lights (
    const gs::Camera & camera
) 
```






### function ~BaseApplication 

```C++
inline virtual robot_dart::gui::magnum::BaseApplication::~BaseApplication () 
```



## Protected Attributes Documentation




### variable \_3D\_axis\_mesh 

```C++
std::unique_ptr<Magnum::GL::Mesh> robot_dart::gui::magnum::BaseApplication::_3D_axis_mesh;
```






### variable \_3D\_axis\_shader 

```C++
std::unique_ptr<Magnum::Shaders::VertexColorGL3D> robot_dart::gui::magnum::BaseApplication::_3D_axis_shader;
```






### variable \_background\_mesh 

```C++
std::unique_ptr<Magnum::GL::Mesh> robot_dart::gui::magnum::BaseApplication::_background_mesh;
```






### variable \_background\_shader 

```C++
std::unique_ptr<Magnum::Shaders::FlatGL2D> robot_dart::gui::magnum::BaseApplication::_background_shader;
```






### variable \_camera 

```C++
std::unique_ptr<gs::Camera> robot_dart::gui::magnum::BaseApplication::_camera;
```






### variable \_color\_shader 

```C++
std::unique_ptr<gs::PhongMultiLight> robot_dart::gui::magnum::BaseApplication::_color_shader;
```






### variable \_configuration 

```C++
GraphicsConfiguration robot_dart::gui::magnum::BaseApplication::_configuration;
```






### variable \_cubemap\_color\_drawables 

```C++
Magnum::SceneGraph::DrawableGroup3D robot_dart::gui::magnum::BaseApplication::_cubemap_color_drawables;
```






### variable \_cubemap\_color\_shader 

```C++
std::unique_ptr<gs::CubeMapColor> robot_dart::gui::magnum::BaseApplication::_cubemap_color_shader;
```






### variable \_cubemap\_drawables 

```C++
Magnum::SceneGraph::DrawableGroup3D robot_dart::gui::magnum::BaseApplication::_cubemap_drawables;
```






### variable \_cubemap\_shader 

```C++
std::unique_ptr<gs::CubeMap> robot_dart::gui::magnum::BaseApplication::_cubemap_shader;
```






### variable \_cubemap\_texture\_color\_shader 

```C++
std::unique_ptr<gs::CubeMapColor> robot_dart::gui::magnum::BaseApplication::_cubemap_texture_color_shader;
```






### variable \_cubemap\_texture\_shader 

```C++
std::unique_ptr<gs::CubeMap> robot_dart::gui::magnum::BaseApplication::_cubemap_texture_shader;
```






### variable \_dart\_world 

```C++
std::unique_ptr<Magnum::DartIntegration::World> robot_dart::gui::magnum::BaseApplication::_dart_world;
```






### variable \_done 

```C++
bool robot_dart::gui::magnum::BaseApplication::_done;
```






### variable \_drawable\_objects 

```C++
std::unordered_map<Magnum::DartIntegration::Object*, ObjectStruct*> robot_dart::gui::magnum::BaseApplication::_drawable_objects;
```






### variable \_drawables 

```C++
Magnum::SceneGraph::DrawableGroup3D robot_dart::gui::magnum::BaseApplication::_drawables;
```






### variable \_font 

```C++
Corrade::Containers::Pointer<Magnum::Text::AbstractFont> robot_dart::gui::magnum::BaseApplication::_font;
```






### variable \_font\_manager 

```C++
Corrade::PluginManager::Manager<Magnum::Text::AbstractFont> robot_dart::gui::magnum::BaseApplication::_font_manager;
```






### variable \_glyph\_cache 

```C++
Corrade::Containers::Pointer<Magnum::Text::DistanceFieldGlyphCache> robot_dart::gui::magnum::BaseApplication::_glyph_cache;
```






### variable \_importer\_manager 

```C++
Corrade::PluginManager::Manager<Magnum::Trade::AbstractImporter> robot_dart::gui::magnum::BaseApplication::_importer_manager;
```






### variable \_lights 

```C++
std::vector<gs::Light> robot_dart::gui::magnum::BaseApplication::_lights;
```






### variable \_max\_lights 

```C++
int robot_dart::gui::magnum::BaseApplication::_max_lights;
```






### variable \_scene 

```C++
Scene3D robot_dart::gui::magnum::BaseApplication::_scene;
```






### variable \_shadow\_camera 

```C++
std::unique_ptr<Camera3D> robot_dart::gui::magnum::BaseApplication::_shadow_camera;
```






### variable \_shadow\_camera\_object 

```C++
Object3D* robot_dart::gui::magnum::BaseApplication::_shadow_camera_object;
```






### variable \_shadow\_color\_cube\_map 

```C++
std::unique_ptr<Magnum::GL::CubeMapTextureArray> robot_dart::gui::magnum::BaseApplication::_shadow_color_cube_map;
```






### variable \_shadow\_color\_shader 

```C++
std::unique_ptr<gs::ShadowMapColor> robot_dart::gui::magnum::BaseApplication::_shadow_color_shader;
```






### variable \_shadow\_color\_texture 

```C++
std::unique_ptr<Magnum::GL::Texture2DArray> robot_dart::gui::magnum::BaseApplication::_shadow_color_texture;
```






### variable \_shadow\_cube\_map 

```C++
std::unique_ptr<Magnum::GL::CubeMapTextureArray> robot_dart::gui::magnum::BaseApplication::_shadow_cube_map;
```






### variable \_shadow\_data 

```C++
std::vector<ShadowData> robot_dart::gui::magnum::BaseApplication::_shadow_data;
```






### variable \_shadow\_map\_size 

```C++
int robot_dart::gui::magnum::BaseApplication::_shadow_map_size;
```






### variable \_shadow\_shader 

```C++
std::unique_ptr<gs::ShadowMap> robot_dart::gui::magnum::BaseApplication::_shadow_shader;
```






### variable \_shadow\_texture 

```C++
std::unique_ptr<Magnum::GL::Texture2DArray> robot_dart::gui::magnum::BaseApplication::_shadow_texture;
```






### variable \_shadow\_texture\_color\_shader 

```C++
std::unique_ptr<gs::ShadowMapColor> robot_dart::gui::magnum::BaseApplication::_shadow_texture_color_shader;
```






### variable \_shadow\_texture\_shader 

```C++
std::unique_ptr<gs::ShadowMap> robot_dart::gui::magnum::BaseApplication::_shadow_texture_shader;
```






### variable \_shadowed 

```C++
bool robot_dart::gui::magnum::BaseApplication::_shadowed;
```






### variable \_shadowed\_color\_drawables 

```C++
Magnum::SceneGraph::DrawableGroup3D robot_dart::gui::magnum::BaseApplication::_shadowed_color_drawables;
```






### variable \_shadowed\_drawables 

```C++
Magnum::SceneGraph::DrawableGroup3D robot_dart::gui::magnum::BaseApplication::_shadowed_drawables;
```






### variable \_simu 

```C++
RobotDARTSimu* robot_dart::gui::magnum::BaseApplication::_simu;
```






### variable \_text\_indices 

```C++
Corrade::Containers::Pointer<Magnum::GL::Buffer> robot_dart::gui::magnum::BaseApplication::_text_indices;
```






### variable \_text\_shader 

```C++
std::unique_ptr<Magnum::Shaders::DistanceFieldVectorGL2D> robot_dart::gui::magnum::BaseApplication::_text_shader;
```






### variable \_text\_vertices 

```C++
Corrade::Containers::Pointer<Magnum::GL::Buffer> robot_dart::gui::magnum::BaseApplication::_text_vertices;
```






### variable \_texture\_shader 

```C++
std::unique_ptr<gs::PhongMultiLight> robot_dart::gui::magnum::BaseApplication::_texture_shader;
```






### variable \_transparentSize 

```C++
int robot_dart::gui::magnum::BaseApplication::_transparentSize;
```






### variable \_transparent\_shadows 

```C++
bool robot_dart::gui::magnum::BaseApplication::_transparent_shadows;
```



## Protected Functions Documentation




### function \_gl\_clean\_up 

```C++
void robot_dart::gui::magnum::BaseApplication::_gl_clean_up () 
```






### function \_prepare\_shadows 

```C++
void robot_dart::gui::magnum::BaseApplication::_prepare_shadows () 
```




------------------------------
The documentation for this class was generated from the following file `robot_dart/gui/magnum/base_application.hpp`

