#ifndef ROBOT_DART_GUI_MAGNUM_GS_CUBE_MAP_HPP
#define ROBOT_DART_GUI_MAGNUM_GS_CUBE_MAP_HPP

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
                class CubeMap : public Magnum::GL::AbstractShaderProgram {
                public:
                    using Position = Magnum::Shaders::Generic3D::Position;

                    explicit CubeMap();
                    explicit CubeMap(Magnum::NoCreateT) noexcept;

                    CubeMap& setTransformationMatrix(const Magnum::Matrix4& matrix);
                    CubeMap& setShadowMatrices(Magnum::Matrix4 matrices[6]);
                    CubeMap& setLightPosition(const Magnum::Vector3& position);
                    CubeMap& setFarPlane(Magnum::Float farPlane);
                    CubeMap& setLightIndex(Magnum::Int index);

                private:
                    Magnum::Int _transformationMatrixUniform{0},
                        _shadowMatricesUniform{4},
                        _lightPositionUniform{1},
                        _farPlaneUniform{2},
                        _indexUniform{3};
                };
            } // namespace gs
        } // namespace magnum
    } // namespace gui
} // namespace robot_dart

#endif