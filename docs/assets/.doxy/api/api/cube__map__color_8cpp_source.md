

# File cube\_map\_color.cpp

[**File List**](files.md) **>** [**gs**](dir_2f8612d80f6bb57c97efd4c82e0df286.md) **>** [**cube\_map\_color.cpp**](cube__map__color_8cpp.md)

[Go to the documentation of this file](cube__map__color_8cpp.md)

```C++

#include "cube_map_color.hpp"
#include "create_compatibility_shader.hpp"

#include <Magnum/GL/CubeMapTextureArray.h>
#include <Magnum/GL/Texture.h>

namespace robot_dart {
    namespace gui {
        namespace magnum {
            namespace gs {
                CubeMapColor::CubeMapColor(CubeMapColor::Flags flags) : _flags(flags)
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
                        .addSource(rs_shaders.get("CubeMapColor.frag"));

                    CORRADE_INTERNAL_ASSERT_OUTPUT(Magnum::GL::Shader::compile({vert, geom, frag}));

                    attachShaders({vert, geom, frag});

                    if (!Magnum::GL::Context::current().isExtensionSupported<Magnum::GL::Extensions::ARB::explicit_attrib_location>(version)) {
                        bindAttributeLocation(Position::Location, "position");
                        if (flags)
                            bindAttributeLocation(TextureCoordinates::Location, "textureCoords");
                    }

                    CORRADE_INTERNAL_ASSERT_OUTPUT(link());

                    if (!Magnum::GL::Context::current().isExtensionSupported<Magnum::GL::Extensions::ARB::explicit_uniform_location>(version)) {
                        _transformation_matrix_uniform = uniformLocation("transformationMatrix");
                        _shadow_matrices_uniform = uniformLocation("shadowMatrices[0]");
                        _light_position_uniform = uniformLocation("lightPosition");
                        _far_plane_uniform = uniformLocation("farPlane");
                        _light_index_uniform = uniformLocation("lightIndex");
                        _diffuse_color_uniform = uniformLocation("diffuseColor");
                    }

                    if (!Magnum::GL::Context::current()
                             .isExtensionSupported<Magnum::GL::Extensions::ARB::shading_language_420pack>(version)) {
                        setUniform(uniformLocation("cubeMapTextures"), _cube_map_textures_location);
                        if (flags) {
                            if (flags & Flag::DiffuseTexture)
                                setUniform(uniformLocation("diffuseTexture"), DiffuseTextureLayer);
                        }
                    }
                }

                CubeMapColor::CubeMapColor(Magnum::NoCreateT) noexcept : Magnum::GL::AbstractShaderProgram{Magnum::NoCreate} {}

                CubeMapColor::Flags CubeMapColor::flags() const { return _flags; }

                CubeMapColor& CubeMapColor::set_transformation_matrix(const Magnum::Matrix4& matrix)
                {
                    setUniform(_transformation_matrix_uniform, matrix);
                    return *this;
                }

                CubeMapColor& CubeMapColor::set_shadow_matrices(Magnum::Matrix4 matrices[6])
                {
                    for (size_t i = 0; i < 6; i++)
                        setUniform(_shadow_matrices_uniform + i, matrices[i]);
                    return *this;
                }

                CubeMapColor& CubeMapColor::set_light_position(const Magnum::Vector3& position)
                {
                    setUniform(_light_position_uniform, position);
                    return *this;
                }

                CubeMapColor& CubeMapColor::set_far_plane(Magnum::Float far_plane)
                {
                    setUniform(_far_plane_uniform, far_plane);
                    return *this;
                }

                CubeMapColor& CubeMapColor::set_light_index(Magnum::Int index)
                {
                    setUniform(_light_index_uniform, index);
                    return *this;
                }

                CubeMapColor& CubeMapColor::set_material(Material& material)
                {
                    if (material.has_diffuse_texture() && (_flags & Flag::DiffuseTexture)) {
                        (*material.diffuse_texture()).bind(DiffuseTextureLayer);
                        setUniform(_diffuse_color_uniform, Magnum::Color4{1.0f});
                    }
                    else
                        setUniform(_diffuse_color_uniform, material.diffuse_color());

                    return *this;
                }

                CubeMapColor& CubeMapColor::bind_cube_map_texture(Magnum::GL::CubeMapTextureArray& texture)
                {
                    texture.bind(_cube_map_textures_location);
                    return *this;
                }
            } // namespace gs
        } // namespace magnum
    } // namespace gui
} // namespace robot_dart

```

