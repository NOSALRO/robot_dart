#include "helper.hpp"

#include <Corrade/Containers/StridedArrayView.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include <iostream>

namespace robot_dart {
    namespace gui {
        namespace magnum {
            namespace gs {
                std::vector<std::vector<std::vector<uint8_t>>> rgb_from_image(Magnum::Image2D* image)
                {
                    auto pixels = image->pixels();
                    auto sz = pixels.size();
                    std::vector<std::vector<std::vector<uint8_t>>> data;

                    // TO-DO: Make this more performant
                    size_t width = sz[1];
                    size_t height = sz[0];
                    size_t p = 3;
                    data.resize(width);
                    for (size_t w = 0; w < width; w++) {
                        data[w].resize(height);
                        for (size_t h = 0; h < height; h++) {
                            data[w][h].resize(p);
                            for (size_t i = 0; i < p; i++) {
                                data[w][h][i] = pixels[height - 1 - h][w][i];
                            }
                        }
                    }

                    return data;
                }

                void save_image(const std::string& filename, const std::vector<std::vector<std::vector<uint8_t>>>& rgb)
                {
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

                    stbi_write_png(filename.c_str(), width, height, p, data.data(), width * p);
                }
            } // namespace gs
        } // namespace magnum
    } // namespace gui
} // namespace robot_dart