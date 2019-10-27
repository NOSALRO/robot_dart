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

            // rgb is a vector of w*h*3
            assert(rgb.size() > 0 && rgb[0].size() > 0 && rgb[0][0].size() == 3);
            size_t width = rgb.size();
            size_t height = rgb[0].size();
            size_t p = 3; //rgb[0][0].size();

            // TO-DO: Make this more performant
            size_t len = width * height * p;
            std::vector<uint8_t> data;
            data.resize(len);
            for (size_t w = 0; w < width; w++) {
                for (size_t h = 0; h < height; h++) {
                    for (size_t i = 0; i < p; i++) {
                        int id = w + h * width;
                        data[id * p + i] = rgb[w][h][i];
                    }
                }
            }

            std::string png = ".png";
            if (ends_with(filename, png))
                png = "";

            stbi_write_png((filename + png).c_str(), width, height, p, data.data(), width * p);
        }

        void save_png_image(const std::string& filename, const GrayscaleImage& gray)
        {
            auto ends_with = [](const std::string& value, const std::string& ending) {
                if (ending.size() > value.size())
                    return false;
                return std::equal(ending.rbegin(), ending.rend(), value.rbegin());
            };

            // gray is a vector of w*h
            assert(gray.size() > 0 && gray[0].size() > 0);
            size_t width = gray.size();
            size_t height = gray[0].size();
            size_t p = 1;

            // TO-DO: Make this more performant
            size_t len = width * height;
            std::vector<uint8_t> data;
            data.resize(len);
            for (size_t w = 0; w < width; w++) {
                for (size_t h = 0; h < height; h++) {
                    int id = w + h * width;
                    data[id] = gray[w][h];
                }
            }

            std::string png = ".png";
            if (ends_with(filename, png))
                png = "";

            stbi_write_png((filename + png).c_str(), width, height, p, data.data(), width * p);
        }

        GrayscaleImage convert_rgb_to_grayscale(const Image& rgb)
        {
            // rgb is a vector of w*h*3
            assert(rgb.size() > 0 && rgb[0].size() > 0 && rgb[0][0].size() == 3);
            size_t width = rgb.size();
            size_t height = rgb[0].size();

            GrayscaleImage gray;
            gray.resize(width);
            for (size_t w = 0; w < width; w++) {
                gray[w].resize(height);
                for (size_t h = 0; h < height; h++) {
                    uint8_t color = 0.3 * rgb[w][h][0] + 0.59 * rgb[w][h][1] + 0.11 * rgb[w][h][2];
                    gray[w][h] = color;
                }
            }

            return gray;
        }
    } // namespace gui
} // namespace robot_dart