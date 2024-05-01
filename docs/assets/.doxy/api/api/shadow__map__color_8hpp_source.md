

# File shadow\_map\_color.hpp

[**File List**](files.md) **>** [**gs**](dir_2f8612d80f6bb57c97efd4c82e0df286.md) **>** [**shadow\_map\_color.hpp**](shadow__map__color_8hpp.md)

[Go to the documentation of this file](shadow__map__color_8hpp.md)


```C++
#ifndef ROBOT_DART_GUI_MAGNUM_GS_SHADOW_MAP_COLOR_HPP
#define ROBOT_DART_GUI_MAGNUM_GS_SHADOW_MAP_COLOR_HPP

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
                class ShadowMapColor : public Magnum::GL::AbstractShaderProgram {
                public:
                    using Position = Magnum::Shaders::Generic3D::Position;
                    using TextureCoordinates = Magnum::Shaders::Generic3D::TextureCoordinates;

                    enum class Flag : Magnum::UnsignedByte {
                        DiffuseTexture = 1 << 0, 
                    };

                    using Flags = Magnum::Containers::EnumSet<Flag>;

                    explicit ShadowMapColor(Flags flags = {});
                    explicit ShadowMapColor(Magnum::NoCreateT) noexcept;

                    Flags flags() const;

                    ShadowMapColor& set_transformation_matrix(const Magnum::Matrix4& matrix);
                    ShadowMapColor& set_projection_matrix(const Magnum::Matrix4& matrix);
                    ShadowMapColor& set_material(Material& material);

                private:
                    Flags _flags;
                    Magnum::Int _transformation_matrix_uniform{0}, _projection_matrix_uniform{1}, _diffuse_color_uniform{2};
                };

                CORRADE_ENUMSET_OPERATORS(ShadowMapColor::Flags)
            } // namespace gs
        } // namespace magnum
    } // namespace gui
} // namespace robot_dart

#endif
```


