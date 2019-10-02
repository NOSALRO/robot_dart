#ifndef ROBOT_DART_GUI_MAGNUM_GS_HELPER_HPP
#define ROBOT_DART_GUI_MAGNUM_GS_HELPER_HPP

#include <string>
#include <vector>

#include <Magnum/Image.h>

namespace robot_dart {
    namespace gui {
        namespace magnum {
            namespace gs {
                std::vector<std::vector<std::vector<uint8_t>>> rgb_from_image(Magnum::Image2D* image);
                void save_image(const std::string& filename, const std::vector<std::vector<std::vector<uint8_t>>>& rgb);
            } // namespace gs
        } // namespace magnum
    } // namespace gui
} // namespace robot_dart

#endif