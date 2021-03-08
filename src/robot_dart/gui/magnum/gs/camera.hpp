#ifndef ROBOT_DART_GUI_MAGNUM_GS_CAMERA_HPP
#define ROBOT_DART_GUI_MAGNUM_GS_CAMERA_HPP

#include <robot_dart/gui/magnum/gs/light.hpp>
#include <robot_dart/gui/magnum/types.hpp>
#include <robot_dart/robot_dart_simu.hpp>

#include <boost/version.hpp>
#if ((BOOST_VERSION / 100000) > 1) || ((BOOST_VERSION / 100000) == 1 && ((BOOST_VERSION / 100 % 1000) >= 64))
#include <boost/process.hpp> // for launching ffmpeg
#define ROBOT_DART_HAS_BOOST_PROCESS
#endif

#include <Corrade/Containers/Optional.h>
#include <Magnum/GL/Mesh.h>
#include <Magnum/Image.h>
#include <Magnum/Shaders/DistanceFieldVector.h>
#include <Magnum/Shaders/VertexColor.h>
#include <Magnum/Text/Renderer.h>

namespace robot_dart {
    namespace gui {
        namespace magnum {
            struct DebugDrawData;

            namespace gs {
                // This is partly code from the ThirdPersonCameraController of https://github.com/alexesDev/magnum-tips
                class Camera : public Object3D {
                public:
                    explicit Camera(Object3D& object, Magnum::Int width, Magnum::Int height);
                    ~Camera();

                    Camera3D& camera() const;
                    Object3D& root_object();
                    Object3D& camera_object() const;

                    Camera& set_viewport(const Magnum::Vector2i& size);

                    Camera& move(const Magnum::Vector2i& shift);
                    Camera& forward(Magnum::Float speed);
                    Camera& strafe(Magnum::Float speed);

                    Camera& set_speed(const Magnum::Vector2& speed);
                    Camera& set_near_plane(Magnum::Float near_plane);
                    Camera& set_far_plane(Magnum::Float far_plane);
                    Camera& set_fov(Magnum::Float fov);
                    Camera& set_camera_params(Magnum::Float near_plane, Magnum::Float far_plane, Magnum::Float fov, Magnum::Int width, Magnum::Int height);

                    Magnum::Vector2 speed() const { return _speed; }
                    Magnum::Float near_plane() const { return _near_plane; }
                    Magnum::Float far_plane() const { return _far_plane; }
                    Magnum::Float fov() const { return static_cast<Magnum::Float>(_fov); }
                    Magnum::Int width() const { return _camera->viewport()[0]; }
                    Magnum::Int height() const { return _camera->viewport()[1]; }
                    Magnum::Matrix3 const intrinsic_matrix();

                    Camera& look_at(const Magnum::Vector3& camera, const Magnum::Vector3& center, const Magnum::Vector3& up = Magnum::Vector3::zAxis());

                    void transform_lights(std::vector<gs::Light>& lights) const;

                    void record(bool recording, bool recording_depth = false)
                    {
                        _recording = recording;
                        _recording_depth = recording_depth;
                    }

                    // FPS is mandatory here (compared to Graphics and CameraOSR)
                    void record_video(const std::string& video_fname, int fps);
                    bool recording() { return _recording; }
                    bool recording_depth() { return _recording_depth; }

                    Corrade::Containers::Optional<Magnum::Image2D>& image() { return _image; }
                    Corrade::Containers::Optional<Magnum::Image2D>& depth_image() { return _depth_image; }

                    void draw(Magnum::SceneGraph::DrawableGroup3D& drawables, Magnum::GL::AbstractFramebuffer& framebuffer, Magnum::PixelFormat format, RobotDARTSimu* simu, const DebugDrawData& debug_data, bool draw_debug = true);

                private:
                    Object3D* _yaw_object;
                    Object3D* _pitch_object;
                    Object3D* _camera_object;

                    Camera3D* _camera;
                    Magnum::Vector2 _speed{-0.01f, 0.01f};

                    Magnum::Vector3 _up, _front, _right;
                    Magnum::Float _aspect_ratio, _near_plane, _far_plane;
                    Magnum::Rad _fov;

                    bool _recording = false, _recording_depth = false;
                    bool _recording_video = false;
                    Corrade::Containers::Optional<Magnum::Image2D> _image, _depth_image;

#ifdef ROBOT_DART_HAS_BOOST_PROCESS
                    // pipe to write a video
                    boost::process::opstream _video_pipe;
                    boost::process::child _ffmpeg_process;
#else
                    pid_t _video_pid = 0;
                    int _video_fd[2];
#endif
                };
            } // namespace gs
        } // namespace magnum
    } // namespace gui
} // namespace robot_dart

#endif
