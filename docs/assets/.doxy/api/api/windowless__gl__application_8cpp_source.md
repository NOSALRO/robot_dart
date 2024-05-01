

# File windowless\_gl\_application.cpp

[**File List**](files.md) **>** [**gui**](dir_6a9d4b7ec29c938d1d9a486c655cfc8a.md) **>** [**magnum**](dir_5d18adecbc10cabf3ca51da31f2acdd1.md) **>** [**windowless\_gl\_application.cpp**](windowless__gl__application_8cpp.md)

[Go to the documentation of this file](windowless__gl__application_8cpp.md)


```C++
#include "windowless_gl_application.hpp"

// #include <Magnum/DebugTools/Screenshot.h>
#include <Magnum/GL/RenderbufferFormat.h>
#include <Magnum/GL/Renderer.h>

namespace robot_dart {
    namespace gui {
        namespace magnum {
            WindowlessGLApplication::WindowlessGLApplication(int argc, char** argv, RobotDARTSimu* simu, const GraphicsConfiguration& configuration)
                : BaseApplication(configuration),
                  Magnum::Platform::WindowlessApplication({argc, argv}, Magnum::NoCreate),
                  _simu(simu),
                  _draw_main_camera(configuration.draw_main_camera),
                  _draw_debug(configuration.draw_debug),
                  _bg_color(configuration.bg_color[0],
                      configuration.bg_color[1],
                      configuration.bg_color[2],
                      configuration.bg_color[3])
            {
                /* Assume context is given externally, if not create it */
                if (!Magnum::GL::Context::hasCurrent()) {
                    Corrade::Utility::Debug{} << "GL::Context not provided. Creating...";
                    if (!tryCreateContext(Configuration())) {
                        Corrade::Utility::Error{} << "Could not create context!";
                        return;
                    }
                }
                // else
                // Corrade::Utility::Debug{} << "Created context with: " << Magnum::GL::Context::current().versionString();

                /* Create FrameBuffer to draw */
                int w = configuration.width, h = configuration.height;
                _framebuffer = Magnum::GL::Framebuffer({{}, {w, h}});
                _color = Magnum::GL::Renderbuffer();
                _depth = Magnum::GL::Renderbuffer();
                _color.setStorage(Magnum::GL::RenderbufferFormat::RGBA8, {w, h});
                _depth.setStorage(Magnum::GL::RenderbufferFormat::DepthComponent, {w, h});

                _format = Magnum::PixelFormat::RGB8Unorm;

                _framebuffer.attachRenderbuffer(
                    Magnum::GL::Framebuffer::ColorAttachment(0), _color);
                _framebuffer.attachRenderbuffer(
                    Magnum::GL::Framebuffer::BufferAttachment::Depth, _depth);

                /* Initialize DART world */
                init(simu, configuration);

                _camera->record(true);
            }

            WindowlessGLApplication::~WindowlessGLApplication()
            {
                _gl_clean_up();
            }

            void WindowlessGLApplication::render()
            {
                if (_draw_main_camera) {
                    /* Update graphic meshes/materials and render */
                    update_graphics();
                    /* Update lights transformations --- this also draws the shadows if enabled */
                    update_lights(*_camera);

                    Magnum::GL::Renderer::enable(Magnum::GL::Renderer::Feature::DepthTest);
                    Magnum::GL::Renderer::enable(Magnum::GL::Renderer::Feature::FaceCulling);
                    Magnum::GL::Renderer::enable(Magnum::GL::Renderer::Feature::Blending);
                    Magnum::GL::Renderer::setBlendFunction(Magnum::GL::Renderer::BlendFunction::SourceAlpha, Magnum::GL::Renderer::BlendFunction::OneMinusSourceAlpha);
                    Magnum::GL::Renderer::setBlendEquation(Magnum::GL::Renderer::BlendEquation::Add);

                    /* Change clear color to _bg_color */
                    Magnum::GL::Renderer::setClearColor(_bg_color);

                    /* Bind the framebuffer */
                    _framebuffer.bind();
                    /* Clear framebuffer */
                    _framebuffer.clear(Magnum::GL::FramebufferClear::Color | Magnum::GL::FramebufferClear::Depth);

                    /* Draw with main camera */
                    _camera->draw(_drawables, _framebuffer, _format, _simu, debug_draw_data(), _draw_debug);

                    // if (_index % 10 == 0) {
                    //     intptr_t tt = (intptr_t)_glx_context;
                    //     Magnum::DebugTools::screenshot(_framebuffer, "img_" + std::to_string(tt) + "_" + std::to_string(_index) + ".png");
                    // }

                    // _index++;
                }
            }
        } // namespace magnum
    } // namespace gui
} // namespace robot_dart
```


