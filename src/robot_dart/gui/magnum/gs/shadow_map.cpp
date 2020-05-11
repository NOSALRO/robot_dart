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
                        _transformationMatrixUniform = uniformLocation("transformationMatrix");
                        _projectionMatrixUniform = uniformLocation("projectionMatrix");
                        _diffuseColorUniform = uniformLocation("diffuseColor");
                    }

                    if (!Magnum::GL::Context::current()
                             .isExtensionSupported<Magnum::GL::Extensions::ARB::shading_language_420pack>(version)
                        && flags) {
                        setUniform(uniformLocation("diffuseTexture"), DiffuseTextureLayer);
                    }
                }

                ShadowMap::ShadowMap(Magnum::NoCreateT) noexcept : Magnum::GL::AbstractShaderProgram{Magnum::NoCreate} {}

                ShadowMap::Flags ShadowMap::flags() const { return _flags; }

                ShadowMap& ShadowMap::setTransformationMatrix(const Magnum::Matrix4& matrix)
                {
                    setUniform(_transformationMatrixUniform, matrix);
                    return *this;
                }

                ShadowMap& ShadowMap::setProjectionMatrix(const Magnum::Matrix4& matrix)
                {
                    setUniform(_projectionMatrixUniform, matrix);
                    return *this;
                }

                ShadowMap& ShadowMap::setMaterial(Material& material)
                {
                    if (material.hasDiffuseTexture() && (_flags & Flag::DiffuseTexture)) {
                        (*material.diffuseTexture()).bind(DiffuseTextureLayer);
                        setUniform(_diffuseColorUniform, Magnum::Color4{1.0f});
                    }
                    else
                        setUniform(_diffuseColorUniform, material.diffuseColor());

                    return *this;
                }
            } // namespace gs
        } // namespace magnum
    } // namespace gui
} // namespace robot_dart