#include "camera.hpp"
#include "robot_dart/gui/magnum/base_application.hpp"
#include "robot_dart/gui_data.hpp"
#include "robot_dart/robot_dart_simu.hpp"
#include "robot_dart/utils.hpp"

#include <algorithm>
#include <signal.h>

#include <Corrade/Containers/ArrayViewStl.h>
#include <Corrade/Containers/StridedArrayView.h>
#include <Corrade/Utility/Algorithms.h>

#include <Magnum/GL/AbstractFramebuffer.h>
#include <Magnum/GL/GL.h>
#include <Magnum/GL/PixelFormat.h>
#include <Magnum/ImageView.h>
#include <Magnum/PixelFormat.h>

#include <Magnum/EigenIntegration/GeometryIntegration.h>

namespace robot_dart {
    namespace gui {
        namespace magnum {
            namespace gs {
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

                    _camera = new Camera3D{*_camera_object};
                    _camera->setAspectRatioPolicy(Magnum::SceneGraph::AspectRatioPolicy::Extend)
                        .setProjectionMatrix(Magnum::Matrix4::perspectiveProjection(_fov, _aspect_ratio, _near_plane, _far_plane))
                        .setViewport({width, height});
                }

                Camera::~Camera()
                {
#ifdef ROBOT_DART_HAS_BOOST_PROCESS
                    if (_ffmpeg_process.id() > 0) {
                        // we let ffmpeg finish nicely by detaching it and sending the signal
                        // terminates() send a violent SIGTERM...
                        _ffmpeg_process.detach();
                        kill(_ffmpeg_process.id(), SIGINT);
                    }
#else
                    if (_video_pid != 0)
                        kill(_video_pid, SIGINT);
#endif
                }

                Camera3D& Camera::camera() const
                {
                    return *_camera;
                }

                Object3D& Camera::camera_object() const
                {
                    return *_camera_object;
                }

                Camera& Camera::set_viewport(const Magnum::Vector2i& size)
                {
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

                    _camera->setProjectionMatrix(Magnum::Matrix4::perspectiveProjection(_fov, _aspect_ratio, _near_plane, _far_plane))
                        .setViewport({width, height});

                    return *this;
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
                    // we use boost process: https://www.boost.org/doc/libs/1_73_0/doc/html/boost_process/tutorial.html
                    namespace bp = boost::process;
                    // search for ffmpeg
                    boost::filesystem::path ffmpeg = bp::search_path("ffmpeg");
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
#ifdef ROBOT_DART_HAS_BOOST_PROCESS
                    // clang-format off
                    _ffmpeg_process = bp::child(ffmpeg, bp::args(args), bp::std_in < _video_pipe, bp::std_out > "/dev/null", bp::std_err > "/dev/null");
                    // clang-format on

#else
                    // we do it the old way
                    // this could should be removed in the future once boost.process is on every computer...
                    pipe(_video_fd);
                    //  Data written to fd[1] appears on (i.e., can be read from) fd[0].
                    _video_pid = fork();
                    if (_video_pid != 0) {  // main process
                        close(_video_fd[0]); // we close the input on this side
                    } else { // ffmpeg process
                        close(_video_fd[1]); // ffmpeg does not write here
                        dup2(_video_fd[0], STDIN_FILENO); // ffmpeg will read the fd[0] as stdin
                        char** argv =  (char**) calloc(args.size() + 1, sizeof(char*)); // we need the 0 at the end
                        for (int i = 0; i < args.size(); ++i)
                            argv[i] = (char*) args[i].c_str();
                        execvp("ffmpeg", argv);
                    }
                   // ROBOT_DART_WARNING(true, "Boost version does not support 'boost.process'. Cannot record video!");
#endif
                }

                void Camera::draw(Magnum::SceneGraph::DrawableGroup3D& drawables, Magnum::GL::AbstractFramebuffer& framebuffer, Magnum::PixelFormat format, RobotDARTSimu* simu, Magnum::Shaders::VertexColor3D& axes_shader, Magnum::GL::Mesh& axes_mesh, bool draw_debug)
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
                        std::vector<std::pair<dart::dynamics::BodyNode*, double>> axes = simu->gui_data()->drawing_axes();
                        for (auto& axis : axes) {
                            Magnum::Matrix4 world_transform = Magnum::Matrix4(Magnum::Matrix4d(axis.first->getWorldTransform().matrix()));
                            Magnum::Matrix4 scaling = Magnum::Matrix4::scaling(Magnum::Vector3(axis.second, axis.second, axis.second));

                            axes_shader.setTransformationProjectionMatrix(_camera->projectionMatrix() * _camera->cameraMatrix() * world_transform * scaling)
                                .draw(axes_mesh);
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
#ifdef ROBOT_DART_HAS_BOOST_PROCESS

                        _video_pipe.write((char*)data.data(), data.size());
                        _video_pipe.flush();
#else
                        write(_video_fd[1], (char*)data.data(), data.size());
#endif

                    }
                }
            } // namespace gs
        } // namespace magnum
    } // namespace gui
} // namespace robot_dart