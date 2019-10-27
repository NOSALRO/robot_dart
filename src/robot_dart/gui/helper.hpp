#ifndef ROBOT_DART_GUI_HELPER_HPP
#define ROBOT_DART_GUI_HELPER_HPP

#include <string>
#include <vector>

namespace robot_dart {
    namespace gui {
        using Image = std::vector<std::vector<std::vector<uint8_t>>>;
        using GrayscaleImage = std::vector<std::vector<uint8_t>>;

        void save_png_image(const std::string& filename, const Image& rgb);
        void save_png_image(const std::string& filename, const GrayscaleImage& gray);

        GrayscaleImage convert_rgb_to_grayscale(const Image& rgb);
    } // namespace gui
} // namespace robot_dart

#endif