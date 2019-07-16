#ifndef ROBOT_DART_GUI_MAGNUM_GS_CAMERA_HPP
#define ROBOT_DART_GUI_MAGNUM_GS_CAMERA_HPP

#include <robot_dart/gui/magnum/types.hpp>

namespace robot_dart {
    namespace gui {
        namespace magnum {
            namespace gs {
                class Camera {
                public:
                    explicit Camera(Scene3D& parent, Magnum::Int width, Magnum::Int height)
                    {
                        using namespace Magnum::Math::Literals;

                        _fov = 35.0_degf;
                        _aspectRatio = 1.0f;
                        _nearPlane = 0.01f;
                        _farPlane = 100.f;

                        _up = Magnum::Vector3{0.f, 0.f, 1.f};
                        _yaw = -Magnum::Math::Constants<Magnum::Float>::piHalf();
                        _pitch = 0.f;
                        _front = (Magnum::Vector3{
                                      std::cos(_yaw) * std::cos(_pitch), std::sin(_pitch), std::sin(_yaw) * std::cos(_pitch)})
                                     .normalized();

                        /* Create the camera object attached to the parent */
                        (*(_cameraObject = new Object3D{&parent}));
                        Magnum::Vector3 cameraPos = _cameraObject->transformation().translation();
                        _cameraObject->setTransformation(Magnum::Matrix4::lookAt(cameraPos, cameraPos + _front, _up));
                        (*(_camera = new Magnum::SceneGraph::Camera3D{*_cameraObject}))
                            .setAspectRatioPolicy(Magnum::SceneGraph::AspectRatioPolicy::Extend)
                            .setProjectionMatrix(Magnum::Matrix4::perspectiveProjection(_fov, _aspectRatio, _nearPlane, _farPlane))
                            .setViewport({width, height});
                    }

                    Object3D* cameraObject() { return _cameraObject; }
                    Camera3D* camera() { return _camera; }

                    Camera& zoom(Magnum::Float percentage)
                    {
                        Magnum::Float perc = std::max(-1.f, std::min(1.f, percentage));

                        _fov += perc * _fov;

                        _camera->setProjectionMatrix(
                            Magnum::Matrix4::perspectiveProjection(_fov, _aspectRatio, _nearPlane, _farPlane));

                        return *this;
                    }

                    Camera& rotate(Magnum::Float dYaw, Magnum::Float dPitch, Magnum::Float sensitivity = 0.001f)
                    {
                        {
                            _pitch += dPitch * sensitivity;
                            _pitch = std::max(-Magnum::Math::Constants<Magnum::Float>::piHalf(),
                                std::min(Magnum::Math::Constants<Magnum::Float>::piHalf(), _pitch));
                            _yaw += dYaw * sensitivity;

                            _front = (Magnum::Vector3{
                                          std::cos(_yaw) * std::cos(_pitch), std::sin(_pitch), std::sin(_yaw) * std::cos(_pitch)})
                                         .normalized();
                            Magnum::Vector3 cameraPos = _cameraObject->transformation().translation();

                            _cameraObject->setTransformation(Magnum::Matrix4::lookAt(cameraPos, cameraPos + _front, _up));

                            return *this;
                        }
                    }

                    Camera& move(Magnum::Float speed)
                    {
                        _cameraObject->translate(speed * _front);

                        return *this;
                    }

                    Camera& strafe(Magnum::Float speed)
                    {
                        _cameraObject->translate(speed * Magnum::Math::cross(_front, _up).normalized());

                        return *this;
                    }

                    Camera& look_at(const Magnum::Vector3& camera, const Magnum::Vector3& center, const Magnum::Vector3& up = Magnum::Vector3::zAxis())
                    {
                        _front = (center - camera).normalized();
                        _up = up;

                        /* Update yaw and pitch */
                        _yaw = std::atan(_front.z() / _front.x());
                        _pitch = std::atan2(_front.y(), _front.x() / std::cos(_yaw));

                        _cameraObject->setTransformation(Magnum::Matrix4::lookAt(camera, camera + _front, _up));

                        return *this;
                    }

                protected:
                    Object3D* _cameraObject;
                    Camera3D* _camera;

                    Magnum::Float _aspectRatio, _nearPlane, _farPlane;
                    Magnum::Rad _fov;

                    Magnum::Vector3 _up, _front;
                    Magnum::Float _yaw, _pitch;
                };
            } // namespace gs
        } // namespace magnum
    } // namespace gui
} // namespace robot_dart

#endif