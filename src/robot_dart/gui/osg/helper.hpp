#ifndef ROBOT_DART_GUI_OSG_HELPER_HPP
#define ROBOT_DART_GUI_OSG_HELPER_HPP

#include <vector>

#include <dart/gui/osg/osg.hpp>

namespace robot_dart {
    namespace gui {
        namespace osg {
            std::vector<std::vector<std::vector<uint8_t>>> rgb_from_image(const ::osg::ref_ptr<::osg::Image>& image)
            {
                std::vector<std::vector<std::vector<uint8_t>>> data;

                // TO-DO: Make this more performant
                size_t width = image->s();
                size_t height = image->t();
                size_t p = 3;
                data.resize(width);
                for (size_t w = 0; w < width; w++) {
                    data[w].resize(height);
                    for (size_t h = 0; h < height; h++) {
                        data[w][h].resize(p);
                        for (size_t i = 0; i < p; i++) {
                            float val = image->getColor(w, height - 1 - h)[i];
                            data[w][h][i] = static_cast<uint8_t>(val * 255.f);
                        }
                    }
                }

                return data;
            }
        } // namespace osg
    } // namespace gui
} // namespace robot_dart

#endif