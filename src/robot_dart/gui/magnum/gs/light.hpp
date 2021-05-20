#ifndef ROBOT_DART_GUI_MAGNUM_GS_LIGHT_HPP
#define ROBOT_DART_GUI_MAGNUM_GS_LIGHT_HPP

#include <robot_dart/gui/magnum/gs/material.hpp>

#include <Magnum/Math/Matrix4.h>

namespace robot_dart {
    namespace gui {
        namespace magnum {
            namespace gs {
                class Light {
                public:
                    Light();

                    Light(const Magnum::Vector4& position, const Material& material, const Magnum::Vector3& spot_direction,
                        Magnum::Float spot_exponent, Magnum::Float spot_cut_off, const Magnum::Vector4& attenuation, bool cast_shadows = true);

                    // Magnum::Vector4& position();
                    Magnum::Vector4 position() const;
                    Magnum::Vector4& transformed_position();
                    Magnum::Vector4 transformed_position() const;

                    Material& material();
                    Material material() const;

                    // Magnum::Vector3& spot_direction();
                    Magnum::Vector3 spot_direction() const;

                    Magnum::Vector3& transformed_spot_direction();
                    Magnum::Vector3 transformed_spot_direction() const;

                    Magnum::Float& spot_exponent();
                    Magnum::Float spot_exponent() const;

                    Magnum::Float& spot_cut_off();
                    Magnum::Float spot_cut_off() const;

                    Magnum::Vector4& attenuation();
                    Magnum::Vector4 attenuation() const;

                    Magnum::Matrix4 shadow_matrix() const;
                    bool casts_shadows() const;

                    Light& set_position(const Magnum::Vector4& position);
                    Light& set_transformed_position(const Magnum::Vector4& transformed_position);

                    Light& set_material(const Material& material);

                    Light& set_spot_direction(const Magnum::Vector3& spot_direction);
                    Light& set_transformed_spot_direction(const Magnum::Vector3& transformed_spot_direction);
                    Light& set_spot_exponent(Magnum::Float spot_exponent);
                    Light& set_spot_cut_off(Magnum::Float spot_cut_off);

                    Light& set_attenuation(const Magnum::Vector4& attenuation);

                    Light& set_shadow_matrix(const Magnum::Matrix4& shadowTransform);
                    Light& set_casts_shadows(bool cast_shadows);

                protected:
                    // Position for point-lights and spot-lights
                    // Direction for directional lights (if position.w == 0)
                    Magnum::Vector4 _position;
                    // TO-DO: Handle dirtiness of transformed position
                    Magnum::Vector4 _transformed_position;
                    Material _material;
                    Magnum::Vector3 _spot_direction;
                    // TO-DO: Handle dirtiness of transformed spot direction
                    Magnum::Vector3 _transformed_spot_direction;
                    Magnum::Float _spot_exponent, _spot_cut_off;

                    // Attenuation is: intensity/(constant + d * (linear + quadratic * d)
                    // where d is the distance from the light position to the vertex position.
                    //
                    // (constant,linear,quadratic,intensity)
                    Magnum::Vector4 _attenuation;

                    // Shadow-Matrix
                    Magnum::Matrix4 _shadow_transform{};

                    // Whether it casts shadows
                    bool _cast_shadows = true;
                };

                // Helpers for creating lights
                inline Light create_point_light(const Magnum::Vector3& position, const Material& material,
                    Magnum::Float intensity, const Magnum::Vector3& attenuationTerms)
                {
                    Light light;
                    light.set_material(material);
                    light.set_position(Magnum::Vector4{position, 1.f})
                        .set_attenuation(Magnum::Vector4{attenuationTerms, intensity});

                    return light;
                }

                inline Light create_spot_light(const Magnum::Vector3& position, const Material& material,
                    const Magnum::Vector3& spot_direction, Magnum::Float spot_exponent, Magnum::Float spot_cut_off,
                    Magnum::Float intensity, const Magnum::Vector3& attenuationTerms)
                {
                    return Light(Magnum::Vector4{position, 1.f}, material, spot_direction, spot_exponent, spot_cut_off,
                        Magnum::Vector4{attenuationTerms, intensity});
                }

                inline Light create_directional_light(
                    const Magnum::Vector3& direction, const Material& material)
                {
                    Light light;
                    light.set_material(material);
                    light.set_position(Magnum::Vector4{direction, 0.f});

                    return light;
                }
            } // namespace gs
        } // namespace magnum
    } // namespace gui
} // namespace robot_dart

#endif
