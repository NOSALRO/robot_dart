#include "phong_multi_light.hpp"
#include "create_compatibility_shader.hpp"

#include <Magnum/GL/CubeMapTextureArray.h>
#include <Magnum/GL/Texture.h>
#include <Magnum/GL/TextureArray.h>

namespace robot_dart {
    namespace gui {
        namespace magnum {
            namespace gs {
                PhongMultiLight::PhongMultiLight(PhongMultiLight::Flags flags, Magnum::Int maxLights) : _flags(flags), _maxLights(maxLights)
                {
                    Corrade::Utility::Resource rs_shaders("RobotDARTShaders");

                    const Magnum::GL::Version version = Magnum::GL::Version::GL320;

                    Magnum::GL::Shader vert = Magnum::Shaders::Implementation::createCompatibilityShader(
                        rs_shaders, version, Magnum::GL::Shader::Type::Vertex);
                    Magnum::GL::Shader frag = Magnum::Shaders::Implementation::createCompatibilityShader(
                        rs_shaders, version, Magnum::GL::Shader::Type::Fragment);

                    std::string defines = "#define LIGHT_COUNT " + std::to_string(_maxLights) + "\n";
                    defines += "#define POSITION_ATTRIBUTE_LOCATION " + std::to_string(Position::Location) + "\n";
                    defines += "#define NORMAL_ATTRIBUTE_LOCATION " + std::to_string(Normal::Location) + "\n";
                    defines += "#define TEXTURECOORDINATES_ATTRIBUTE_LOCATION " + std::to_string(TextureCoordinates::Location) + "\n";

                    vert.addSource(flags ? "#define TEXTURED\n" : "")
                        .addSource(defines)
                        .addSource(rs_shaders.get("PhongMultiLight.vert"));
                    frag.addSource(flags & Flag::AmbientTexture ? "#define AMBIENT_TEXTURE\n" : "")
                        .addSource(flags & Flag::DiffuseTexture ? "#define DIFFUSE_TEXTURE\n" : "")
                        .addSource(flags & Flag::SpecularTexture ? "#define SPECULAR_TEXTURE\n" : "")
                        .addSource(defines)
                        .addSource(rs_shaders.get("PhongMultiLight.frag"));

                    CORRADE_INTERNAL_ASSERT_OUTPUT(Magnum::GL::Shader::compile({vert, frag}));

                    attachShaders({vert, frag});

                    if (!Magnum::GL::Context::current().isExtensionSupported<Magnum::GL::Extensions::ARB::explicit_attrib_location>(version)) {
                        bindAttributeLocation(Position::Location, "position");
                        bindAttributeLocation(Normal::Location, "normal");
                        if (flags)
                            bindAttributeLocation(TextureCoordinates::Location, "textureCoords");
                    }

                    CORRADE_INTERNAL_ASSERT_OUTPUT(link());

                    /* Get light matrices uniform */
                    _lightsMatricesUniform = uniformLocation("lightMatrices[0]");

                    if (!Magnum::GL::Context::current().isExtensionSupported<Magnum::GL::Extensions::ARB::explicit_uniform_location>(version)) {
                        _transformationMatrixUniform = uniformLocation("transformationMatrix");
                        _projectionMatrixUniform = uniformLocation("projectionMatrix");
                        _cameraMatrixUniform = uniformLocation("cameraMatrix");
                        _normalMatrixUniform = uniformLocation("normalMatrix");
                        _lightsUniform = uniformLocation("lights[0].position");
                        _lightsMatricesUniform = uniformLocation("lightMatrices[0]");
                        _ambientColorUniform = uniformLocation("ambientColor");
                        _diffuseColorUniform = uniformLocation("diffuseColor");
                        _specularColorUniform = uniformLocation("specularColor");
                        _shininessUniform = uniformLocation("shininess");
                        _farPlaneUniform = uniformLocation("farPlane");
                        _isShadowedUniform = uniformLocation("isShadowed");
                        _drawTransparentShadowsUniform = uniformLocation("drawTransparentShadows");
                    }

                    if (!Magnum::GL::Context::current()
                             .isExtensionSupported<Magnum::GL::Extensions::ARB::shading_language_420pack>(version)) {
                        setUniform(uniformLocation("shadowTextures"), _shadowTexturesLocation);
                        setUniform(uniformLocation("cubeMapTextures"), _cubeMapTexturesLocation);
                        setUniform(uniformLocation("shadowColorTextures"), _shadowColorTexturesLocation);
                        setUniform(uniformLocation("cubeMapColorTextures"), _cubeMapColorTexturesLocation);
                        if (flags) {
                            if (flags & Flag::AmbientTexture)
                                setUniform(uniformLocation("ambientTexture"), AmbientTextureLayer);
                            if (flags & Flag::DiffuseTexture)
                                setUniform(uniformLocation("diffuseTexture"), DiffuseTextureLayer);
                            if (flags & Flag::SpecularTexture)
                                setUniform(uniformLocation("specularTexture"), SpecularTextureLayer);
                        }
                    }

                    /* Set defaults (normally they are set in shader code itself, but just in case) */
                    Material material;

                    /* Default to fully opaque white so we can see the textures */
                    if (flags & Flag::AmbientTexture)
                        material.setAmbientColor(Magnum::Color4{1.0f});
                    else
                        material.setAmbientColor(Magnum::Color4{0.0f, 1.0f});

                    if (flags & Flag::DiffuseTexture)
                        material.setDiffuseColor(Magnum::Color4{1.0f});

                    material.setSpecularColor(Magnum::Color4{1.0f});
                    material.setShininess(80.0f);

                    setMaterial(material);

                    /* Lights defaults need to be set by code */
                    /* All lights are disabled i.e., color equal to black */
                    Light light;
                    for (Magnum::Int i = 0; i < _maxLights; i++)
                        setLight(i, light);
                }

