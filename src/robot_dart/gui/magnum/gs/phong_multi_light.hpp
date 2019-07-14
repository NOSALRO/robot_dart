#ifndef ROBOT_DART_GUI_MAGNUM_GS_PHONG_MULTI_LIGHT_HPP
#define ROBOT_DART_GUI_MAGNUM_GS_PHONG_MULTI_LIGHT_HPP

#include <robot_dart/gui/magnum/gs/create_compatibility_shader.hpp>
#include <robot_dart/gui/magnum/gs/light.hpp>

#include <Corrade/Containers/ArrayView.h>
#include <Corrade/Containers/Reference.h>
#include <Corrade/Utility/Assert.h>

#include <Magnum/GL/AbstractShaderProgram.h>
#include <Magnum/Math/Color.h>
#include <Magnum/Math/Matrix4.h>
#include <Magnum/Shaders/Generic.h>

namespace robot_dart {
    namespace gui {
        namespace magnum {
            namespace gs {

                namespace {
                    enum : Magnum::Int { AmbientTextureLayer = 0,
                        DiffuseTextureLayer = 1,
                        SpecularTextureLayer = 2 };
                }

                class PhongMultiLight : public Magnum::GL::AbstractShaderProgram {
                public:
                    using Position = Magnum::Shaders::Generic3D::Position;
                    using Normal = Magnum::Shaders::Generic3D::Normal;
                    using TextureCoordinates = Magnum::Shaders::Generic3D::TextureCoordinates;

                    enum class Flag : Magnum::UnsignedByte {
                        AmbientTexture = 1 << 0, /**< The shader uses ambient texture instead of color */
                        DiffuseTexture = 1 << 1, /**< The shader uses diffuse texture instead of color */
                        SpecularTexture = 1 << 2 /**< The shader uses specular texture instead of color */
                    };

                    using Flags = Magnum::Containers::EnumSet<Flag>;

                    explicit PhongMultiLight(Flags flags = {}, Magnum::Int maxLights = 10) : _flags(flags), _maxLights(maxLights)
                    {
                        Corrade::Utility::Resource rs_shaders("RobotDARTShaders");

#ifndef MAGNUM_TARGET_GLES
                        const Magnum::GL::Version version = Magnum::GL::Context::current().supportedVersion(
                            {Magnum::GL::Version::GL320, Magnum::GL::Version::GL310, Magnum::GL::Version::GL300, Magnum::GL::Version::GL210});
#else
                        const Magnum::GL::Version version
                            = Magnum::GL::Context::current().supportedVersion({Magnum::GL::Version::GLES300, Magnum::GL::Version::GLES200});
#endif

                        Magnum::GL::Shader vert = Magnum::Shaders::Implementation::createCompatibilityShader(
                            rs_shaders, version, Magnum::GL::Shader::Type::Vertex);
                        Magnum::GL::Shader frag = Magnum::Shaders::Implementation::createCompatibilityShader(
                            rs_shaders, version, Magnum::GL::Shader::Type::Fragment);

                        std::string definesFrag = "#define LIGHT_COUNT " + std::to_string(_maxLights) + "\n";

                        vert.addSource(flags ? "#define TEXTURED\n" : "")
                            .addSource(rs_shaders.get("generic.glsl"))
                            .addSource(rs_shaders.get("PhongMultiLight.vert"));
                        frag.addSource(flags & Flag::AmbientTexture ? "#define AMBIENT_TEXTURE\n" : "")
                            .addSource(flags & Flag::DiffuseTexture ? "#define DIFFUSE_TEXTURE\n" : "")
                            .addSource(flags & Flag::SpecularTexture ? "#define SPECULAR_TEXTURE\n" : "")
                            .addSource(definesFrag)
                            .addSource(rs_shaders.get("PhongMultiLight.frag"));

                        CORRADE_INTERNAL_ASSERT_OUTPUT(Magnum::GL::Shader::compile({vert, frag}));

                        attachShaders({vert, frag});

#ifndef MAGNUM_TARGET_GLES
                        if (!Magnum::GL::Context::current().isExtensionSupported<Magnum::GL::Extensions::ARB::explicit_attrib_location>(
                                version))
#else
                        if (!Magnum::GL::Context::current().isVersionSupported(Magnum::GL::Version::GLES300))
#endif
                        {
                            bindAttributeLocation(Position::Location, "position");
                            bindAttributeLocation(Normal::Location, "normal");
                            if (flags)
                                bindAttributeLocation(TextureCoordinates::Location, "textureCoordinates");
                        }

                        CORRADE_INTERNAL_ASSERT_OUTPUT(link());

#ifndef MAGNUM_TARGET_GLES
                        if (!Magnum::GL::Context::current()
                                 .isExtensionSupported<Magnum::GL::Extensions::ARB::explicit_uniform_location>(version))
#endif
                        {
                            _transformationMatrixUniform = uniformLocation("transformationMatrix");
                            _projectionMatrixUniform = uniformLocation("projectionMatrix");
                            _normalMatrixUniform = uniformLocation("normalMatrix");
                            _lightsUniform = uniformLocation("lights[0].position");
                            _ambientColorUniform = uniformLocation("ambientColor");
                            _diffuseColorUniform = uniformLocation("diffuseColor");
                            _specularColorUniform = uniformLocation("specularColor");
                            _shininessUniform = uniformLocation("shininess");
                        }

#ifndef MAGNUM_TARGET_GLES
                        if (flags
                            && !Magnum::GL::Context::current()
                                    .isExtensionSupported<Magnum::GL::Extensions::ARB::shading_language_420pack>(version))
#endif
                        {
                            if (flags & Flag::AmbientTexture)
                                setUniform(uniformLocation("ambientTexture"), AmbientTextureLayer);
                            if (flags & Flag::DiffuseTexture)
                                setUniform(uniformLocation("diffuseTexture"), DiffuseTextureLayer);
                            if (flags & Flag::SpecularTexture)
                                setUniform(uniformLocation("specularTexture"), SpecularTextureLayer);
                        }

/* Set defaults in OpenGL ES (for desktop they are set in shader code itself) */
#ifdef MAGNUM_TARGET_GLES
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
#endif

                        /* Lights defaults need to be set by code */
                        /* All lights are disabled i.e., color equal to black */
                        Light light;
                        for (Magnum::Int i = 0; i < _maxLights; i++)
                            setLight(i, light);
                    }

