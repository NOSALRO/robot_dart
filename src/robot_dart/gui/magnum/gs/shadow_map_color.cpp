#include "shadow_map_color.hpp"
#include "create_compatibility_shader.hpp"

#include <Magnum/GL/Texture.h>

namespace robot_dart {
    namespace gui {
        namespace magnum {
            namespace gs {
                ShadowMapColor::ShadowMapColor(ShadowMapColor::Flags flags) : _flags(flags)
                {
                    Corrade::Utility::Resource rs_shaders("RobotDARTShaders");

                    const Magnum::GL::Version version = Magnum::GL::Version::GL320;

                    Magnum::GL::Shader vert = Magnum::Shaders::Implementation::createCompatibilityShader(
                        rs_shaders, version, Magnum::GL::Shader::Type::Vertex);
                    Magnum::GL::Shader frag = Magnum::Shaders::Implementation::createCompatibilityShader(
                        rs_shaders, version, Magnum::GL::Shader::Type::Fragment);

                    vert.addSource(flags ? "#define TEXTURED\n" : "")
                        .addSource(rs_shaders.get("generic.glsl"))
                        .addSource(rs_shaders.get("ShadowMap.vert"));
                    frag.addSource(flags ? "#define TEXTURED\n" : "")
                        .addSource(rs_shaders.get("ShadowMapColor.frag"));

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

                ShadowMapColor::ShadowMapColor(Magnum::NoCreateT) noexcept : Magnum::GL::AbstractShaderProgram{Magnum::NoCreate} {}

                ShadowMapColor::Flags ShadowMapColor::flags() const { return _flags; }

                ShadowMapColor& ShadowMapColor::setTransformationMatrix(const Magnum::Matrix4& matrix)
                {
                    setUniform(_transformationMatrixUniform, matrix);
                    return *this;
                }

                ShadowMapColor& ShadowMapColor::setProjectionMatrix(const Magnum::Matrix4& matrix)
                {
                    setUniform(_projectionMatrixUniform, matrix);
                    return *this;
                }

                ShadowMapColor& ShadowMapColor::setMaterial(Material& material)
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