#ifndef ROBOT_DART_GUI_MAGNUM_GS_SHADOW_MAP_HPP
#define ROBOT_DART_GUI_MAGNUM_GS_SHADOW_MAP_HPP

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
                class ShadowMap : public Magnum::GL::AbstractShaderProgram {
                public:
                    using Position = Magnum::Shaders::Generic3D::Position;

                    explicit ShadowMap();
                    explicit ShadowMap(Magnum::NoCreateT) noexcept;

                    ShadowMap& setTransformationMatrix(const Magnum::Matrix4& matrix);
                    ShadowMap& setProjectionMatrix(const Magnum::Matrix4& matrix);

                private:
                    Magnum::Int _transformationMatrixUniform{0}, _projectionMatrixUniform{1};
                };
            } // namespace gs
        } // namespace magnum
    } // namespace gui
} // namespace robot_dart

#endif