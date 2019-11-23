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

                    Material(Magnum::GL::Texture2D* ambientTexture,
                        Magnum::GL::Texture2D* diffuseTexture,
                        Magnum::GL::Texture2D* specularTexture, Magnum::Float shininess);

                    Magnum::Color4& ambientColor();
                    Magnum::Color4 ambientColor() const;

                    Magnum::Color4& diffuseColor();
                    Magnum::Color4 diffuseColor() const;

                    Magnum::Color4& specularColor();
                    Magnum::Color4 specularColor() const;

                    Magnum::Float& shininess();
                    Magnum::Float shininess() const;

                    Magnum::GL::Texture2D* ambientTexture();
                    Magnum::GL::Texture2D* diffuseTexture();
                    Magnum::GL::Texture2D* specularTexture();

                    bool hasAmbientTexture() const;
                    bool hasDiffuseTexture() const;
                    bool hasSpecularTexture() const;

                    Material& setAmbientColor(const Magnum::Color4& ambient);
                    Material& setDiffuseColor(const Magnum::Color4& diffuse);
                    Material& setSpecularColor(const Magnum::Color4& specular);
                    Material& setShininess(Magnum::Float shininess);

                    Material& setAmbientTexture(Magnum::GL::Texture2D* ambientTexture);
                    Material& setDiffuseTexture(Magnum::GL::Texture2D* diffuseTexture);
                    Material& setSpecularTexture(Magnum::GL::Texture2D* specularTexture);

                protected:
                    Magnum::Color4 _ambient, _diffuse, _specular;
                    Magnum::Float _shininess;
                    Magnum::GL::Texture2D* _ambientTexture = NULL;
                    Magnum::GL::Texture2D* _diffuseTexture = NULL;
                    Magnum::GL::Texture2D* _specularTexture = NULL;
                };
            } // namespace gs
        } // namespace magnum
    } // namespace gui
} // namespace robot_dart

#endif