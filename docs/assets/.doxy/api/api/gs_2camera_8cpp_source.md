

# File camera.cpp

[**File List**](files.md) **>** [**gs**](dir_2f8612d80f6bb57c97efd4c82e0df286.md) **>** [**camera.cpp**](gs_2camera_8cpp.md)

[Go to the documentation of this file](gs_2camera_8cpp.md)

```C++

#include "camera.hpp"
#include "robot_dart/gui/magnum/base_application.hpp"
#include "robot_dart/gui_data.hpp"
#include "robot_dart/robot_dart_simu.hpp"
#include "robot_dart/utils.hpp"

#include <external/subprocess.hpp>

#include <algorithm>

#include <Corrade/Containers/ArrayViewStl.h>
#include <Corrade/Containers/StridedArrayView.h>
#include <Corrade/Utility/Algorithms.h>

#include <Magnum/GL/AbstractFramebuffer.h>
#include <Magnum/GL/GL.h>
#include <Magnum/GL/PixelFormat.h>
#include <Magnum/GL/Renderer.h>
#include <Magnum/ImageView.h>
#include <Magnum/PixelFormat.h>

#include <robot_dart/gui/magnum/utils_headers_eigen.hpp>
#include <utheque/utheque.hpp>

namespace robot_dart {
    namespace gui {
        namespace magnum {
            namespace gs {

                static fs::path search_path(const fs::path& filename)
                {
                    std::string path = std::getenv("PATH");
                    std::string delimiter = ":";

                    size_t pos = 0;
                    std::string token;

                    std::vector<fs::path> all_paths;
                    while ((pos = path.find(delimiter)) != std::string::npos) {
                        token = path.substr(0, pos);
                        if (token.size() > 0)
                            all_paths.push_back(token);
                        path.erase(0, pos + delimiter.length());
                    }
                    if (path.size() > 0)
                        all_paths.push_back(path);

                    for (const fs::path& pp : all_paths) {
                        auto p = pp / filename;
                        std::error_code ec;
                        if (fs::is_regular_file(p, ec))
                            return p;
                    }

                    return "";
                }

                Camera::Camera(Object3D& object, Magnum::Int width, Magnum::Int height) : Object3D{&object}
                {
                    _yaw_object = new Object3D{this};
                    _pitch_object = new Object3D{_yaw_object};

                    _yaw_object->setTransformation(Magnum::Matrix4{});
                    _pitch_object->setTransformation(Magnum::Matrix4{});

                    Magnum::Vector3 center{0., 0., 0.};
                    Magnum::Vector3 camera{0., 2., 1.};
                    _front = (center - camera).normalized();
                    _up = Magnum::Vector3::zAxis();
                    _right = Magnum::Math::cross(_front, _up).normalized();

                    _camera_object = new Object3D{_pitch_object};
                    _camera_object->setTransformation(Magnum::Matrix4::lookAt(camera, center, _up));

                    _fov = Magnum::Deg(60.0f);
                    _aspect_ratio = width / static_cast<Magnum::Float>(height);
                    _near_plane = 0.01f;
                    _far_plane = 200.f;
                    _width = width;
                    _height = height;

                    _camera = new Camera3D{*_camera_object};
                    _camera->setAspectRatioPolicy(Magnum::SceneGraph::AspectRatioPolicy::Extend)
                        .setProjectionMatrix(Magnum::Matrix4::perspectiveProjection(_fov, _aspect_ratio, _near_plane, _far_plane))
                        .setViewport({width, height});
                }

                Camera::~Camera()
                {
                    _clean_up_subprocess();
                }

                Camera3D& Camera::camera() const
                {
                    return *_camera;
                }

                Object3D& Camera::root_object()
                {
                    return *this;
                }

                Object3D& Camera::camera_object() const
                {
                    return *_camera_object;
                }

                Camera& Camera::set_viewport(const Magnum::Vector2i& size)
                {
                    _width = size[0];
                    _height = size[1];
                    _aspect_ratio = size[0] / static_cast<Magnum::Float>(size[1]);
                    _camera->setProjectionMatrix(Magnum::Matrix4::perspectiveProjection(_fov, _aspect_ratio, _near_plane, _far_plane))
                        .setViewport(size);

                    return *this;
                }

                Camera& Camera::move(const Magnum::Vector2i& shift)
                {
                    Magnum::Vector2 s = Magnum::Vector2{shift} * _speed;

                    _yaw_object->rotate(Magnum::Rad(s.x()), _up);
                    _pitch_object->rotate(Magnum::Rad(s.y()), _right);

                    return *this;
                }

                Camera& Camera::forward(Magnum::Float speed)
                {
                    _camera_object->translate(speed * _front);

                    return *this;
                }

                Camera& Camera::strafe(Magnum::Float speed)
                {
                    _camera_object->translate(speed * _right);

                    return *this;
                }

                Camera& Camera::set_speed(const Magnum::Vector2& speed)
                {
                    _speed = speed;
                    return *this;
                }

                Camera& Camera::set_near_plane(Magnum::Float near_plane)
                {
                    _near_plane = near_plane;
                    _camera->setProjectionMatrix(Magnum::Matrix4::perspectiveProjection(_fov, _aspect_ratio, _near_plane, _far_plane));

                    return *this;
                }

                Camera& Camera::set_far_plane(Magnum::Float far_plane)
                {
                    _far_plane = far_plane;
                    _camera->setProjectionMatrix(Magnum::Matrix4::perspectiveProjection(_fov, _aspect_ratio, _near_plane, _far_plane));

                    return *this;
                }

                Camera& Camera::set_fov(Magnum::Float fov)
                {
                    // Maximum FOV is around 170 degrees
                    _fov = Magnum::Rad(std::max(0.f, std::min(3.f, fov)));
                    _camera->setProjectionMatrix(Magnum::Matrix4::perspectiveProjection(_fov, _aspect_ratio, _near_plane, _far_plane));

                    return *this;
                }

                Camera& Camera::set_camera_params(Magnum::Float near_plane, Magnum::Float far_plane, Magnum::Float fov, Magnum::Int width, Magnum::Int height)
                {
                    _near_plane = near_plane;
                    _far_plane = far_plane;
                    // Maximum FOV is around 170 degrees
                    _fov = Magnum::Rad(std::max(0.f, std::min(3.f, fov)));
                    _aspect_ratio = width / static_cast<Magnum::Float>(height);
                    _width = width;
                    _height = height;

                    _camera->setProjectionMatrix(Magnum::Matrix4::perspectiveProjection(_fov, _aspect_ratio, _near_plane, _far_plane))
                        .setViewport({width, height});

                    return *this;
                }

                Magnum::Matrix3 Camera::intrinsic_matrix() const
                {
                    // This function returns the intrinsic matrix as if it was a normal camera (pointing to +Z), not an OpenGL camera (pointing to -Z)
                    // even if the camera is pointing towards -Z. This should be appropriately handled by the user.
                    // TO-DO: Make this represent the correct intrinsic matrix. See http://ksimek.github.io/2013/06/03/calibrated_cameras_in_opengl/
                    Magnum::Matrix4 projection = _camera->projectionMatrix() * Magnum::Matrix4::orthographicProjection({static_cast<float>(_width), static_cast<float>(_height)}, _near_plane, _far_plane).inverted();
                    return {{projection[0][0], 0., 0.},
                        {projection[1][0], projection[1][1], 0.},
                        {_width / 2.f, _height / 2.f, 1.}};
                }

                Magnum::Matrix4 Camera::extrinsic_matrix() const
                {
                    return _camera->cameraMatrix();
                }

                Camera& Camera::look_at(const Magnum::Vector3& camera, const Magnum::Vector3& center, const Magnum::Vector3& up)
                {
                    _front = (center - camera).normalized();
                    _up = up;
                    _right = Magnum::Math::cross(_front, _up).normalized();

                    _camera_object->setTransformation(Magnum::Matrix4::lookAt(camera, center, up));
                    _yaw_object->setTransformation(Magnum::Matrix4{});
                    _pitch_object->setTransformation(Magnum::Matrix4{});

                    return *this;
                }

                void Camera::transform_lights(std::vector<gs::Light>& lights) const
                {
                    /* Update lights transformations */
                    for (size_t i = 0; i < lights.size(); i++) {
                        Magnum::Vector4 old_pos = lights[i].position();
                        Magnum::Vector3 pos;
                        /* Directional lights need only rotational transformation */
                        if (lights[i].position().w() == 0.f)
                            pos = _camera->cameraMatrix().transformVector(old_pos.xyz());
                        /* Other light types, need full transformation */
                        else
                            pos = _camera->cameraMatrix().transformPoint(old_pos.xyz());
                        lights[i].set_transformed_position(Magnum::Vector4{pos, old_pos.w()});
                        /* Transform spotlight direction */
                        lights[i].set_transformed_spot_direction(_camera->cameraMatrix().transformVector(lights[i].spot_direction()));
                    }
                }

                void Camera::record_video(const std::string& video_fname, int fps)
                {
                    // first clean up previous recording if necessary
                    _clean_up_subprocess();

                    // search for ffmpeg
                    fs::path ffmpeg = search_path("ffmpeg");
                    if (ffmpeg.empty()) {
                        ROBOT_DART_WARNING(ffmpeg.empty(), "ffmpeg not found in the PATH. RobotDART will not be able to record videos!");
                        return;
                    }

                    // std::cout << "Found FFMPEG:" << ffmpeg << std::endl;
                    _recording_video = true;
                    // list our options
                    std::vector<std::string> args = {"-y",
                        "-f", "rawvideo",
                        "-vcodec", "rawvideo",
                        "-s", std::to_string(width()) + 'x' + std::to_string(height()),
                        "-pix_fmt", "rgb24",
                        "-r", std::to_string(fps),
                        "-i", "-",
                        "-an",
                        "-vcodec", "mpeg4",
                        "-vb", "20M",
                        video_fname};

                    _ffmpeg_process = new subprocess::popen(ffmpeg, args);
                }

                void Camera::draw(Magnum::SceneGraph::DrawableGroup3D& drawables, Magnum::GL::AbstractFramebuffer& framebuffer, Magnum::PixelFormat format, RobotDARTSimu* simu, const DebugDrawData& debug_data, bool draw_debug)
                {
                    // TO-DO: Maybe check if world moved?
                    std::vector<std::pair<std::reference_wrapper<Magnum::SceneGraph::Drawable3D>, Magnum::Matrix4>>
                        drawableTransformations = _camera->drawableTransformations(drawables);

                    std::vector<std::pair<std::reference_wrapper<Magnum::SceneGraph::Drawable3D>, Magnum::Matrix4>> opaque, transparent;
                    for (size_t i = 0; i < drawableTransformations.size(); i++) {
                        auto& obj = static_cast<DrawableObject&>(drawableTransformations[i].first.get().object());
                        if (!draw_debug && simu->gui_data()->ghost(obj.shape()))
                            continue;
                        if (obj.transparent())
                            transparent.emplace_back(drawableTransformations[i]);
                        else
                            opaque.emplace_back(drawableTransformations[i]);
                    }

                    _camera->draw(opaque);
                    if (transparent.size() > 0) {
                        std::sort(transparent.begin(), transparent.end(),
                            [](const std::pair<std::reference_wrapper<Magnum::SceneGraph::Drawable3D>, Magnum::Matrix4>& a,
                                const std::pair<std::reference_wrapper<Magnum::SceneGraph::Drawable3D>, Magnum::Matrix4>& b) {
                                return a.second.translation().z() < b.second.translation().z();
                            });

                        _camera->draw(transparent);
                    }

                    /* Draw debug */
                    if (draw_debug) {
                        /* Draw axes */
                        std::vector<std::pair<dart::dynamics::BodyNode*, double>> axes = simu->gui_data()->drawing_axes();
                        for (auto& axis : axes) {
                            Magnum::Matrix4 world_transform = Magnum::Matrix4(Magnum::Matrix4d(axis.first->getWorldTransform().matrix()));
                            Magnum::Matrix4 scaling = Magnum::Matrix4::scaling(Magnum::Vector3(axis.second, axis.second, axis.second));

                            debug_data.axes_shader->setTransformationProjectionMatrix(_camera->projectionMatrix() * _camera->cameraMatrix() * world_transform * scaling)
                                .draw(*debug_data.axes_mesh);
                        }

                        /* Draw text */
                        if (debug_data.text_shader && debug_data.text_vertices) {
                            using namespace Magnum::Math::Literals;
                            Magnum::GL::Renderer::disable(Magnum::GL::Renderer::Feature::DepthTest);
                            Magnum::GL::Renderer::disable(Magnum::GL::Renderer::Feature::FaceCulling);

                            for (auto& text : simu->gui_data()->drawing_texts()) {
                                if (text->text.empty()) // ignore empty strings
                                    continue;

                                Magnum::GL::Mesh mesh{Magnum::NoCreate};
                                Magnum::Range2D rectangle;
                                double fnt_size = text->font_size;
                                if (fnt_size <= 0.)
                                    fnt_size = 28.;

                                std::tie(mesh, rectangle) = Magnum::Text::Renderer2D::render(*debug_data.font, *debug_data.cache, fnt_size, text->text, *debug_data.text_vertices, *debug_data.text_indices, Magnum::GL::BufferUsage::DynamicDraw, Magnum::Text::Alignment(text->alignment));

                                auto viewport = Magnum::Vector2{_camera->viewport()};
                                auto sc = Magnum::Vector2{viewport.max() / 1024.f};
                                auto text_scaling = Magnum::Matrix3::scaling(sc);
                                auto extra_tr = Magnum::Matrix3(Magnum::Math::IdentityInit);
                                if ((text->alignment & Magnum::Text::Implementation::AlignmentVertical) == Magnum::Text::Implementation::AlignmentLine) // if line (bottom) alignment, push the text a bit above
                                    extra_tr = Magnum::Matrix3::translation({0.f, sc[1] * 0.25f * rectangle.sizeY()});

                                auto text_tr = Magnum::Matrix3(Magnum::Matrix3d(text->transformation));

                                if (text->draw_background) {
                                    auto bg_scaling = Magnum::Matrix3::scaling(Magnum::Vector2{viewport[0], rectangle.sizeY() * sc[1]});

                                    // draw the background
                                    (*debug_data.background_shader)
                                        .setTransformationProjectionMatrix(Magnum::Matrix3::projection(viewport) * text_tr * bg_scaling)
                                        .setColor(Magnum::Vector4(Magnum::Vector4d(text->background_color)))
                                        .draw(*debug_data.background_mesh);
                                }

                                (*debug_data.text_shader)
                                    .bindVectorTexture(debug_data.cache->texture())
                                    .setTransformationProjectionMatrix(Magnum::Matrix3::projection(viewport) * text_tr * extra_tr * text_scaling)
                                    // .setTransformationProjectionMatrix(Magnum::Matrix3::projection(Magnum::Vector2{_camera->viewport()}) * Magnum::Matrix3::translation(Magnum::Vector2{-text_renderer->rectangle().sizeX() / 2.f, -text_renderer->rectangle().sizeY() / 2.f}) * Magnum::Matrix3(Magnum::Matrix3d(text.transformation)))
                                    .setColor(Magnum::Vector4(Magnum::Vector4d(text->color)))
                                    .setOutlineRange(0.4f, 0.45f)
                                    .setSmoothness(0.075f)
                                    .draw(mesh);
                            }

                            Magnum::GL::Renderer::enable(Magnum::GL::Renderer::Feature::DepthTest);
                            Magnum::GL::Renderer::enable(Magnum::GL::Renderer::Feature::FaceCulling);
                        }
                    }

                    if (_recording) {
                        _image = framebuffer.read(framebuffer.viewport(), {format});
                    }

                    if (_recording_depth) {
                        _depth_image = framebuffer.read(framebuffer.viewport(), {Magnum::GL::PixelFormat::DepthComponent, Magnum::GL::PixelType::Float});
                    }

                    if (_recording_video) {
                        auto image = framebuffer.read(framebuffer.viewport(), {Magnum::PixelFormat::RGB8Unorm});

                        std::vector<uint8_t> data(image.size().product() * sizeof(Magnum::Color3ub));
                        Corrade::Containers::StridedArrayView2D<const Magnum::Color3ub> src = image.pixels<Magnum::Color3ub>().flipped<0>();
                        Corrade::Containers::StridedArrayView2D<Magnum::Color3ub> dst{Corrade::Containers::arrayCast<Magnum::Color3ub>(Corrade::Containers::arrayView(data)), {std::size_t(image.size().y()), std::size_t(image.size().x())}};
                        Corrade::Utility::copy(src, dst);

                        _ffmpeg_process->stdin().write(reinterpret_cast<char*>(data.data()), data.size());
                        _ffmpeg_process->stdin().flush();
                    }
                }

                void Camera::_clean_up_subprocess()
                {
                    if (_ffmpeg_process) {
                        _ffmpeg_process->close();
                        delete _ffmpeg_process;
                    }

                    _ffmpeg_process = nullptr;
                }
            } // namespace gs
        } // namespace magnum
    } // namespace gui
} // namespace robot_dart

```

