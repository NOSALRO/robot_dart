#include "cube_map.hpp"
#include "create_compatibility_shader.hpp"

#include <Magnum/GL/Texture.h>

namespace robot_dart {
    namespace gui {
        namespace magnum {
            namespace gs {
                CubeMap::CubeMap(CubeMap::Flags flags) : _flags(flags)
                {
                    Corrade::Utility::Resource rs_shaders("RobotDARTShaders");

                    const Magnum::GL::Version version = Magnum::GL::Version::GL320;

                    Magnum::GL::Shader vert = Magnum::Shaders::Implementation::createCompatibilityShader(
                        rs_shaders, version, Magnum::GL::Shader::Type::Vertex);
                    Magnum::GL::Shader geom = Magnum::Shaders::Implementation::createCompatibilityShader(
                        rs_shaders, version, Magnum::GL::Shader::Type::Geometry);
                    Magnum::GL::Shader frag = Magnum::Shaders::Implementation::createCompatibilityShader(
                        rs_shaders, version, Magnum::GL::Shader::Type::Fragment);

                    std::string defines = "#define POSITION_ATTRIBUTE_LOCATION " + std::to_string(Position::Location) + "\n";
                    defines += "#define TEXTURECOORDINATES_ATTRIBUTE_LOCATION " + std::to_string(TextureCoordinates::Location) + "\n";

                    vert.addSource(flags ? "#define TEXTURED\n" : "")
                        .addSource(defines)
                        .addSource(rs_shaders.get("CubeMap.vert"));
                    geom.addSource(flags ? "#define TEXTURED\n" : "")
                        .addSource(rs_shaders.get("CubeMap.geom"));
                    frag.addSource(flags ? "#define TEXTURED\n" : "")
                        .addSource(rs_shaders.get("CubeMap.frag"));

                    CORRADE_INTERNAL_ASSERT_OUTPUT(Magnum::GL::Shader::compile({vert, geom, frag}));

                    attachShaders({vert, geom, frag});

                    if (!Magnum::GL::Context::current().isExtensionSupported<Magnum::GL::Extensions::ARB::explicit_attrib_location>(version)) {
                        bindAttributeLocation(Position::Location, "position");
                        if (flags)
                            bindAttributeLocation(TextureCoordinates::Location, "textureCoords");
                    }

                    CORRADE_INTERNAL_ASSERT_OUTPUT(link());

                    if (!Magnum::GL::Context::current().isExtensionSupported<Magnum::GL::Extensions::ARB::explicit_uniform_location>(version)) {
                        _transformationMatrixUniform = uniformLocation("transformationMatrix");
                        _shadowMatricesUniform = uniformLocation("shadowMatrices[0]");
                        _lightPositionUniform = uniformLocation("lightPosition");
                        _farPlaneUniform = uniformLocation("farPlane");
                        _lightIndexUniform = uniformLocation("lightIndex");
                        _diffuseColorUniform = uniformLocation("diffuseColor");
                    }
                }

                CubeMap::CubeMap(Magnum::NoCreateT) noexcept : Magnum::GL::AbstractShaderProgram{Magnum::NoCreate} {}

                CubeMap::Flags CubeMap::flags() const { return _flags; }

                CubeMap& CubeMap::setTransformationMatrix(const Magnum::Matrix4& matrix)
                {
                    setUniform(_transformationMatrixUniform, matrix);
                    return *this;
                }

                CubeMap& CubeMap::setShadowMatrices(Magnum::Matrix4 matrices[6])
                {
                    for (size_t i = 0; i < 6; i++)
                        setUniform(_shadowMatricesUniform + i, matrices[i]);
                    return *this;
                }

                CubeMap& CubeMap::setLightPosition(const Magnum::Vector3& position)
                {
                    setUniform(_lightPositionUniform, position);
                    return *this;
                }

                CubeMap& CubeMap::setFarPlane(Magnum::Float farPlane)
                {
                    setUniform(_farPlaneUniform, farPlane);
                    return *this;
                }

                CubeMap& CubeMap::setLightIndex(Magnum::Int index)
                {
                    setUniform(_lightIndexUniform, index);
                    return *this;
                }

                CubeMap& CubeMap::setMaterial(Material& material)
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