                PhongMultiLight::PhongMultiLight(Magnum::NoCreateT) noexcept : Magnum::GL::AbstractShaderProgram{Magnum::NoCreate} {}

                PhongMultiLight::Flags PhongMultiLight::flags() const { return _flags; }

                PhongMultiLight& PhongMultiLight::setMaterial(Material& material)
                {
                    // TO-DO: Check if we should do this or let the user define the proper
                    // material
                    if (material.hasAmbientTexture() && (_flags & Flag::AmbientTexture)) {
                        (*material.ambientTexture()).bind(AmbientTextureLayer);
                        setUniform(_ambientColorUniform, Magnum::Color4{1.0f});
                    }
                    else
                        setUniform(_ambientColorUniform, material.ambientColor());

                    if (material.hasDiffuseTexture() && (_flags & Flag::DiffuseTexture)) {
                        (*material.diffuseTexture()).bind(DiffuseTextureLayer);
                        setUniform(_diffuseColorUniform, Magnum::Color4{1.0f});
                    }
                    else
                        setUniform(_diffuseColorUniform, material.diffuseColor());

                    if (material.hasSpecularTexture() && (_flags & Flag::SpecularTexture)) {
                        (*material.specularTexture()).bind(SpecularTextureLayer);
                        setUniform(_specularColorUniform, Magnum::Color4{1.0f});
                    }
                    else
                        setUniform(_specularColorUniform, material.specularColor());

                    setUniform(_shininessUniform, material.shininess());

                    return *this;
                }

                PhongMultiLight& PhongMultiLight::setLight(Magnum::Int i, const Light& light)
                {
                    CORRADE_INTERNAL_ASSERT(i >= 0 && i < _maxLights);
                    Magnum::Vector4 attenuation = light.attenuation();

                    // light position
                    setUniform(_lightsUniform + i * _lightLocSize, light.transformedPosition());
                    // light material
                    setUniform(_lightsUniform + i * _lightLocSize + 1, light.material().ambientColor());
                    setUniform(_lightsUniform + i * _lightLocSize + 2, light.material().diffuseColor());
                    setUniform(_lightsUniform + i * _lightLocSize + 3, light.material().specularColor());
                    // spotlight properties
                    setUniform(_lightsUniform + i * _lightLocSize + 4, light.transformedSpotDirection());
                    setUniform(_lightsUniform + i * _lightLocSize + 5, light.spotExponent());
                    setUniform(_lightsUniform + i * _lightLocSize + 6, light.spotCutOff());
                    // intesity
                    setUniform(_lightsUniform + i * _lightLocSize + 7, attenuation[3]);
                    // constant attenuation term
                    setUniform(_lightsUniform + i * _lightLocSize + 8, attenuation[0]);
                    // linear attenuation term
                    setUniform(_lightsUniform + i * _lightLocSize + 9, attenuation[1]);
                    // quadratic attenuation term
                    setUniform(_lightsUniform + i * _lightLocSize + 10, attenuation[2]);
                    // world position
                    setUniform(_lightsUniform + i * _lightLocSize + 11, light.position());

                    setUniform(_lightsMatricesUniform + i, light.shadowMatrix());

                    return *this;
                }

                PhongMultiLight& PhongMultiLight::setTransformationMatrix(const Magnum::Matrix4& matrix)
                {
                    setUniform(_transformationMatrixUniform, matrix);
                    return *this;
                }

                PhongMultiLight& PhongMultiLight::setCameraMatrix(const Magnum::Matrix4& matrix)
                {
                    setUniform(_cameraMatrixUniform, matrix);
                    return *this;
                }

                PhongMultiLight& PhongMultiLight::setNormalMatrix(const Magnum::Matrix3x3& matrix)
                {
                    setUniform(_normalMatrixUniform, matrix);
                    return *this;
                }

                PhongMultiLight& PhongMultiLight::setProjectionMatrix(const Magnum::Matrix4& matrix)
                {
                    setUniform(_projectionMatrixUniform, matrix);
                    return *this;
                }

                PhongMultiLight& PhongMultiLight::setFarPlane(Magnum::Float farPlane)
                {
                    setUniform(_farPlaneUniform, farPlane);
                    return *this;
                }

                PhongMultiLight& PhongMultiLight::setIsShadowed(bool shadows)
                {
                    setUniform(_isShadowedUniform, shadows);
                    return *this;
                }

                PhongMultiLight& PhongMultiLight::setTransparentShadows(bool shadows)
                {
                    setUniform(_drawTransparentShadowsUniform, shadows);
                    return *this;
                }

                PhongMultiLight& PhongMultiLight::bindShadowTexture(Magnum::GL::Texture2DArray& texture)
                {
                    texture.bind(_shadowTexturesLocation);
                    return *this;
                }

                PhongMultiLight& PhongMultiLight::bindShadowColorTexture(Magnum::GL::Texture2DArray& texture)
                {
                    texture.bind(_shadowColorTexturesLocation);
                    return *this;
                }

                PhongMultiLight& PhongMultiLight::bindCubeMapTexture(Magnum::GL::CubeMapTextureArray& texture)
                {
                    texture.bind(_cubeMapTexturesLocation);
                    return *this;
                }

                PhongMultiLight& PhongMultiLight::bindCubeMapColorTexture(Magnum::GL::CubeMapTextureArray& texture)
                {
                    texture.bind(_cubeMapColorTexturesLocation);
                    return *this;
                }

                Magnum::Int PhongMultiLight::maxLights() const { return _maxLights; }
            } // namespace gs
        } // namespace magnum
    } // namespace gui
} // namespace robot_dart