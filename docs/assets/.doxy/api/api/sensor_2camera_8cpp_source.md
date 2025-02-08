

# File camera.cpp

[**File List**](files.md) **>** [**gui**](dir_6a9d4b7ec29c938d1d9a486c655cfc8a.md) **>** [**magnum**](dir_5d18adecbc10cabf3ca51da31f2acdd1.md) **>** [**sensor**](dir_2c74a777547786aaf50e99ba400e19fa.md) **>** [**camera.cpp**](sensor_2camera_8cpp.md)

[Go to the documentation of this file](sensor_2camera_8cpp.md)


```C++
#include "camera.hpp"

#include <Corrade/Containers/ArrayViewStl.h>
#include <Corrade/Containers/StridedArrayView.h>
#include <Corrade/Utility/Algorithms.h>

#include <Magnum/GL/PixelFormat.h>
#include <Magnum/GL/RenderbufferFormat.h>
#include <Magnum/GL/Renderer.h>
#include <Magnum/GL/TextureFormat.h>
#include <Magnum/ImageView.h>
#include <Magnum/PixelFormat.h>

#include <robot_dart/gui/magnum/utils_headers_eigen.hpp>

namespace robot_dart {
    namespace gui {
        namespace magnum {
            namespace sensor {
                Camera::Camera(BaseApplication* app, size_t width, size_t height, size_t freq, bool draw_debug) : robot_dart::sensor::Sensor(freq), _magnum_app(app), _width(width), _height(height), _draw_debug(draw_debug)
                {
                    /* Camera setup */
                    _camera.reset(
                        new gs::Camera(_magnum_app->scene(), static_cast<int>(_width), static_cast<int>(_height)));

                    /* Assume context is given externally, if not, we cannot have a camera */
                    if (!Magnum::GL::Context::hasCurrent()) {
                        Corrade::Utility::Error{} << "GL::Context not provided.. Cannot use this camera!";
                        _camera->record(false, false);
                        _active = false;
                        return;
                    }
                    else
                        _camera->record(true, false);

                    /* Create FrameBuffer to draw */
                    int w = _width, h = _height;
                    _framebuffer = Magnum::GL::Framebuffer({{}, {w, h}});
                    _color.setStorage(Magnum::GL::RenderbufferFormat::RGBA8, {w, h});
                    // _color.setStorageMultisample(8, Magnum::GL::RenderbufferFormat::RGBA8, {w, h});
                    _depth.setStorage(Magnum::GL::RenderbufferFormat::DepthComponent, {w, h});

                    _format = Magnum::PixelFormat::RGB8Unorm;

                    _framebuffer.attachRenderbuffer(
                        Magnum::GL::Framebuffer::ColorAttachment(0), _color);
                    _framebuffer.attachRenderbuffer(
                        Magnum::GL::Framebuffer::BufferAttachment::Depth, _depth);
                }

                void Camera::init()
                {
                    if (_simu)
                        _active = true;
                    // TO-DO: Maybe create a camera configuration structure that gets saved and re-initialized each time
                }

                void Camera::calculate(double)
                {
                    ROBOT_DART_EXCEPTION_ASSERT(_simu, "Simulation pointer is null!");
                    /* Update graphic meshes/materials and render */
                    _magnum_app->update_graphics();
                    /* Update lights transformations --- this also draws the shadows if enabled */
                    _magnum_app->update_lights(*_camera);

                    Magnum::GL::Renderer::enable(Magnum::GL::Renderer::Feature::DepthTest);
                    Magnum::GL::Renderer::enable(Magnum::GL::Renderer::Feature::FaceCulling);
                    Magnum::GL::Renderer::enable(Magnum::GL::Renderer::Feature::Blending);
                    Magnum::GL::Renderer::setBlendFunction(Magnum::GL::Renderer::BlendFunction::SourceAlpha, Magnum::GL::Renderer::BlendFunction::OneMinusSourceAlpha);
                    Magnum::GL::Renderer::setBlendEquation(Magnum::GL::Renderer::BlendEquation::Add);

                    /* Change clear color to black */
                    Magnum::GL::Renderer::setClearColor(Magnum::Vector4{0.f, 0.f, 0.f, 1.f});

                    /* Bind the framebuffer */
                    _framebuffer.bind();
                    /* Clear framebuffer */
                    _framebuffer.clear(Magnum::GL::FramebufferClear::Color | Magnum::GL::FramebufferClear::Depth);

                    /* Draw with this camera */
                    _camera->draw(_magnum_app->drawables(), _framebuffer, _format, _simu, _magnum_app->debug_draw_data(), _draw_debug);
                }

                std::string Camera::type() const { return "rgb_camera"; }

                void Camera::attach_to_body(dart::dynamics::BodyNode* body, const Eigen::Isometry3d& tf)
                {
                    robot_dart::sensor::Sensor::attach_to_body(body, tf);

                    if (_attached_to_body) {
                        if (!_magnum_app->attach_camera(*_camera, body)) {
                            _attaching_to_body = true;
                            _attached_to_body = false;
                            return;
                        }

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

                Eigen::Matrix3d Camera::camera_intrinsic_matrix() const
                {
                    ROBOT_DART_EXCEPTION_ASSERT(_camera, "Camera pointer is null!");
                    return Magnum::EigenIntegration::cast<Eigen::Matrix3d>(Magnum::Matrix3d(_camera->intrinsic_matrix()));
                }

                Eigen::Matrix4d Camera::camera_extrinsic_matrix() const
                {
                    ROBOT_DART_EXCEPTION_ASSERT(_camera, "Camera pointer is null!");
                    return Magnum::EigenIntegration::cast<Eigen::Matrix4d>(Magnum::Matrix4d(_camera->extrinsic_matrix()));
                }

                void Camera::look_at(const Eigen::Vector3d& camera_pos, const Eigen::Vector3d& look_at, const Eigen::Vector3d& up)
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

                    if (_attached_to_body) {
                        Eigen::Matrix4d tr = Magnum::EigenIntegration::cast<Eigen::Matrix4d>(Magnum::Math::Matrix4<double>(_camera->camera_object().transformationMatrix()));

                        _world_pose = Eigen::Isometry3d(tr);

                        _camera->root_object().setParent(&_magnum_app->scene());
                    }
                }

                GrayscaleImage Camera::depth_image()
                {
                    auto& depth_image = _camera->depth_image();
                    if (!depth_image)
                        return GrayscaleImage();

                    return gs::depth_from_image(&*depth_image, true, _camera->near_plane(), _camera->far_plane());
                }

                GrayscaleImage Camera::raw_depth_image()
                {
                    auto& depth_image = _camera->depth_image();
                    if (!depth_image)
                        return GrayscaleImage();

                    return gs::depth_from_image(&*depth_image);
                }

                DepthImage Camera::depth_array()
                {
                    auto& depth_image = _camera->depth_image();
                    if (!depth_image)
                        return DepthImage();

                    return gs::depth_array_from_image(&*depth_image, _camera->near_plane(), _camera->far_plane());
                }
            } // namespace sensor
        } // namespace magnum
    } // namespace gui
} // namespace robot_dart
```


