#ifndef ROBOT_DART_GUI_MAGNUM_GS_CAMERA_HPP
#define ROBOT_DART_GUI_MAGNUM_GS_CAMERA_HPP

#include <robot_dart/gui/magnum/types.hpp>

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

                    Camera& zoom(Magnum::Float percentage);
                    Camera& move(const Magnum::Vector2i& shift);
                    Camera& forward(Magnum::Float speed);
                    Camera& strafe(Magnum::Float speed);

                    Camera& setSpeed(const Magnum::Vector2& speed);

                    Camera& lookAt(const Magnum::Vector3& camera, const Magnum::Vector3& center, const Magnum::Vector3& up = Magnum::Vector3::zAxis());

                private:
                    Object3D* _yawObject;
                    Object3D* _pitchObject;
                    Object3D* _cameraObject;

                    Camera3D* _camera;
                    Magnum::Vector2 _speed{-0.01f, 0.01f};

                    Magnum::Vector3 _up, _front;
                    Magnum::Float _aspectRatio, _nearPlane, _farPlane;
                    Magnum::Rad _fov;
                };
            } // namespace gs
        } // namespace magnum
    } // namespace gui
} // namespace robot_dart

#endif