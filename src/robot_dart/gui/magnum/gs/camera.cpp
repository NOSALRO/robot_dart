#include "camera.hpp"

#include <Magnum/GL/AbstractFramebuffer.h>
#include <Magnum/GL/GL.h>
#include <Magnum/GL/PixelFormat.h>
#include <Magnum/ImageView.h>
#include <Magnum/PixelFormat.h>

namespace robot_dart {
    namespace gui {
        namespace magnum {
            namespace gs {
                Camera::Camera(Object3D& object, Magnum::Int width, Magnum::Int height) : Object3D{&object}
                {
                    _yawObject = new Object3D{this};
                    _pitchObject = new Object3D{_yawObject};

                    _yawObject->setTransformation(Magnum::Matrix4{});
                    _pitchObject->setTransformation(Magnum::Matrix4{});

                    Magnum::Vector3 center{0., 0., 0.};
                    Magnum::Vector3 camera{0., 2., 1.};
                    _front = (center - camera).normalized();
                    _up = Magnum::Vector3::zAxis();
                    _right = Magnum::Math::cross(_front, _up).normalized();

                    _cameraObject = new Object3D{_pitchObject};
                    _cameraObject->setTransformation(Magnum::Matrix4::lookAt(camera, center, _up));

                    _fov = Magnum::Deg(60.0f);
                    _aspectRatio = 4.0f / 3.0f;
                    _nearPlane = 0.01f;
                    _farPlane = 200.f;

                    _camera = new Camera3D{*_cameraObject};
                    _camera->setAspectRatioPolicy(Magnum::SceneGraph::AspectRatioPolicy::Extend)
                        .setProjectionMatrix(Magnum::Matrix4::perspectiveProjection(_fov, _aspectRatio, _nearPlane, _farPlane))
                        .setViewport({width, height});
                }

                Camera3D& Camera::camera() const
                {
                    return *_camera;
                }

                Object3D& Camera::cameraObject() const
                {
                    return *_cameraObject;
                }

                Camera& Camera::zoom(Magnum::Float percentage)
                {
                    Magnum::Float perc = std::max(-1.f, std::min(1.f, percentage));

                    _fov += perc * _fov;

                    _camera->setProjectionMatrix(
                        Magnum::Matrix4::perspectiveProjection(_fov, _aspectRatio, _nearPlane, _farPlane));

                    return *this;
                }

                Camera& Camera::move(const Magnum::Vector2i& shift)
                {
                    Magnum::Vector2 s = Magnum::Vector2{shift} * _speed;

                    _yawObject->rotate(Magnum::Rad(s.x()), _up);
                    _pitchObject->rotate(Magnum::Rad(s.y()), _right);

                    return *this;
                }

                Camera& Camera::forward(Magnum::Float speed)
                {
                    _cameraObject->translate(speed * _front);

                    return *this;
                }

                Camera& Camera::strafe(Magnum::Float speed)
                {
                    _cameraObject->translate(speed * _right);

                    return *this;
                }

                Camera& Camera::setSpeed(const Magnum::Vector2& speed)
                {
                    _speed = speed;
                    return *this;
                }

                Camera& Camera::lookAt(const Magnum::Vector3& camera, const Magnum::Vector3& center, const Magnum::Vector3& up)
                {
                    _front = (center - camera).normalized();
                    _up = up;
                    _right = Magnum::Math::cross(_front, _up).normalized();

                    _cameraObject->setTransformation(Magnum::Matrix4::lookAt(camera, center, up));
                    _yawObject->setTransformation(Magnum::Matrix4{});
                    _pitchObject->setTransformation(Magnum::Matrix4{});

                    return *this;
                }

                void Camera::transformLights(std::vector<gs::Light>& lights) const
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
                        lights[i].setTransformedPosition(Magnum::Vector4{pos, old_pos.w()});
                        /* Transform spotlight direction */
                        lights[i].setTransformedSpotDirection(_camera->cameraMatrix().transformVector(lights[i].spotDirection()));
                    }
                }

                void Camera::draw(Magnum::SceneGraph::DrawableGroup3D& drawables, Magnum::GL::AbstractFramebuffer& framebuffer, Magnum::PixelFormat format)
                {
                    _camera->draw(drawables);
                    if (_recording) {
                        _image = framebuffer.read(framebuffer.viewport(), {format});
                    }
                }
            } // namespace gs
        } // namespace magnum
    } // namespace gui
} // namespace robot_dart