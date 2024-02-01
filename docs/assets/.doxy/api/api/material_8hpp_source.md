

# File material.hpp

[**File List**](files.md) **>** [**gs**](dir_2f8612d80f6bb57c97efd4c82e0df286.md) **>** [**material.hpp**](material_8hpp.md)

[Go to the documentation of this file](material_8hpp.md)

```C++

#ifndef ROBOT_DART_GUI_MAGNUM_GS_MATERIAL_HPP
#define ROBOT_DART_GUI_MAGNUM_GS_MATERIAL_HPP

#include <Corrade/Containers/Optional.h>

#include <Magnum/GL/GL.h>
#include <Magnum/Magnum.h>
#include <Magnum/Math/Color.h>

namespace robot_dart {
    namespace gui {
        namespace magnum {
            namespace gs {
                class Material {
                public:
                    Material();

                    Material(const Magnum::Color4& ambient, const Magnum::Color4& diffuse,
                        const Magnum::Color4& specular, Magnum::Float shininess);

                    Material(Magnum::GL::Texture2D* ambient_texture,
                        Magnum::GL::Texture2D* diffuse_texture,
                        Magnum::GL::Texture2D* specular_texture, Magnum::Float shininess);

                    Magnum::Color4& ambient_color();
                    Magnum::Color4 ambient_color() const;

                    Magnum::Color4& diffuse_color();
                    Magnum::Color4 diffuse_color() const;

                    Magnum::Color4& specular_color();
                    Magnum::Color4 specular_color() const;

                    Magnum::Float& shininess();
                    Magnum::Float shininess() const;

                    Magnum::GL::Texture2D* ambient_texture();
                    Magnum::GL::Texture2D* diffuse_texture();
                    Magnum::GL::Texture2D* specular_texture();

                    bool has_ambient_texture() const;
                    bool has_diffuse_texture() const;
                    bool has_specular_texture() const;

                    Material& set_ambient_color(const Magnum::Color4& ambient);
                    Material& set_diffuse_color(const Magnum::Color4& diffuse);
                    Material& set_specular_color(const Magnum::Color4& specular);
                    Material& set_shininess(Magnum::Float shininess);

                    Material& set_ambient_texture(Magnum::GL::Texture2D* ambient_texture);
                    Material& set_diffuse_texture(Magnum::GL::Texture2D* diffuse_texture);
                    Material& set_specular_texture(Magnum::GL::Texture2D* specular_texture);

                protected:
                    Magnum::Color4 _ambient, _diffuse, _specular;
                    Magnum::Float _shininess;
                    Magnum::GL::Texture2D* _ambient_texture = NULL;
                    Magnum::GL::Texture2D* _diffuse_texture = NULL;
                    Magnum::GL::Texture2D* _specular_texture = NULL;
                };
            } // namespace gs
        } // namespace magnum
    } // namespace gui
} // namespace robot_dart

#endif

```

