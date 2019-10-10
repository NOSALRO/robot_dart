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
                    Object3D& cameraObject() const;

                    Camera& setViewport(const Magnum::Vector2i& size);

                    Camera& move(const Magnum::Vector2i& shift);
                    Camera& forward(Magnum::Float speed);
                    Camera& strafe(Magnum::Float speed);

                    Camera& setSpeed(const Magnum::Vector2& speed);
                    Camera& setNearPlane(Magnum::Float nearPlane);
                    Camera& setFarPlane(Magnum::Float farPlane);
                    Camera& setFOV(Magnum::Float fov);
                    Camera& setCameraParameters(Magnum::Float nearPlane, Magnum::Float farPlane, Magnum::Float fov, Magnum::Int width, Magnum::Int height);

                    Magnum::Vector2 speed() const { return _speed; }
                    Magnum::Float nearPlane() const { return _nearPlane; }
                    Magnum::Float farPlane() const { return _farPlane; }
                    Magnum::Float fov() const { return static_cast<Magnum::Float>(_fov); }

                    Camera& lookAt(const Magnum::Vector3& camera, const Magnum::Vector3& center, const Magnum::Vector3& up = Magnum::Vector3::zAxis());

                    void transformLights(std::vector<gs::Light>& lights) const;

                    void record(bool recording) { _recording = recording; }
                    bool isRecording() { return _recording; }

                    Corrade::Containers::Optional<Magnum::Image2D>& image() { return _image; }

                    void draw(Magnum::SceneGraph::DrawableGroup3D& drawables, Magnum::GL::AbstractFramebuffer& framebuffer, Magnum::PixelFormat format);

                private:
                    Object3D* _yawObject;
                    Object3D* _pitchObject;
                    Object3D* _cameraObject;

                    Camera3D* _camera;
                    Magnum::Vector2 _speed{-0.01f, 0.01f};

                    Magnum::Vector3 _up, _front, _right;
                    Magnum::Float _aspectRatio, _nearPlane, _farPlane;
                    Magnum::Rad _fov;

                    bool _recording = false;
                    Corrade::Containers::Optional<Magnum::Image2D> _image;
                };
            } // namespace gs
        } // namespace magnum
    } // namespace gui
} // namespace robot_dart

#endif