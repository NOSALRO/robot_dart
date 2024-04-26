

# File light.cpp

[**File List**](files.md) **>** [**gs**](dir_2f8612d80f6bb57c97efd4c82e0df286.md) **>** [**light.cpp**](light_8cpp.md)

[Go to the documentation of this file](light_8cpp.md)


```C++
#include "light.hpp"

#include <Magnum/GL/Texture.h>

namespace robot_dart {
    namespace gui {
        namespace magnum {
            namespace gs {
                Light::Light() : _position(Magnum::Vector4{0.f, 0.f, 0.f, 1.f}),
                                 _transformed_position(_position),
                                 _material(Material()),
                                 _spot_direction(Magnum::Vector3{1.f, 0.f, 0.f}),
                                 _spot_exponent(1.f),
                                 _spot_cut_off(Magnum::Math::Constants<Magnum::Float>::pi()),
                                 _attenuation(Magnum::Vector4{0.f, 0.f, 1.f, 1.f}),
                                 _cast_shadows(true) {}

                Light::Light(const Magnum::Vector4& position, const Material& material, const Magnum::Vector3& spot_direction,
                    Magnum::Float spot_exponent, Magnum::Float spot_cut_off, const Magnum::Vector4& attenuation, bool cast_shadows) : _position(position),
                                                                                                                                      _transformed_position(_position),
                                                                                                                                      _material(material),
                                                                                                                                      _spot_direction(spot_direction),
                                                                                                                                      _spot_exponent(spot_exponent),
                                                                                                                                      _spot_cut_off(spot_cut_off),
                                                                                                                                      _attenuation(attenuation),
                                                                                                                                      _cast_shadows(cast_shadows) {}

                // Magnum::Vector4& Light::position();
                Magnum::Vector4 Light::position() const { return _position; }

                Magnum::Vector4& Light::transformed_position() { return _transformed_position; }
                Magnum::Vector4 Light::transformed_position() const { return _transformed_position; }

                Material& Light::material() { return _material; }
                Material Light::material() const { return _material; }

                // Magnum::Vector3& Light::spot_direction() { return _spot_direction; }
                Magnum::Vector3 Light::spot_direction() const { return _spot_direction; }

                Magnum::Vector3& Light::transformed_spot_direction() { return _transformed_spot_direction; }
                Magnum::Vector3 Light::transformed_spot_direction() const { return _transformed_spot_direction; }

                Magnum::Float& Light::spot_exponent() { return _spot_exponent; }
                Magnum::Float Light::spot_exponent() const { return _spot_exponent; }

                Magnum::Float& Light::spot_cut_off() { return _spot_cut_off; }
                Magnum::Float Light::spot_cut_off() const { return _spot_cut_off; }

                Magnum::Vector4& Light::attenuation() { return _attenuation; }
                Magnum::Vector4 Light::attenuation() const { return _attenuation; }

                Magnum::Matrix4 Light::shadow_matrix() const { return _shadow_transform; }

                bool Light::casts_shadows() const { return _cast_shadows; }

                Light& Light::set_position(const Magnum::Vector4& position)
                {
                    _position = position;
                    _transformed_position = position;
                    return *this;
                }

                Light& Light::set_transformed_position(const Magnum::Vector4& transformed_position)
                {
                    _transformed_position = transformed_position;
                    return *this;
                }

                Light& Light::set_material(const Material& material)
                {
                    _material = material;
                    return *this;
                }

                Light& Light::set_spot_direction(const Magnum::Vector3& spot_direction)
                {
                    _spot_direction = spot_direction;
                    _transformed_spot_direction = _spot_direction;
                    return *this;
                }

                Light& Light::set_transformed_spot_direction(const Magnum::Vector3& transformed_spot_direction)
                {
                    _transformed_spot_direction = transformed_spot_direction;
                    return *this;
                }

                Light& Light::set_spot_exponent(Magnum::Float spot_exponent)
                {
                    _spot_exponent = spot_exponent;
                    return *this;
                }

                Light& Light::set_spot_cut_off(Magnum::Float spot_cut_off)
                {
                    _spot_cut_off = spot_cut_off;
                    return *this;
                }

                Light& Light::set_attenuation(const Magnum::Vector4& attenuation)
                {
                    _attenuation = attenuation;
                    return *this;
                }

                Light& Light::set_shadow_matrix(const Magnum::Matrix4& shadowTransform)
                {
                    _shadow_transform = shadowTransform;
                    return *this;
                }

                Light& Light::set_casts_shadows(bool cast_shadows)
                {
                    _cast_shadows = cast_shadows;
                    return *this;
                }
            } // namespace gs
        } // namespace magnum
    } // namespace gui
} // namespace robot_dart
```


