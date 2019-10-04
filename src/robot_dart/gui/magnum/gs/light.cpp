#include "light.hpp"

#include <Magnum/GL/Texture.h>

namespace robot_dart {
    namespace gui {
        namespace magnum {
            namespace gs {
                Light::Light() : _position(Magnum::Vector4{0.f, 0.f, 0.f, 1.f}),
                                 _transformedPosition(_position),
                                 _material(Material()),
                                 _spotDirection(Magnum::Vector3{1.f, 0.f, 0.f}),
                                 _spotExponent(1.f),
                                 _spotCutOff(Magnum::Math::Constants<Magnum::Float>::piHalf()),
                                 _attenuation(Magnum::Vector4{0.f, 0.f, 1.f, 1.f}) {}

                Light::Light(const Magnum::Vector4& position, const Material& material, const Magnum::Vector3& spotDirection,
                    Magnum::Float spotExponent, Magnum::Float spotCutOff, const Magnum::Vector4& attenuation) : _position(position),
                                                                                                                _transformedPosition(_position),
                                                                                                                _material(material),
                                                                                                                _spotDirection(spotDirection),
                                                                                                                _spotExponent(spotExponent),
                                                                                                                _spotCutOff(spotCutOff),
                                                                                                                _attenuation(attenuation) {}

                // Magnum::Vector4& Light::position();
                Magnum::Vector4 Light::position() const { return _position; }

                Magnum::Vector4& Light::transformedPosition() { return _transformedPosition; }
                Magnum::Vector4 Light::transformedPosition() const { return _transformedPosition; }

                Material& Light::material() { return _material; }
                Material Light::material() const { return _material; }

                // Magnum::Vector3& Light::spotDirection() { return _spotDirection; }
                Magnum::Vector3 Light::spotDirection() const { return _spotDirection; }

                Magnum::Vector3& Light::transformedSpotDirection() { return _transformedSpotDirection; }
                Magnum::Vector3 Light::transformedSpotDirection() const { return _transformedSpotDirection; }

                Magnum::Float& Light::spotExponent() { return _spotExponent; }
                Magnum::Float Light::spotExponent() const { return _spotExponent; }

                Magnum::Float& Light::spotCutOff() { return _spotCutOff; }
                Magnum::Float Light::spotCutOff() const { return _spotCutOff; }

                Magnum::Vector4& Light::attenuation() { return _attenuation; }
                Magnum::Vector4 Light::attenuation() const { return _attenuation; }

                Magnum::Matrix4 Light::shadowMatrix() const { return _shadowTransform; }

                Light& Light::setPosition(const Magnum::Vector4& position)
                {
                    _position = position;
                    _transformedPosition = position;
                    return *this;
                }

                Light& Light::setTransformedPosition(const Magnum::Vector4& transformedPosition)
                {
                    _transformedPosition = transformedPosition;
                    return *this;
                }

                Light& Light::setMaterial(const Material& material)
                {
                    _material = material;
                    return *this;
                }

                Light& Light::setSpotDirection(const Magnum::Vector3& spotDirection)
                {
                    _spotDirection = spotDirection;
                    _transformedSpotDirection = _spotDirection;
                    return *this;
                }

                Light& Light::setTransformedSpotDirection(const Magnum::Vector3& transformedSpotDirection)
                {
                    _transformedSpotDirection = transformedSpotDirection;
                    return *this;
                }

                Light& Light::setSpotExponent(Magnum::Float spotExponent)
                {
                    _spotExponent = spotExponent;
                    return *this;
                }

                Light& Light::setSpotCutOff(Magnum::Float spotCutOff)
                {
                    _spotCutOff = spotCutOff;
                    return *this;
                }

                Light& Light::setAttenuation(const Magnum::Vector4& attenuation)
                {
                    _attenuation = attenuation;
                    return *this;
                }

                Light& Light::setShadowMatrix(const Magnum::Matrix4& shadowTransform)
                {
                    _shadowTransform = shadowTransform;
                    return *this;
                }
            } // namespace gs
        } // namespace magnum
    } // namespace gui
} // namespace robot_dart