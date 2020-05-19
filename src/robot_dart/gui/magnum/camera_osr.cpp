#include "camera_osr.hpp"

#include <robot_dart/robot_dart_simu.hpp>

#include <Corrade/Containers/StridedArrayView.h>

// #include <Magnum/DebugTools/Screenshot.h>
#include <Magnum/GL/PixelFormat.h>
#include <Magnum/GL/RenderbufferFormat.h>
#include <Magnum/GL/Renderer.h>
#include <Magnum/GL/TextureFormat.h>
#include <Magnum/ImageView.h>
#include <Magnum/PixelFormat.h>

namespace robot_dart {
    namespace gui {
        namespace magnum {
            CameraOSR::CameraOSR(RobotDARTSimu* simu, BaseApplication* app, size_t width, size_t height, bool draw_ghost)
                : Base(), _magnum_app(app), _enabled(true), _done(false), _draw_ghosts(draw_ghost)
            {
                /* Camera setup */
                _camera.reset(
                    new gs::Camera(app->scene(), static_cast<int>(width), static_cast<int>(height)));

                set_render_period(simu->world()->getTimeStep());

                /* Assume context is given externally, if not, we cannot have a camera */
                if (!Magnum::GL::Context::hasCurrent()) {
                    Corrade::Utility::Error{} << "GL::Context not provided.. Cannot use this camera!";
                    record(false);
                    _done = true;
                    return;
                }
                else
                    record(true);

                /* Create FrameBuffer to draw */
                int w = width, h = height;
                _framebuffer = Magnum::GL::Framebuffer({{}, {w, h}});
                _color.setStorage(Magnum::GL::RenderbufferFormat::RGBA8, {w, h});
                // _color.setStorageMultisample(8, Magnum::GL::RenderbufferFormat::RGBA8, {w, h});
                _depth.setStorage(Magnum::GL::RenderbufferFormat::DepthComponent, {w, h});

                _format = Magnum::PixelFormat::RGBA8Unorm;

                _framebuffer.attachRenderbuffer(
                    Magnum::GL::Framebuffer::ColorAttachment(0), _color);
                _framebuffer.attachRenderbuffer(
                    Magnum::GL::Framebuffer::BufferAttachment::Depth, _depth);
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
                // cameras usually operate at around 30Hz (of simulated time)
                _render_period = std::floor((1. / 30.) / dt);
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

                _camera->look_at(Magnum::Vector3{cx, cy, cz},
                    Magnum::Vector3{lx, ly, lz},
                    Magnum::Vector3{ux, uy, uz});
            }

            void CameraOSR::render()
            {
                /* Update graphic meshes/materials and render */
                _magnum_app->update_graphics();
                /* Update lights transformations */
                _magnum_app->update_lights(*_camera);

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

                /* Draw with this camera */
                _camera->draw(_magnum_app->drawables(), _framebuffer, _format, _draw_ghosts);
            }

            GrayscaleImage CameraOSR::depth_image()
            {
                auto& depth_image = _camera->depth_image();
                if (!depth_image)
                    return GrayscaleImage();
                auto pixels = depth_image->pixels<Magnum::Float>();
                auto sz = pixels.size();

                GrayscaleImage img;
                // TO-DO: Make this more performant
                size_t width = sz[1];
                size_t height = sz[0];
                img.resize(width);
                for (size_t w = 0; w < width; w++) {
                    img[w].resize(height);
                    for (size_t h = 0; h < height; h++) {
                        Magnum::Float depth = pixels[height - 1 - h][w];

                        /* Linearize depth for visualization */
                        Magnum::Float zNear = _camera->near_plane();
                        Magnum::Float zFar = _camera->far_plane();
                        Magnum::Float val = (2.f * zNear) / (zFar + zNear - depth * (zFar - zNear));
                        img[w][h] = val * 255.f;
                    }
                }

                return img;
            }

            GrayscaleImage CameraOSR::raw_depth_image()
            {
                auto& depth_image = _camera->depth_image();
                if (!depth_image)
                    return GrayscaleImage();
                auto pixels = depth_image->pixels<Magnum::Float>();
                auto sz = pixels.size();

                GrayscaleImage img;
                // TO-DO: Make this more performant
                size_t width = sz[1];
                size_t height = sz[0];
                img.resize(width);
                for (size_t w = 0; w < width; w++) {
                    img[w].resize(height);
                    for (size_t h = 0; h < height; h++) {
                        Magnum::Float depth = pixels[height - 1 - h][w];
                        img[w][h] = depth * 255.f;
                    }
                }

                return img;
            }

            // std::vector<Eigen::Vector3d> CameraOSR::point_cloud()
            // {
            //     Magnum::Image2D depth_image = _camera->depth_image();
            //     auto pixels = depth_image.pixels<Magnum::Float>();
            //     auto sz = pixels.size();

            //     auto inv_cam_proj = _camera->camera().projectionMatrix().inverted();

            //     std::vector<Eigen::Vector3d> points;
            //     // TO-DO: Make this more performant
            //     size_t width = sz[1];
            //     size_t height = sz[0];
            //     for (size_t w = 0; w < width; w++) {
            //         for (size_t h = 0; h < height; h++) {
            //             Magnum::Float depth = pixels[height - 1 - h][w];
            //             Magnum::Float tX = 2.f * (w / Magnum::Float(width - 1)) - 1.f;
            //             Magnum::Float tY = 2.f * (h / Magnum::Float(height - 1)) - 1.f;
            //             Magnum::Float tZ = 2.f * depth - 1.f;
            //             Magnum::Vector4 vec{tX, tY, tZ, 1.f};
            //             Magnum::Vector4 v = (inv_cam_proj * vec);
            //             Magnum::Vector3 point = v.xyz() / v.w();

            //             points.push_back(Eigen::Vector3d(point.x(), point.y(), point.z()));
            //         }
            //     }

            //     return points;
            // }

            void CameraOSR::attach_to(const std::string& name, const Eigen::Isometry3d& tf)
            {
                _attach_to = name;
                _attaching = true;
                _attached = false;
                _attached_tf = tf;

                _attached = _magnum_app->attach_camera(*_camera, name);
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
                    _camera->camera_object().setTransformation({});
                    _camera->camera_object().rotate(theta, u).translate(t);
                }
            }
        } // namespace magnum
    } // namespace gui
} // namespace robot_dart
