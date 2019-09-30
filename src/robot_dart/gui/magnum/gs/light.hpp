#ifndef ROBOT_DART_GUI_MAGNUM_GS_LIGHT_HPP
#define ROBOT_DART_GUI_MAGNUM_GS_LIGHT_HPP

#include <robot_dart/gui/magnum/gs/material.hpp>

namespace robot_dart {
    namespace gui {
        namespace magnum {
            namespace gs {
                class Light {
                public:
                    Light();

                    Light(const Magnum::Vector4& position, const Material& material, const Magnum::Vector3& spotDirection,
                        Magnum::Float spotExponent, Magnum::Float spotCutOff, const Magnum::Vector4& attenuation);

                    // Magnum::Vector4& position();
                    Magnum::Vector4 position() const;
                    Magnum::Vector4& transformedPosition();
                    Magnum::Vector4 transformedPosition() const;

                    Material& material();
                    Material material() const;

                    // Magnum::Vector3& spotDirection();
                    Magnum::Vector3 spotDirection() const;

                    Magnum::Vector3& transformedSpotDirection();
                    Magnum::Vector3 transformedSpotDirection() const;

                    Magnum::Float& spotExponent();
                    Magnum::Float spotExponent() const;

                    Magnum::Float& spotCutOff();
                    Magnum::Float spotCutOff() const;

                    Magnum::Vector4& attenuation();
                    Magnum::Vector4 attenuation() const;

                    Light& setPosition(const Magnum::Vector4& position);
                    Light& setTransformedPosition(const Magnum::Vector4& transformedPosition);

                    Light& setMaterial(const Material& material);

                    Light& setSpotDirection(const Magnum::Vector3& spotDirection);
                    Light& setTransformedSpotDirection(const Magnum::Vector3& transformedSpotDirection);
                    Light& setSpotExponent(Magnum::Float spotExponent);
                    Light& setSpotCutOff(Magnum::Float spotCutOff);

                    Light& setAttenuation(const Magnum::Vector4& attenuation);

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
                inline Light createPointLight(const Magnum::Vector3& position, const Material& material,
                    Magnum::Float intensity, const Magnum::Vector3& attenuationTerms)
                {
                    Light light;
                    light.setMaterial(material);
                    light.setPosition(Magnum::Vector4{position, 1.f})
                        .setAttenuation(Magnum::Vector4{attenuationTerms, intensity});

                    return light;
                }

                inline Light createSpotLight(const Magnum::Vector3& position, const Material& material,
                    const Magnum::Vector3& spotDirection, Magnum::Float spotExponent, Magnum::Float spotCutOff,
                    Magnum::Float intensity, const Magnum::Vector3& attenuationTerms)
                {
                    return Light(Magnum::Vector4{position, 1.f}, material, spotDirection, spotExponent, spotCutOff,
                        Magnum::Vector4{attenuationTerms, intensity});
                }

                inline Light createDirectionalLight(
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
