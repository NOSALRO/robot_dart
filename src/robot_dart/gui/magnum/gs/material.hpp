#ifndef ROBOT_DART_GUI_MAGNUM_GS_MATERIAL_HPP
#define ROBOT_DART_GUI_MAGNUM_GS_MATERIAL_HPP

#include <Corrade/Containers/Optional.h>

#include <Magnum/Math/Color.h>

namespace robot_dart {
    namespace gui {
        namespace magnum {
            namespace gs {
                class Material {
                public:
                    Material() : _ambient(Magnum::Color4{0.f, 0.f, 0.f, 1.f}),
                                 _diffuse(Magnum::Color4{0.f, 0.f, 0.f, 1.f}),
                                 _specular(Magnum::Color4{0.f, 0.f, 0.f, 1.f}),
                                 _shininess(2000.f) {}

                    Material(const Magnum::Color4& ambient, const Magnum::Color4& diffuse,
                        const Magnum::Color4& specular, Magnum::Float shininess) : _ambient(ambient),
                                                                                   _diffuse(diffuse),
                                                                                   _specular(specular),
                                                                                   _shininess(shininess) {}

                    Material(Magnum::GL::Texture2D* ambientTexture,
                        Magnum::GL::Texture2D* diffuseTexture,
                        Magnum::GL::Texture2D* specularTexture, Magnum::Float shininess) : _ambient(Magnum::Color4{0.f, 0.f, 0.f, 1.f}),
                                                                                           _diffuse(Magnum::Color4{0.f, 0.f, 0.f, 1.f}),
                                                                                           _specular(Magnum::Color4{0.f, 0.f, 0.f, 1.f}),
                                                                                           _shininess(shininess),
                                                                                           _ambientTexture(ambientTexture),
                                                                                           _diffuseTexture(diffuseTexture),
                                                                                           _specularTexture(specularTexture) {}

                    Magnum::Color4& ambientColor() { return _ambient; }
                    Magnum::Color4 ambientColor() const { return _ambient; }

                    Magnum::Color4& diffuseColor() { return _diffuse; }
                    Magnum::Color4 diffuseColor() const { return _diffuse; }

                    Magnum::Color4& specularColor() { return _specular; }
                    Magnum::Color4 specularColor() const { return _specular; }

                    Magnum::Float& shininess() { return _shininess; }
                    Magnum::Float shininess() const { return _shininess; }

                    Magnum::GL::Texture2D* ambientTexture() { return _ambientTexture; }
                    Magnum::GL::Texture2D* diffuseTexture() { return _diffuseTexture; }
                    Magnum::GL::Texture2D* specularTexture() { return _specularTexture; }

                    bool hasAmbientTexture() { return _ambientTexture != NULL; }
                    bool hasDiffuseTexture() { return _diffuseTexture != NULL; }
                    bool hasSpecularTexture() { return _specularTexture != NULL; }

                    Material& setAmbientColor(const Magnum::Color4& ambient)
                    {
                        _ambient = ambient;
                        return *this;
                    }

                    Material& setDiffuseColor(const Magnum::Color4& diffuse)
                    {
                        _diffuse = diffuse;
                        return *this;
                    }

                    Material& setSpecularColor(const Magnum::Color4& specular)
                    {
                        _specular = specular;
                        return *this;
                    }

                    Material& setShininess(Magnum::Float shininess)
                    {
                        _shininess = shininess;
                        return *this;
                    }

                    Material& setAmbientTexture(Magnum::GL::Texture2D* ambientTexture)
                    {
                        _ambientTexture = ambientTexture;
                        return *this;
                    }

                    Material& setDiffuseTexture(Magnum::GL::Texture2D* diffuseTexture)
                    {
                        _diffuseTexture = diffuseTexture;
                        return *this;
                    }

                    Material& setSpecularTexture(Magnum::GL::Texture2D* specularTexture)
                    {
                        _specularTexture = specularTexture;
                        return *this;
                    }

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