                    explicit PhongMultiLight(Magnum::NoCreateT) noexcept : Magnum::GL::AbstractShaderProgram{Magnum::NoCreate} {}

                    Flags flags() const { return _flags; }

                    PhongMultiLight& setMaterial(Material& material)
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

                    PhongMultiLight& setLight(Magnum::Int i, const Light& light)
                    {
                        CORRADE_INTERNAL_ASSERT(i >= 0 && i < _maxLights);
                        Magnum::Int locSize = 11;
                        Magnum::Vector4 attenuation = light.attenuation();

                        // light position
                        setUniform(_lightsUniform + i * locSize, light.transformedPosition());
                        // light material
                        setUniform(_lightsUniform + i * locSize + 1, light.material().ambientColor());
                        setUniform(_lightsUniform + i * locSize + 2, light.material().diffuseColor());
                        setUniform(_lightsUniform + i * locSize + 3, light.material().specularColor());
                        // spotlight properties
                        setUniform(_lightsUniform + i * locSize + 4, light.transformedSpotDirection());
                        setUniform(_lightsUniform + i * locSize + 5, light.spotExponent());
                        setUniform(_lightsUniform + i * locSize + 6, light.spotCutOff());
                        // intesity
                        setUniform(_lightsUniform + i * locSize + 7, attenuation[3]);
                        // constant attenuation term
                        setUniform(_lightsUniform + i * locSize + 8, attenuation[0]);
                        // linear attenuation term
                        setUniform(_lightsUniform + i * locSize + 9, attenuation[1]);
                        // quadratic attenuation term
                        setUniform(_lightsUniform + i * locSize + 10, attenuation[2]);

                        return *this;
                    }

                    PhongMultiLight& setTransformationMatrix(const Magnum::Matrix4& matrix)
                    {
                        setUniform(_transformationMatrixUniform, matrix);
                        return *this;
                    }

                    PhongMultiLight& setNormalMatrix(const Magnum::Matrix3x3& matrix)
                    {
                        setUniform(_normalMatrixUniform, matrix);
                        return *this;
                    }

                    PhongMultiLight& setProjectionMatrix(const Magnum::Matrix4& matrix)
                    {
                        setUniform(_projectionMatrixUniform, matrix);
                        return *this;
                    }

                    Magnum::Int maxLights() const { return _maxLights; }

                private:
                    Flags _flags;
                    Magnum::Int _maxLights = 10;
                    Magnum::Int _transformationMatrixUniform{0}, _projectionMatrixUniform{1}, _normalMatrixUniform{2},
                        _shininessUniform{3}, _ambientColorUniform{4}, _diffuseColorUniform{5}, _specularColorUniform{6},
                        _lightsUniform{7};
                };

                CORRADE_ENUMSET_OPERATORS(PhongMultiLight::Flags)
            } // namespace gs
        } // namespace magnum
    } // namespace gui
} // namespace robot_dart

#endif