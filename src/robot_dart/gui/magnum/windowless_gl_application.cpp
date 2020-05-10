#include "windowless_gl_application.hpp"

// #include <Magnum/DebugTools/Screenshot.h>
#include <Magnum/GL/RenderbufferFormat.h>
#include <Magnum/GL/Renderer.h>

namespace robot_dart {
    namespace gui {
        namespace magnum {
            WindowlessGLApplication::WindowlessGLApplication(int argc, char** argv, const dart::simulation::WorldPtr& world, size_t width, size_t height, const std::string& title, bool isShadowed, bool drawTransparentShadows)
                : BaseApplication(isShadowed, drawTransparentShadows), Magnum::Platform::WindowlessApplication({argc, argv}, Magnum::NoCreate)
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
                int w = width, h = height;
                _framebuffer = Magnum::GL::Framebuffer({{}, {w, h}});
                _color = Magnum::GL::Renderbuffer();
                _depth = Magnum::GL::Renderbuffer();
                _color.setStorage(Magnum::GL::RenderbufferFormat::RGBA8, {w, h});
                _depth.setStorage(Magnum::GL::RenderbufferFormat::DepthComponent, {w, h});

                _format = Magnum::PixelFormat::RGBA8Unorm;

                _framebuffer.attachRenderbuffer(
                    Magnum::GL::Framebuffer::ColorAttachment(0), _color);
                _framebuffer.attachRenderbuffer(
                    Magnum::GL::Framebuffer::BufferAttachment::Depth, _depth);

                /* Initialize DART world */
                init(world, width, height);

                record(true);
            }

            WindowlessGLApplication::~WindowlessGLApplication()
            {
                GLCleanUp();
            }

            void WindowlessGLApplication::render()
            {
                /* Update graphic meshes/materials and render */
                updateGraphics();
                /* Update lights transformations */
                updateLights(*_camera);

                Magnum::GL::Renderer::enable(Magnum::GL::Renderer::Feature::DepthTest);
                Magnum::GL::Renderer::enable(Magnum::GL::Renderer::Feature::FaceCulling);
                Magnum::GL::Renderer::enable(Magnum::GL::Renderer::Feature::Blending);
                Magnum::GL::Renderer::setBlendFunction(Magnum::GL::Renderer::BlendFunction::SourceAlpha, Magnum::GL::Renderer::BlendFunction::OneMinusSourceAlpha);
                Magnum::GL::Renderer::setBlendEquation(Magnum::GL::Renderer::BlendEquation::Add);

                /* Change default clear color to black */
                Magnum::GL::Renderer::setClearColor(Magnum::Vector4{0.f, 0.f, 0.f, 1.f});

                /* Bind the framebuffer */
                _framebuffer.bind();
                /* Clear framebuffer */
                _framebuffer.clear(Magnum::GL::FramebufferClear::Color | Magnum::GL::FramebufferClear::Depth);

                /* Draw with main camera */
                _camera->draw(_drawables, _framebuffer, _format);

                // if (_index % 10 == 0) {
                //     intptr_t tt = (intptr_t)_glx_context;
                //     Magnum::DebugTools::screenshot(_framebuffer, "img_" + std::to_string(tt) + "_" + std::to_string(_index) + ".png");
                // }

                // _index++;
            }
        } // namespace magnum
    } // namespace gui
} // namespace robot_dart