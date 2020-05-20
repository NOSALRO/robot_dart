#include "helper.hpp"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

namespace robot_dart {
    namespace gui {
        void save_png_image(const std::string& filename, const Image& rgb)
        {
            auto ends_with = [](const std::string& value, const std::string& ending) {
                if (ending.size() > value.size())
                    return false;
                return std::equal(ending.rbegin(), ending.rend(), value.rbegin());
            };

            std::string png = ".png";
            if (ends_with(filename, png))
                png = "";

            stbi_write_png((filename + png).c_str(), rgb.width, rgb.height, rgb.channels, rgb.data.data(), rgb.width * rgb.channels);
        }

        void save_png_image(const std::string& filename, const GrayscaleImage& gray)
        {
            auto ends_with = [](const std::string& value, const std::string& ending) {
                if (ending.size() > value.size())
                    return false;
                return std::equal(ending.rbegin(), ending.rend(), value.rbegin());
            };

            std::string png = ".png";
            if (ends_with(filename, png))
                png = "";

            stbi_write_png((filename + png).c_str(), gray.width, gray.height, 1, gray.data.data(), gray.width);
        }

        GrayscaleImage convert_rgb_to_grayscale(const Image& rgb)
        {
            assert(rgb.channels == 3);
            size_t width = rgb.width;
            size_t height = rgb.height;

            GrayscaleImage gray;
            gray.width = width;
            gray.height = height;
            gray.data.resize(width * height);

            for (size_t h = 0; h < height; h++) {
                for (size_t w = 0; w < width; w++) {
                    int id = w + h * width;
                    int id_rgb = w * rgb.channels + h * (width * rgb.channels);
                    uint8_t color = 0.3 * rgb.data[id_rgb + 0] + 0.59 * rgb.data[id_rgb + 1] + 0.11 * rgb.data[id_rgb + 2];
                    gray.data[id] = color;
                }
            }

            return gray;
        }
    } // namespace gui
} // namespace robot_dart