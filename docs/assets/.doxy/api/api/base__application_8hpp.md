

# File base\_application.hpp



[**FileList**](files.md) **>** [**gui**](dir_6a9d4b7ec29c938d1d9a486c655cfc8a.md) **>** [**magnum**](dir_5d18adecbc10cabf3ca51da31f2acdd1.md) **>** [**base\_application.hpp**](base__application_8hpp.md)

[Go to the source code of this file](base__application_8hpp_source.md)



* `#include <mutex>`
* `#include <unistd.h>`
* `#include <unordered_map>`
* `#include <robot_dart/gui/helper.hpp>`
* `#include <robot_dart/gui/magnum/drawables.hpp>`
* `#include <robot_dart/gui/magnum/gs/camera.hpp>`
* `#include <robot_dart/gui/magnum/gs/cube_map.hpp>`
* `#include <robot_dart/gui/magnum/gs/cube_map_color.hpp>`
* `#include <robot_dart/gui/magnum/gs/phong_multi_light.hpp>`
* `#include <robot_dart/gui/magnum/gs/shadow_map.hpp>`
* `#include <robot_dart/gui/magnum/gs/shadow_map_color.hpp>`
* `#include <robot_dart/gui/magnum/types.hpp>`
* `#include <robot_dart/utils_headers_external_gui.hpp>`
* `#include <Magnum/GL/CubeMapTextureArray.h>`
* `#include <Magnum/GL/Framebuffer.h>`
* `#include <Magnum/GL/Mesh.h>`
* `#include <Magnum/GL/TextureArray.h>`
* `#include <Magnum/Platform/GLContext.h>`
* `#include <Magnum/Platform/WindowlessEglApplication.h>`
* `#include <Magnum/Shaders/DistanceFieldVector.h>`
* `#include <Magnum/Shaders/Flat.h>`
* `#include <Magnum/Shaders/VertexColor.h>`
* `#include <Magnum/Text/AbstractFont.h>`
* `#include <Magnum/Text/DistanceFieldGlyphCache.h>`
* `#include <Magnum/Text/Renderer.h>`













## Namespaces

| Type | Name |
| ---: | :--- |
| namespace | [**robot\_dart**](namespacerobot__dart.md) <br> |
| namespace | [**gui**](namespacerobot__dart_1_1gui.md) <br> |
| namespace | [**magnum**](namespacerobot__dart_1_1gui_1_1magnum.md) <br> |


## Classes

| Type | Name |
| ---: | :--- |
| class | [**BaseApplication**](classrobot__dart_1_1gui_1_1magnum_1_1BaseApplication.md) <br> |
| struct | [**DebugDrawData**](structrobot__dart_1_1gui_1_1magnum_1_1DebugDrawData.md) <br> |
| struct | [**GlobalData**](structrobot__dart_1_1gui_1_1magnum_1_1GlobalData.md) <br> |
| struct | [**GraphicsConfiguration**](structrobot__dart_1_1gui_1_1magnum_1_1GraphicsConfiguration.md) <br> |

















































## Macros

| Type | Name |
| ---: | :--- |
| define  | [**get\_gl\_context**](base__application_8hpp.md#define-get_gl_context) (name) get\_gl\_context\_with\_sleep(name, 0)<br> |
| define  | [**get\_gl\_context\_with\_sleep**](base__application_8hpp.md#define-get_gl_context_with_sleep) (name, ms\_sleep) <br> |
| define  | [**release\_gl\_context**](base__application_8hpp.md#define-release_gl_context) (name) robot\_dart::gui::magnum::GlobalData::instance()-&gt;free\_gl\_context(name);<br> |

## Macro Definition Documentation





### define get\_gl\_context 

```C++
#define get_gl_context (
    name
) get_gl_context_with_sleep(name, 0)
```




<hr>



### define get\_gl\_context\_with\_sleep 

```C++
#define get_gl_context_with_sleep (
    name,
    ms_sleep
) /* Create/Get GLContext */                                                \
    Corrade::Utility::Debug name##_magnum_silence_output{nullptr};            \
    Magnum::Platform::WindowlessGLContext* name = nullptr;                    \
    while (name == nullptr) {                                                 \
        name = robot_dart::gui::magnum::GlobalData::instance()->gl_context(); \
        /* Sleep for some ms */                                               \
        usleep(ms_sleep * 1000);                                              \
    }                                                                         \
    while (!name->makeCurrent()) {                                            \
        /* Sleep for some ms */                                               \
        usleep(ms_sleep * 1000);                                              \
    }                                                                         \
                                                                              \
    Magnum::Platform::GLContext name##_magnum_context;
```




<hr>



### define release\_gl\_context 

```C++
#define release_gl_context (
    name
) robot_dart::gui::magnum::GlobalData::instance()->free_gl_context(name);
```




<hr>

------------------------------
The documentation for this class was generated from the following file `robot_dart/gui/magnum/base_application.hpp`

