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
                    explicit Camera(Object3D& object, Magnum::Int width, Magnum::Int height) : Object3D{&object}
                    {
                        _yawObject = new Object3D{this};
                        _pitchObject = new Object3D{_yawObject};

                        _cameraObject = new Object3D{_pitchObject};
                        _cameraObject->setTransformation(Magnum::Matrix4::lookAt({0., 2., 1.}, {0., 0., 0.}, Magnum::Vector3::zAxis(1)));

                        _fov = Magnum::Deg(60.0f);
                        _aspectRatio = 4.0f / 3.0f;
                        _nearPlane = 0.01f;
                        _farPlane = 200.f;

                        _camera = new Camera3D{*_cameraObject};
                        _camera->setAspectRatioPolicy(Magnum::SceneGraph::AspectRatioPolicy::Extend)
                            .setProjectionMatrix(Magnum::Matrix4::perspectiveProjection(_fov, _aspectRatio, _nearPlane, _farPlane))
                            .setViewport({width, height});
                    }

                    Camera3D& camera() const
                    {
                        return *_camera;
                    }

                    Object3D& cameraObject() const
                    {
                        return *_cameraObject;
                    }

                    Camera& zoom(Magnum::Float percentage)
                    {
                        Magnum::Float perc = std::max(-1.f, std::min(1.f, percentage));

                        _fov += perc * _fov;

                        _camera->setProjectionMatrix(
                            Magnum::Matrix4::perspectiveProjection(_fov, _aspectRatio, _nearPlane, _farPlane));

                        return *this;
                    }

                    Camera& move(const Magnum::Vector2i& shift)
                    {
                        Magnum::Vector2 s = Magnum::Vector2{shift} * _speed;

                        _yawObject->rotate(Magnum::Rad(s.x()), Magnum::Vector3::zAxis(1));
                        _pitchObject->rotate(Magnum::Rad(s.y()), Magnum::Vector3::xAxis(1));

                        return *this;
                    }

                    Camera& forward(Magnum::Float speed)
                    {
                        _cameraObject->translate(speed * _front);

                        return *this;
                    }

                    Camera& strafe(Magnum::Float speed)
                    {
                        _cameraObject->translate(speed * Magnum::Math::cross(_front, _up).normalized());

                        return *this;
                    }

                    Camera& setSpeed(const Magnum::Vector2& speed)
                    {
                        _speed = speed;
                        return *this;
                    }

                    Camera& lookAt(const Magnum::Vector3& camera, const Magnum::Vector3& center, const Magnum::Vector3& up = Magnum::Vector3::zAxis())
                    {
                        _front = (center - camera).normalized();
                        _up = up;

                        _cameraObject->setTransformation(Magnum::Matrix4::lookAt(camera, center, up));
                        _yawObject->setTransformation(Magnum::Matrix4{});
                        _pitchObject->setTransformation(Magnum::Matrix4{});

                        return *this;
                    }

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