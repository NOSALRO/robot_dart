

# File material.cpp

[**File List**](files.md) **>** [**gs**](dir_2f8612d80f6bb57c97efd4c82e0df286.md) **>** [**material.cpp**](material_8cpp.md)

[Go to the documentation of this file](material_8cpp.md)

```C++

#include "material.hpp"

#include <Magnum/GL/Texture.h>

namespace robot_dart {
    namespace gui {
        namespace magnum {
            namespace gs {
                Material::Material() : _ambient(Magnum::Color4{0.f, 0.f, 0.f, 1.f}),
                                       _diffuse(Magnum::Color4{0.f, 0.f, 0.f, 1.f}),
                                       _specular(Magnum::Color4{0.f, 0.f, 0.f, 1.f}),
                                       _shininess(2000.f) {}

                Material::Material(const Magnum::Color4& ambient, const Magnum::Color4& diffuse,
                    const Magnum::Color4& specular, Magnum::Float shininess) : _ambient(ambient),
                                                                               _diffuse(diffuse),
                                                                               _specular(specular),
                                                                               _shininess(shininess) {}

                Material::Material(Magnum::GL::Texture2D* ambient_texture,
                    Magnum::GL::Texture2D* diffuse_texture,
                    Magnum::GL::Texture2D* specular_texture, Magnum::Float shininess) : _ambient(Magnum::Color4{0.f, 0.f, 0.f, 1.f}),
                                                                                       _diffuse(Magnum::Color4{0.f, 0.f, 0.f, 1.f}),
                                                                                       _specular(Magnum::Color4{0.f, 0.f, 0.f, 1.f}),
                                                                                       _shininess(shininess),
                                                                                       _ambient_texture(ambient_texture),
                                                                                       _diffuse_texture(diffuse_texture),
                                                                                       _specular_texture(specular_texture) {}

                Magnum::Color4& Material::ambient_color() { return _ambient; }
                Magnum::Color4 Material::ambient_color() const { return _ambient; }

                Magnum::Color4& Material::diffuse_color() { return _diffuse; }
                Magnum::Color4 Material::diffuse_color() const { return _diffuse; }

                Magnum::Color4& Material::specular_color() { return _specular; }
                Magnum::Color4 Material::specular_color() const { return _specular; }

                Magnum::Float& Material::shininess() { return _shininess; }
                Magnum::Float Material::shininess() const { return _shininess; }

                Magnum::GL::Texture2D* Material::ambient_texture() { return _ambient_texture; }
                Magnum::GL::Texture2D* Material::diffuse_texture() { return _diffuse_texture; }
                Magnum::GL::Texture2D* Material::specular_texture() { return _specular_texture; }

                bool Material::has_ambient_texture() const { return _ambient_texture != NULL; }
                bool Material::has_diffuse_texture() const { return _diffuse_texture != NULL; }
                bool Material::has_specular_texture() const { return _specular_texture != NULL; }

                Material& Material::set_ambient_color(const Magnum::Color4& ambient)
                {
                    _ambient = ambient;
                    return *this;
                }

                Material& Material::set_diffuse_color(const Magnum::Color4& diffuse)
                {
                    _diffuse = diffuse;
                    return *this;
                }

                Material& Material::set_specular_color(const Magnum::Color4& specular)
                {
                    _specular = specular;
                    return *this;
                }

                Material& Material::set_shininess(Magnum::Float shininess)
                {
                    _shininess = shininess;
                    return *this;
                }

                Material& Material::set_ambient_texture(Magnum::GL::Texture2D* ambient_texture)
                {
                    _ambient_texture = ambient_texture;
                    return *this;
                }

                Material& Material::set_diffuse_texture(Magnum::GL::Texture2D* diffuse_texture)
                {
                    _diffuse_texture = diffuse_texture;
                    return *this;
                }

                Material& Material::set_specular_texture(Magnum::GL::Texture2D* specular_texture)
                {
                    _specular_texture = specular_texture;
                    return *this;
                }
            } // namespace gs
        } // namespace magnum
    } // namespace gui
} // namespace robot_dart

```

