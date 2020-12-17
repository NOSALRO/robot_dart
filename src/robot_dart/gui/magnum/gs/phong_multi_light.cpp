#include "phong_multi_light.hpp"
#include "create_compatibility_shader.hpp"

#include <Magnum/GL/CubeMapTextureArray.h>
#include <Magnum/GL/Texture.h>
#include <Magnum/GL/TextureArray.h>

namespace robot_dart {
    namespace gui {
        namespace magnum {
            namespace gs {
                PhongMultiLight::PhongMultiLight(PhongMultiLight::Flags flags, Magnum::Int max_lights) : _flags(flags), _max_lights(max_lights)
                {
                    Corrade::Utility::Resource rs_shaders("RobotDARTShaders");

                    const Magnum::GL::Version version = Magnum::GL::Version::GL320;

                    Magnum::GL::Shader vert = Magnum::Shaders::Implementation::createCompatibilityShader(
                        rs_shaders, version, Magnum::GL::Shader::Type::Vertex);
                    Magnum::GL::Shader frag = Magnum::Shaders::Implementation::createCompatibilityShader(
                        rs_shaders, version, Magnum::GL::Shader::Type::Fragment);

                    std::string defines = "#define LIGHT_COUNT " + std::to_string(_max_lights) + "\n";
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
                    _lights_matrices_uniform = uniformLocation("lightMatrices[0]");

                    if (!Magnum::GL::Context::current().isExtensionSupported<Magnum::GL::Extensions::ARB::explicit_uniform_location>(version)) {
                        _transformation_matrix_uniform = uniformLocation("transformationMatrix");
                        _projection_matrix_uniform = uniformLocation("projectionMatrix");
                        _camera_matrix_uniform = uniformLocation("cameraMatrix");
                        _normal_matrix_uniform = uniformLocation("normalMatrix");
                        _lights_uniform = uniformLocation("lights[0].position");
                        _lights_matrices_uniform = uniformLocation("lightMatrices[0]");
                        _ambient_color_uniform = uniformLocation("ambientColor");
                        _diffuse_color_uniform = uniformLocation("diffuseColor");
                        _specular_color_uniform = uniformLocation("specularColor");
                        _shininess_uniform = uniformLocation("shininess");
                        _far_plane_uniform = uniformLocation("farPlane");
                        _specular_strength_uniform = uniformLocation("specularStrength");
                        _is_shadowed_uniform = uniformLocation("isShadowed");
                        _transparent_shadows_uniform = uniformLocation("drawTransparentShadows");
                    }

                    if (!Magnum::GL::Context::current()
                             .isExtensionSupported<Magnum::GL::Extensions::ARB::shading_language_420pack>(version)) {
                        setUniform(uniformLocation("shadowTextures"), _shadow_textures_location);
                        setUniform(uniformLocation("cubeMapTextures"), _cube_map_textures_location);
                        setUniform(uniformLocation("shadowColorTextures"), _shadow_color_textures_location);
                        setUniform(uniformLocation("cubeMapColorTextures"), _cube_map_color_textures_location);
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
                        material.set_ambient_color(Magnum::Color4{1.0f});
                    else
                        material.set_ambient_color(Magnum::Color4{0.0f, 1.0f});

                    if (flags & Flag::DiffuseTexture)
                        material.set_diffuse_color(Magnum::Color4{1.0f});

                    material.set_specular_color(Magnum::Color4{1.0f});
                    material.set_shininess(80.0f);

                    set_material(material);

                    /* Lights defaults need to be set by code */
                    /* All lights are disabled i.e., color equal to black */
                    Light light;
                    for (Magnum::Int i = 0; i < _max_lights; i++)
                        set_light(i, light);
                }

                PhongMultiLight::PhongMultiLight(Magnum::NoCreateT) noexcept : Magnum::GL::AbstractShaderProgram{Magnum::NoCreate} {}

                PhongMultiLight::Flags PhongMultiLight::flags() const { return _flags; }

                PhongMultiLight& PhongMultiLight::set_material(Material& material)
                {
                    // TO-DO: Check if we should do this or let the user define the proper
                    // material
                    if (material.has_ambient_texture() && (_flags & Flag::AmbientTexture)) {
                        (*material.ambient_texture()).bind(AmbientTextureLayer);
                        setUniform(_ambient_color_uniform, Magnum::Color4{1.0f});
                    }
                    else
                        setUniform(_ambient_color_uniform, material.ambient_color());

                    if (material.has_diffuse_texture() && (_flags & Flag::DiffuseTexture)) {
                        (*material.diffuse_texture()).bind(DiffuseTextureLayer);
                        setUniform(_diffuse_color_uniform, Magnum::Color4{1.0f});
                    }
                    else
                        setUniform(_diffuse_color_uniform, material.diffuse_color());

                    if (material.has_specular_texture() && (_flags & Flag::SpecularTexture)) {
                        (*material.specular_texture()).bind(SpecularTextureLayer);
                        setUniform(_specular_color_uniform, Magnum::Color4{1.0f});
                    }
                    else
                        setUniform(_specular_color_uniform, material.specular_color());

                    setUniform(_shininess_uniform, material.shininess());

                    return *this;
                }

                PhongMultiLight& PhongMultiLight::set_light(Magnum::Int i, const Light& light)
                {
                    CORRADE_INTERNAL_ASSERT(i >= 0 && i < _max_lights);
                    Magnum::Vector4 attenuation = light.attenuation();

                    // light position
                    setUniform(_lights_uniform + i * _light_loc_size, light.transformed_position());
                    // light material
                    setUniform(_lights_uniform + i * _light_loc_size + 1, light.material().ambient_color());
                    setUniform(_lights_uniform + i * _light_loc_size + 2, light.material().diffuse_color());
                    setUniform(_lights_uniform + i * _light_loc_size + 3, light.material().specular_color());
                    // spotlight properties
                    setUniform(_lights_uniform + i * _light_loc_size + 4, light.transformed_spot_direction());
                    setUniform(_lights_uniform + i * _light_loc_size + 5, light.spot_exponent());
                    setUniform(_lights_uniform + i * _light_loc_size + 6, light.spot_cut_off());
                    // intesity
                    setUniform(_lights_uniform + i * _light_loc_size + 7, attenuation[3]);
                    // constant attenuation term
                    setUniform(_lights_uniform + i * _light_loc_size + 8, attenuation[0]);
                    // linear attenuation term
                    setUniform(_lights_uniform + i * _light_loc_size + 9, attenuation[1]);
                    // quadratic attenuation term
                    setUniform(_lights_uniform + i * _light_loc_size + 10, attenuation[2]);
                    // world position
                    setUniform(_lights_uniform + i * _light_loc_size + 11, light.position());
                    // casts shadows?
                    setUniform(_lights_uniform + i * _light_loc_size + 12, light.casts_shadows());

                    setUniform(_lights_matrices_uniform + i, light.shadow_matrix());

                    return *this;
                }

                PhongMultiLight& PhongMultiLight::set_transformation_matrix(const Magnum::Matrix4& matrix)
                {
                    setUniform(_transformation_matrix_uniform, matrix);
                    return *this;
                }

                PhongMultiLight& PhongMultiLight::set_camera_matrix(const Magnum::Matrix4& matrix)
                {
                    setUniform(_camera_matrix_uniform, matrix);
                    return *this;
                }

                PhongMultiLight& PhongMultiLight::set_normal_matrix(const Magnum::Matrix3x3& matrix)
                {
                    setUniform(_normal_matrix_uniform, matrix);
                    return *this;
                }

                PhongMultiLight& PhongMultiLight::set_projection_matrix(const Magnum::Matrix4& matrix)
                {
                    setUniform(_projection_matrix_uniform, matrix);
                    return *this;
                }

                PhongMultiLight& PhongMultiLight::set_far_plane(Magnum::Float far_plane)
                {
                    setUniform(_far_plane_uniform, far_plane);
                    return *this;
                }

                PhongMultiLight& PhongMultiLight::set_is_shadowed(bool shadows)
                {
                    setUniform(_is_shadowed_uniform, shadows);
                    return *this;
                }

                PhongMultiLight& PhongMultiLight::set_transparent_shadows(bool shadows)
                {
                    setUniform(_transparent_shadows_uniform, shadows);
                    return *this;
                }

                PhongMultiLight& PhongMultiLight::set_specular_strength(Magnum::Float specular_strength)
                {
                    setUniform(_specular_strength_uniform, std::max(0.f, specular_strength));
                    return *this;
                }

                PhongMultiLight& PhongMultiLight::bind_shadow_texture(Magnum::GL::Texture2DArray& texture)
                {
                    texture.bind(_shadow_textures_location);
                    return *this;
                }

                PhongMultiLight& PhongMultiLight::bind_shadow_color_texture(Magnum::GL::Texture2DArray& texture)
                {
                    texture.bind(_shadow_color_textures_location);
                    return *this;
                }

                PhongMultiLight& PhongMultiLight::bind_cube_map_texture(Magnum::GL::CubeMapTextureArray& texture)
                {
                    texture.bind(_cube_map_textures_location);
                    return *this;
                }

                PhongMultiLight& PhongMultiLight::bind_cube_map_color_texture(Magnum::GL::CubeMapTextureArray& texture)
                {
                    texture.bind(_cube_map_color_textures_location);
                    return *this;
                }

                Magnum::Int PhongMultiLight::max_lights() const { return _max_lights; }
            } // namespace gs
        } // namespace magnum
    } // namespace gui
} // namespace robot_dart