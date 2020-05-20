#ifndef ROBOT_DART_GUI_HELPER_HPP
#define ROBOT_DART_GUI_HELPER_HPP

#include <string>
#include <vector>

namespace robot_dart {
    namespace gui {
        struct Image {
            size_t width, height;
            size_t channels = 3;
            std::vector<uint8_t> data;
        };

        struct GrayscaleImage {
            size_t width, height;
            std::vector<uint8_t> data;
        };

        void save_png_image(const std::string& filename, const Image& rgb);
        void save_png_image(const std::string& filename, const GrayscaleImage& gray);

        GrayscaleImage convert_rgb_to_grayscale(const Image& rgb);
    } // namespace gui
} // namespace robot_dart

#endif