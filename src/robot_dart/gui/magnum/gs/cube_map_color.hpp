#ifndef ROBOT_DART_GUI_MAGNUM_GS_CUBE_MAP_COLOR_HPP
#define ROBOT_DART_GUI_MAGNUM_GS_CUBE_MAP_COLOR_HPP

#include <robot_dart/gui/magnum/gs/material.hpp>

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
                class CubeMapColor : public Magnum::GL::AbstractShaderProgram {
                public:
                    using Position = Magnum::Shaders::Generic3D::Position;
                    using TextureCoordinates = Magnum::Shaders::Generic3D::TextureCoordinates;

                    enum class Flag : Magnum::UnsignedByte {
                        DiffuseTexture = 1 << 0, /**< The shader uses diffuse texture instead of color */
                    };

                    using Flags = Magnum::Containers::EnumSet<Flag>;

                    explicit CubeMapColor(Flags flags = {});
                    explicit CubeMapColor(Magnum::NoCreateT) noexcept;

                    Flags flags() const;

                    CubeMapColor& setTransformationMatrix(const Magnum::Matrix4& matrix);
                    CubeMapColor& setShadowMatrices(Magnum::Matrix4 matrices[6]);
                    CubeMapColor& setLightPosition(const Magnum::Vector3& position);
                    CubeMapColor& setFarPlane(Magnum::Float farPlane);
                    CubeMapColor& setLightIndex(Magnum::Int index);
                    CubeMapColor& setMaterial(Material& material);

                    CubeMapColor& bindCubeMapTexture(Magnum::GL::CubeMapTextureArray& texture);

                private:
                    Flags _flags;
                    Magnum::Int _transformationMatrixUniform{0},
                        _shadowMatricesUniform{5},
                        _lightPositionUniform{1},
                        _farPlaneUniform{2},
                        _lightIndexUniform{3},
                        _diffuseColorUniform{4},
                        _cubeMapTexturesLocation{2};
                };

                CORRADE_ENUMSET_OPERATORS(CubeMapColor::Flags)
            } // namespace gs
        } // namespace magnum
    } // namespace gui
} // namespace robot_dart

#endif