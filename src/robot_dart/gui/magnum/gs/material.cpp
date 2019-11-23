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

                Material::Material(Magnum::GL::Texture2D* ambientTexture,
                    Magnum::GL::Texture2D* diffuseTexture,
                    Magnum::GL::Texture2D* specularTexture, Magnum::Float shininess) : _ambient(Magnum::Color4{0.f, 0.f, 0.f, 1.f}),
                                                                                       _diffuse(Magnum::Color4{0.f, 0.f, 0.f, 1.f}),
                                                                                       _specular(Magnum::Color4{0.f, 0.f, 0.f, 1.f}),
                                                                                       _shininess(shininess),
                                                                                       _ambientTexture(ambientTexture),
                                                                                       _diffuseTexture(diffuseTexture),
                                                                                       _specularTexture(specularTexture) {}

                Magnum::Color4& Material::ambientColor() { return _ambient; }
                Magnum::Color4 Material::ambientColor() const { return _ambient; }

                Magnum::Color4& Material::diffuseColor() { return _diffuse; }
                Magnum::Color4 Material::diffuseColor() const { return _diffuse; }

                Magnum::Color4& Material::specularColor() { return _specular; }
                Magnum::Color4 Material::specularColor() const { return _specular; }

                Magnum::Float& Material::shininess() { return _shininess; }
                Magnum::Float Material::shininess() const { return _shininess; }

                Magnum::GL::Texture2D* Material::ambientTexture() { return _ambientTexture; }
                Magnum::GL::Texture2D* Material::diffuseTexture() { return _diffuseTexture; }
                Magnum::GL::Texture2D* Material::specularTexture() { return _specularTexture; }

                bool Material::hasAmbientTexture() const { return _ambientTexture != NULL; }
                bool Material::hasDiffuseTexture() const { return _diffuseTexture != NULL; }
                bool Material::hasSpecularTexture() const { return _specularTexture != NULL; }

                Material& Material::setAmbientColor(const Magnum::Color4& ambient)
                {
                    _ambient = ambient;
                    return *this;
                }

                Material& Material::setDiffuseColor(const Magnum::Color4& diffuse)
                {
                    _diffuse = diffuse;
                    return *this;
                }

                Material& Material::setSpecularColor(const Magnum::Color4& specular)
                {
                    _specular = specular;
                    return *this;
                }

                Material& Material::setShininess(Magnum::Float shininess)
                {
                    _shininess = shininess;
                    return *this;
                }

                Material& Material::setAmbientTexture(Magnum::GL::Texture2D* ambientTexture)
                {
                    _ambientTexture = ambientTexture;
                    return *this;
                }

                Material& Material::setDiffuseTexture(Magnum::GL::Texture2D* diffuseTexture)
                {
                    _diffuseTexture = diffuseTexture;
                    return *this;
                }

                Material& Material::setSpecularTexture(Magnum::GL::Texture2D* specularTexture)
                {
                    _specularTexture = specularTexture;
                    return *this;
                }
            } // namespace gs
        } // namespace magnum
    } // namespace gui
} // namespace robot_dart