#include "camera_osr.hpp"

// #include <Magnum/DebugTools/Screenshot.h>
#include <Magnum/GL/Renderbuffer.h>
#include <Magnum/GL/RenderbufferFormat.h>
#include <Magnum/GL/Renderer.h>

namespace robot_dart {
    namespace gui {
        namespace magnum {
            CameraOSR::CameraOSR(const dart::simulation::WorldPtr& world, BaseApplication* app, size_t width, size_t height)
                : Base(nullptr), _magnum_app(app), _enabled(true), _done(false)
            {
                /* Camera setup */
                _camera.reset(
                    new gs::Camera(app->scene(), static_cast<int>(width), static_cast<int>(height)));

                set_render_period(world->getTimeStep());

                /* Assume context is given externally, if not, we cannot have a camera */
                if (!Magnum::GL::Context::hasCurrent()) {
                    Corrade::Utility::Error{} << "GL::Context not provided.. Cannot use this camera!";
                    set_recording(false);
                    _done = true;
                    return;
                }
                else
                    set_recording(true);

                /* Create FrameBuffer to draw */
                int w = width, h = height;
                _framebuffer = Magnum::GL::Framebuffer({{}, {w, h}});
                Magnum::GL::Renderbuffer color, depth;
                color.setStorage(Magnum::GL::RenderbufferFormat::RGBA8, {w, h});
                // color.setStorageMultisample(8, Magnum::GL::RenderbufferFormat::RGBA8, {w, h});
                depth.setStorage(Magnum::GL::RenderbufferFormat::DepthComponent, {w, h});

                _format = Magnum::PixelFormat::RGBA8Unorm;

                _framebuffer.attachRenderbuffer(
                    Magnum::GL::Framebuffer::ColorAttachment(0), color);
                _framebuffer.attachRenderbuffer(
                    Magnum::GL::Framebuffer::BufferAttachment::Depth, depth);
            }

            void CameraOSR::render()
            {
                Magnum::GL::Renderer::enable(Magnum::GL::Renderer::Feature::DepthTest);
                Magnum::GL::Renderer::enable(Magnum::GL::Renderer::Feature::FaceCulling);

                /* Change default clear color to black */
                Magnum::GL::Renderer::setClearColor(Magnum::Vector4{0.f, 0.f, 0.f, 1.f});

                /* Bind the framebuffer */
                _framebuffer.bind();
                /* Clear framebuffer */
                _framebuffer.clear(Magnum::GL::FramebufferClear::Color | Magnum::GL::FramebufferClear::Depth);

                /* Update graphic meshes/materials and render */
                _magnum_app->updateGraphics();
                /* Update lights transformations */
                _magnum_app->updateLights(*_camera);
                /* Draw with this camera */
                _camera->draw(_magnum_app->drawables(), _framebuffer, _format);
            }
        } // namespace magnum
    } // namespace gui
} // namespace robot_dart