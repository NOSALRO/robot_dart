

# File shadow\_map.cpp

[**File List**](files.md) **>** [**gs**](dir_2f8612d80f6bb57c97efd4c82e0df286.md) **>** [**shadow\_map.cpp**](shadow__map_8cpp.md)

[Go to the documentation of this file](shadow__map_8cpp.md)


```C++
#include "shadow_map.hpp"
#include "create_compatibility_shader.hpp"

#include <Magnum/GL/Texture.h>

namespace robot_dart {
    namespace gui {
        namespace magnum {
            namespace gs {
                ShadowMap::ShadowMap(ShadowMap::Flags flags) : _flags(flags)
                {
                    Corrade::Utility::Resource rs_shaders("RobotDARTShaders");

                    const Magnum::GL::Version version = Magnum::GL::Version::GL320;

                    Magnum::GL::Shader vert = Magnum::Shaders::Implementation::createCompatibilityShader(
                        rs_shaders, version, Magnum::GL::Shader::Type::Vertex);
                    Magnum::GL::Shader frag = Magnum::Shaders::Implementation::createCompatibilityShader(
                        rs_shaders, version, Magnum::GL::Shader::Type::Fragment);

                    std::string defines = "#define POSITION_ATTRIBUTE_LOCATION " + std::to_string(Position::Location) + "\n";
                    defines += "#define TEXTURECOORDINATES_ATTRIBUTE_LOCATION " + std::to_string(TextureCoordinates::Location) + "\n";

                    vert.addSource(flags ? "#define TEXTURED\n" : "")
                        .addSource(defines)
                        .addSource(rs_shaders.get("ShadowMap.vert"));
                    frag.addSource(flags ? "#define TEXTURED\n" : "")
                        .addSource(rs_shaders.get("ShadowMap.frag"));

                    CORRADE_INTERNAL_ASSERT_OUTPUT(Magnum::GL::Shader::compile({vert, frag}));

                    attachShaders({vert, frag});

                    if (!Magnum::GL::Context::current().isExtensionSupported<Magnum::GL::Extensions::ARB::explicit_attrib_location>(version)) {
                        bindAttributeLocation(Position::Location, "position");
                        if (flags)
                            bindAttributeLocation(TextureCoordinates::Location, "textureCoords");
                    }

                    CORRADE_INTERNAL_ASSERT_OUTPUT(link());

                    if (!Magnum::GL::Context::current().isExtensionSupported<Magnum::GL::Extensions::ARB::explicit_uniform_location>(version)) {
                        _transformation_matrix_uniform = uniformLocation("transformationMatrix");
                        _projection_matrix_uniform = uniformLocation("projectionMatrix");
                        _diffuse_color_uniform = uniformLocation("diffuseColor");
                    }

                    if (!Magnum::GL::Context::current()
                             .isExtensionSupported<Magnum::GL::Extensions::ARB::shading_language_420pack>(version)
                        && flags) {
                        setUniform(uniformLocation("diffuseTexture"), DiffuseTextureLayer);
                    }
                }

                ShadowMap::ShadowMap(Magnum::NoCreateT) noexcept : Magnum::GL::AbstractShaderProgram{Magnum::NoCreate} {}

                ShadowMap::Flags ShadowMap::flags() const { return _flags; }

                ShadowMap& ShadowMap::set_transformation_matrix(const Magnum::Matrix4& matrix)
                {
                    setUniform(_transformation_matrix_uniform, matrix);
                    return *this;
                }

                ShadowMap& ShadowMap::set_projection_matrix(const Magnum::Matrix4& matrix)
                {
                    setUniform(_projection_matrix_uniform, matrix);
                    return *this;
                }

                ShadowMap& ShadowMap::set_material(Material& material)
                {
                    if (material.has_diffuse_texture() && (_flags & Flag::DiffuseTexture)) {
                        (*material.diffuse_texture()).bind(DiffuseTextureLayer);
                        setUniform(_diffuse_color_uniform, Magnum::Color4{1.0f});
                    }
                    else
                        setUniform(_diffuse_color_uniform, material.diffuse_color());

                    return *this;
                }
            } // namespace gs
        } // namespace magnum
    } // namespace gui
} // namespace robot_dart
```


