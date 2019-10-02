#ifndef ROBOT_DART_GUI_HELPER_HPP
#define ROBOT_DART_GUI_HELPER_HPP

#include <string>
#include <vector>

namespace robot_dart {
    namespace gui {
        void save_png_image(const std::string& filename, const std::vector<std::vector<std::vector<uint8_t>>>& rgb);
    } // namespace gui
} // namespace robot_dart

#endif