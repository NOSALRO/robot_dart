#ifndef ROBOT_DART_GUI_MAGNUM_GS_HELPER_HPP
#define ROBOT_DART_GUI_MAGNUM_GS_HELPER_HPP

#include <robot_dart/gui/helper.hpp>

#include <vector>

#include <Magnum/Image.h>

namespace robot_dart {
    namespace gui {
        namespace magnum {
            namespace gs {
                Image rgb_from_image(Magnum::Image2D* image);
                GrayscaleImage depth_from_image(Magnum::Image2D* image, bool linearize = false, Magnum::Float near_plane = 0.f, Magnum::Float far_plane = 100.f);
            } // namespace gs
        } // namespace magnum
    } // namespace gui
} // namespace robot_dart

#endif