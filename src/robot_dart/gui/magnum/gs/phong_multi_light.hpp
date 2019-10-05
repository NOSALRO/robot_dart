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

                    explicit PhongMultiLight(Flags flags = {}, Magnum::Int maxLights = 10);
                    explicit PhongMultiLight(Magnum::NoCreateT) noexcept;

                    Flags flags() const;

                    PhongMultiLight& setMaterial(Material& material);
                    PhongMultiLight& setLight(Magnum::Int i, const Light& light);

                    PhongMultiLight& setTransformationMatrix(const Magnum::Matrix4& matrix);
                    PhongMultiLight& setCameraMatrix(const Magnum::Matrix4& matrix);
                    PhongMultiLight& setNormalMatrix(const Magnum::Matrix3x3& matrix);
                    PhongMultiLight& setProjectionMatrix(const Magnum::Matrix4& matrix);

                    PhongMultiLight& bindShadowTexture(Magnum::GL::Texture2DArray& texture);

                    Magnum::Int maxLights() const;

                private:
                    Flags _flags;
                    Magnum::Int _maxLights = 10;
                    Magnum::Int _transformationMatrixUniform{0}, _cameraMatrixUniform{7}, _projectionMatrixUniform{1}, _normalMatrixUniform{2},
                        _shininessUniform{3}, _ambientColorUniform{4}, _diffuseColorUniform{5}, _specularColorUniform{6},
                        _lightsUniform{8}, _lightsMatricesUniform, _shadowTexturesLocation{3};
                };

                CORRADE_ENUMSET_OPERATORS(PhongMultiLight::Flags)
            } // namespace gs
        } // namespace magnum
    } // namespace gui
} // namespace robot_dart

#endif