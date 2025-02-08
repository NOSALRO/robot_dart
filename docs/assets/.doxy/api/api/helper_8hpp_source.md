

# File helper.hpp

[**File List**](files.md) **>** [**gui**](dir_6a9d4b7ec29c938d1d9a486c655cfc8a.md) **>** [**helper.hpp**](helper_8hpp.md)

[Go to the documentation of this file](helper_8hpp.md)


```C++
#ifndef ROBOT_DART_GUI_HELPER_HPP
#define ROBOT_DART_GUI_HELPER_HPP

#include <string>
#include <vector>

#include <robot_dart/utils.hpp>

namespace robot_dart {
    namespace gui {
        struct Image {
            size_t width = 0, height = 0;
            size_t channels = 3;
            std::vector<uint8_t> data;
        };

        struct GrayscaleImage {
            size_t width = 0, height = 0;
            std::vector<uint8_t> data;
        };

        struct DepthImage {
            size_t width = 0, height = 0;
            std::vector<double> data;
        };

        void save_png_image(const std::string& filename, const Image& rgb);
        void save_png_image(const std::string& filename, const GrayscaleImage& gray);

        GrayscaleImage convert_rgb_to_grayscale(const Image& rgb);

        std::vector<Eigen::Vector3d> point_cloud_from_depth_array(const DepthImage& depth_image, const Eigen::Matrix3d& intrinsic_matrix, const Eigen::Matrix4d& tf, double far_plane = 1000.);
    } // namespace gui
} // namespace robot_dart

#endif
```


