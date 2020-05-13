#ifndef ROBOT_DART_GUI_MAGNUM_GS_CAMERA_HPP
#define ROBOT_DART_GUI_MAGNUM_GS_CAMERA_HPP

#include <robot_dart/gui/magnum/gs/light.hpp>
#include <robot_dart/gui/magnum/types.hpp>

#include <Corrade/Containers/Optional.h>
#include <Magnum/Image.h>

namespace robot_dart {
    namespace gui {
        namespace magnum {
            namespace gs {
                // This is partly code from the ThirdPersonCameraController of https://github.com/alexesDev/magnum-tips
                class Camera : public Object3D {
                public:
                    explicit Camera(Object3D& object, Magnum::Int width, Magnum::Int height);

                    Camera3D& camera() const;
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

                    Camera& look_at(const Magnum::Vector3& camera, const Magnum::Vector3& center, const Magnum::Vector3& up = Magnum::Vector3::zAxis());

                    void transform_lights(std::vector<gs::Light>& lights) const;

                    void record(bool recording, bool depthRecording = false)
                    {
                        _recording = recording;
                        _recording_depth = depthRecording;
                    }

                    bool recording() { return _recording; }
                    bool recording_depth() { return _recording_depth; }

                    Corrade::Containers::Optional<Magnum::Image2D>& image() { return _image; }
                    Corrade::Containers::Optional<Magnum::Image2D>& depth_image() { return _depth_image; }

                    void draw(Magnum::SceneGraph::DrawableGroup3D& drawables, Magnum::GL::AbstractFramebuffer& framebuffer, Magnum::PixelFormat format);

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
                    Corrade::Containers::Optional<Magnum::Image2D> _image, _depth_image;
                };
            } // namespace gs
        } // namespace magnum
    } // namespace gui
} // namespace robot_dart

#endif