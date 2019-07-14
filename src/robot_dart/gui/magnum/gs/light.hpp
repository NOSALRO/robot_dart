#ifndef ROBOT_DART_GUI_MAGNUM_GS_LIGHT_HPP
#define ROBOT_DART_GUI_MAGNUM_GS_LIGHT_HPP

#include <robot_dart/gui/magnum/gs/material.hpp>

namespace robot_dart {
    namespace gui {
        namespace magnum {
            namespace gs {
                class Light {
                public:
                    Light() : _position(Magnum::Vector4{0.f, 0.f, 0.f, 1.f}),
                              _transformedPosition(_position),
                              _material(Material()),
                              _spotDirection(Magnum::Vector3{1.f, 0.f, 0.f}),
                              _spotExponent(1.f),
                              _spotCutOff(Magnum::Math::Constants<Magnum::Float>::piHalf()),
                              _attenuation(Magnum::Vector4{0.f, 0.f, 1.f, 1.f}) {}

                    Light(const Magnum::Vector4& position, const Material& material, const Magnum::Vector3& spotDirection,
                        Magnum::Float spotExponent, Magnum::Float spotCutOff, const Magnum::Vector4& attenuation) : _position(position),
                                                                                                                    _transformedPosition(_position),
                                                                                                                    _material(material),
                                                                                                                    _spotDirection(spotDirection),
                                                                                                                    _spotExponent(spotExponent),
                                                                                                                    _spotCutOff(spotCutOff),
                                                                                                                    _attenuation(attenuation) {}

                    // Magnum::Vector4& position();
                    Magnum::Vector4 position() const { return _position; }

                    Magnum::Vector4& transformedPosition() { return _transformedPosition; }
                    Magnum::Vector4 transformedPosition() const { return _transformedPosition; }

                    Material& material() { return _material; }
                    Material material() const { return _material; }

                    // Magnum::Vector3& spotDirection() { return _spotDirection; }
                    Magnum::Vector3 spotDirection() const { return _spotDirection; }

                    Magnum::Vector3& transformedSpotDirection() { return _transformedSpotDirection; }
                    Magnum::Vector3 transformedSpotDirection() const { return _transformedSpotDirection; }

                    Magnum::Float& spotExponent() { return _spotExponent; }
                    Magnum::Float spotExponent() const { return _spotExponent; }

                    Magnum::Float& spotCutOff() { return _spotCutOff; }
                    Magnum::Float spotCutOff() const { return _spotCutOff; }

                    Magnum::Vector4& attenuation() { return _attenuation; }
                    Magnum::Vector4 attenuation() const { return _attenuation; }

                    Light& setPosition(const Magnum::Vector4& position)
                    {
                        _position = position;
                        _transformedPosition = position;
                        return *this;
                    }

                    Light& setTransformedPosition(const Magnum::Vector4& transformedPosition)
                    {
                        _transformedPosition = transformedPosition;
                        return *this;
                    }

                    Light& setMaterial(const Material& material)
                    {
                        _material = material;
                        return *this;
                    }

                    Light& setSpotDirection(const Magnum::Vector3& spotDirection)
                    {
                        _spotDirection = spotDirection;
                        _transformedSpotDirection = _spotDirection;
                        return *this;
                    }

                    Light& setTransformedSpotDirection(const Magnum::Vector3& transformedSpotDirection)
                    {
                        _transformedSpotDirection = transformedSpotDirection;
                        return *this;
                    }

                    Light& setSpotExponent(Magnum::Float spotExponent)
                    {
                        _spotExponent = spotExponent;
                        return *this;
                    }

                    Light& setSpotCutOff(Magnum::Float spotCutOff)
                    {
                        _spotCutOff = spotCutOff;
                        return *this;
                    }

                    Light& setAttenuation(const Magnum::Vector4& attenuation)
                    {
                        _attenuation = attenuation;
                        return *this;
                    }

                protected:
                    // Position for point-lights and spot-lights
                    // Direction for directional lights (if position.w == 0)
                    Magnum::Vector4 _position;
                    // TO-DO: Handle dirtiness of transformed position
                    Magnum::Vector4 _transformedPosition;
                    Material _material;
                    Magnum::Vector3 _spotDirection;
                    // TO-DO: Handle dirtiness of transformed spot direction
                    Magnum::Vector3 _transformedSpotDirection;
                    Magnum::Float _spotExponent, _spotCutOff;

                    // Attenuation is: intensity/(constant + d * (linear + quadratic * d)
                    // where d is the distance from the light position to the vertex position.
                    //
                    // (constant,linear,quadratic,intensity)
                    Magnum::Vector4 _attenuation;
                };

                // Helpers for creating lights
                Light createPointLight(const Magnum::Vector3& position, const Material& material,
                    Magnum::Float intensity, const Magnum::Vector3& attenuationTerms)
                {
                    Light light;
                    light.setMaterial(material);
                    light.setPosition(Magnum::Vector4{position, 1.f})
                        .setAttenuation(Magnum::Vector4{attenuationTerms, intensity});

                    return light;
                }

                Light createSpotLight(const Magnum::Vector3& position, const Material& material,
                    const Magnum::Vector3& spotDirection, Magnum::Float spotExponent, Magnum::Float spotCutOff,
                    Magnum::Float intensity, const Magnum::Vector3& attenuationTerms)
                {
                    return Light(Magnum::Vector4{position, 1.f}, material, spotDirection, spotExponent, spotCutOff,
                        Magnum::Vector4{attenuationTerms, intensity});
                }

                Light createDirectionalLight(
                    const Magnum::Vector3& direction, const Material& material)
                {
                    Light light;
                    light.setMaterial(material);
                    light.setPosition(Magnum::Vector4{direction, 0.f});

                    return light;
                }
            } // namespace gs
        } // namespace magnum
    } // namespace gui
} // namespace robot_dart

#endif
