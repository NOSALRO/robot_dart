#include "helper.hpp"

#include <Corrade/Containers/StridedArrayView.h>

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
            } // namespace gs
        } // namespace magnum
    } // namespace gui
} // namespace robot_dart