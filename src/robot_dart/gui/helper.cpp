#include "helper.hpp"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

namespace robot_dart {
    namespace gui {
        void save_png_image(const std::string& filename, const std::vector<std::vector<std::vector<uint8_t>>>& rgb)
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
    } // namespace gui
} // namespace robot_dart