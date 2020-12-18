#ifndef ROBOT_DART_GUI_MAGNUM_GS_PHONG_MULTI_LIGHT_HPP
#define ROBOT_DART_GUI_MAGNUM_GS_PHONG_MULTI_LIGHT_HPP

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

                    explicit PhongMultiLight(Flags flags = {}, Magnum::Int max_lights = 10);
                    explicit PhongMultiLight(Magnum::NoCreateT) noexcept;

                    Flags flags() const;

                    PhongMultiLight& set_material(Material& material);
                    PhongMultiLight& set_light(Magnum::Int i, const Light& light);

                    PhongMultiLight& set_transformation_matrix(const Magnum::Matrix4& matrix);
                    PhongMultiLight& set_camera_matrix(const Magnum::Matrix4& matrix);
                    PhongMultiLight& set_normal_matrix(const Magnum::Matrix3x3& matrix);
                    PhongMultiLight& set_projection_matrix(const Magnum::Matrix4& matrix);

                    PhongMultiLight& set_far_plane(Magnum::Float far_plane);
                    PhongMultiLight& set_is_shadowed(bool shadows);
                    PhongMultiLight& set_transparent_shadows(bool shadows);
                    PhongMultiLight& set_specular_strength(Magnum::Float specular_strength);

                    PhongMultiLight& bind_shadow_texture(Magnum::GL::Texture2DArray& texture);
                    PhongMultiLight& bind_shadow_color_texture(Magnum::GL::Texture2DArray& texture);
                    PhongMultiLight& bind_cube_map_texture(Magnum::GL::CubeMapTextureArray& texture);
                    PhongMultiLight& bind_cube_map_color_texture(Magnum::GL::CubeMapTextureArray& texture);

                    Magnum::Int max_lights() const;

                private:
                    Flags _flags;
                    Magnum::Int _max_lights = 10;
                    Magnum::Int _transformation_matrix_uniform{0}, _camera_matrix_uniform{7}, _projection_matrix_uniform{1}, _normal_matrix_uniform{2},
                        _shininess_uniform{3}, _ambient_color_uniform{4}, _diffuse_color_uniform{5}, _specular_color_uniform{6}, _specular_strength_uniform{11},
                        _lights_uniform{12}, _lights_matrices_uniform, _far_plane_uniform{8}, _is_shadowed_uniform{9}, _transparent_shadows_uniform{10},
                        _shadow_textures_location{3}, _cube_map_textures_location{4}, _shadow_color_textures_location{5}, _cube_map_color_textures_location{6};
                    const Magnum::Int _light_loc_size = 13;
                };

                CORRADE_ENUMSET_OPERATORS(PhongMultiLight::Flags)
            } // namespace gs
        } // namespace magnum
    } // namespace gui
} // namespace robot_dart

#endif