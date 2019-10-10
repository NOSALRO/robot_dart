#include "camera_osr.hpp"

// #include <Magnum/DebugTools/Screenshot.h>
#include <Magnum/GL/Renderbuffer.h>
#include <Magnum/GL/RenderbufferFormat.h>
#include <Magnum/GL/Renderer.h>

namespace robot_dart {
    namespace gui {
        namespace magnum {
            CameraOSR::CameraOSR(const dart::simulation::WorldPtr& world, BaseApplication* app, size_t width, size_t height)
                : Base(), _magnum_app(app), _enabled(true), _done(false)
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

            void CameraOSR::refresh()
            {
                if (!_enabled)
                    return;

                if (_attaching && !_attached) {
                    attach_to(_attach_to, _attached_tf);
                }

                // process next frame
                if (_frame_counter % _render_period == 0)
                    render();
                _frame_counter++;
            }

            void CameraOSR::set_render_period(double dt)
            {
                // we want to display at around 60Hz of simulated time
                _render_period = std::floor((1. / 60.) / dt);
                if (_render_period < 1)
                    _render_period = 1;
            }

            void CameraOSR::look_at(const Eigen::Vector3d& camera_pos, const Eigen::Vector3d& look_at, const Eigen::Vector3d& up)
            {
                float cx = static_cast<float>(camera_pos[0]);
                float cy = static_cast<float>(camera_pos[1]);
                float cz = static_cast<float>(camera_pos[2]);

                float lx = static_cast<float>(look_at[0]);
                float ly = static_cast<float>(look_at[1]);
                float lz = static_cast<float>(look_at[2]);

                float ux = static_cast<float>(up[0]);
                float uy = static_cast<float>(up[1]);
                float uz = static_cast<float>(up[2]);

                _camera->lookAt(Magnum::Vector3{cx, cy, cz},
                    Magnum::Vector3{lx, ly, lz},
                    Magnum::Vector3{ux, uy, uz});
            }

            void CameraOSR::render()
            {
                /* Update graphic meshes/materials and render */
                _magnum_app->updateGraphics();
                /* Update lights transformations */
                _magnum_app->updateLights(*_camera);

                Magnum::GL::Renderer::enable(Magnum::GL::Renderer::Feature::DepthTest);
                Magnum::GL::Renderer::enable(Magnum::GL::Renderer::Feature::FaceCulling);

                /* Change default clear color to black */
                Magnum::GL::Renderer::setClearColor(Magnum::Vector4{0.f, 0.f, 0.f, 1.f});

                /* Bind the framebuffer */
                _framebuffer.bind();
                /* Clear framebuffer */
                _framebuffer.clear(Magnum::GL::FramebufferClear::Color | Magnum::GL::FramebufferClear::Depth);

                /* Draw with this camera */
                _camera->draw(_magnum_app->drawables(), _framebuffer, _format);
            }

            void CameraOSR::attach_to(const std::string& name, const Eigen::Isometry3d& tf)
            {
                _attach_to = name;
                _attaching = true;
                _attached = false;
                _attached_tf = tf;

                _attached = _magnum_app->attachCamera(*_camera, name);
                if (_attached) {
                    _attaching = false;

                    Eigen::Quaterniond quat(tf.linear());
                    Eigen::Vector3d axis(quat.x(), quat.y(), quat.z());
                    double angle = 2. * std::acos(quat.w());
                    if (std::abs(angle) > 1e-5) {
                        axis = axis.array() / std::sqrt(1 - quat.w() * quat.w());
                        axis.normalize();
                    }
                    else
                        axis(0) = 1.;

                    Eigen::Vector3d T = tf.translation();

                    /* Convert it to axis-angle representation */
                    Magnum::Math::Vector3<Magnum::Float> t(T[0], T[1], T[2]);
                    Magnum::Math::Vector3<Magnum::Float> u(axis(0), axis(1), axis(2));
                    Magnum::Rad theta(angle);

                    /* Pass it to Magnum */
                    _camera->cameraObject().setTransformation({});
                    _camera->cameraObject().rotate(theta, u).translate(t);
                }
            }
        } // namespace magnum
    } // namespace gui
} // namespace robot_dart