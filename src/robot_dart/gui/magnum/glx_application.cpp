#include "glx_application.hpp"

// #include <Magnum/DebugTools/Screenshot.h>
#include <Magnum/GL/Renderbuffer.h>
#include <Magnum/GL/RenderbufferFormat.h>
#include <Magnum/GL/Renderer.h>

namespace robot_dart {
    namespace gui {
        namespace magnum {
            GLXApplication::GLXApplication(int argc, char** argv, const dart::simulation::WorldPtr& world, size_t width, size_t height, const std::string& title)
                : BaseApplication(), Magnum::Platform::WindowlessApplication({argc, argv}, Magnum::NoCreate)
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
                Magnum::GL::Renderbuffer color, depth;
                color.setStorage(Magnum::GL::RenderbufferFormat::RGBA8, {w, h});
                depth.setStorage(Magnum::GL::RenderbufferFormat::DepthComponent, {w, h});

                _framebuffer.attachRenderbuffer(
                    Magnum::GL::Framebuffer::ColorAttachment(0), color);
                _framebuffer.attachRenderbuffer(
                    Magnum::GL::Framebuffer::BufferAttachment::Depth, depth);

                Magnum::GL::Renderer::enable(Magnum::GL::Renderer::Feature::DepthTest);
                Magnum::GL::Renderer::enable(Magnum::GL::Renderer::Feature::FaceCulling);

                /* Initialize DART world */
                this->init(world);

                /* Change default clear color to black */
                Magnum::GL::Renderer::setClearColor(Magnum::Vector4{0.f, 0.f, 0.f, 1.f});
            }

            GLXApplication::~GLXApplication()
            {
                GLCleanUp();
            }

            void GLXApplication::render()
            {
                /* Clear framebuffer */
                _framebuffer.clear(Magnum::GL::FramebufferClear::Color | Magnum::GL::FramebufferClear::Depth);
                /* Bind the framebuffer */
                _framebuffer.bind();

                /* Update graphic meshes/materials and render */
                updateGraphics();
                /* Update lights transformations */
                updateLights();
                _camera->camera().draw(_drawables);

                // if (_index % 10 == 0) {
                //     intptr_t tt = (intptr_t)_glx_context;
                //     Magnum::DebugTools::screenshot(_framebuffer, "img_" + std::to_string(tt) + "_" + std::to_string(_index) + ".png");
                // }

                // _index++;
            }
        } // namespace magnum
    } // namespace gui
} // namespace robot_dart