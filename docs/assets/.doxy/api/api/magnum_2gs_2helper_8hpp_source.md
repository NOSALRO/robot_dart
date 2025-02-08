

# File helper.hpp

[**File List**](files.md) **>** [**gs**](dir_2f8612d80f6bb57c97efd4c82e0df286.md) **>** [**helper.hpp**](magnum_2gs_2helper_8hpp.md)

[Go to the documentation of this file](magnum_2gs_2helper_8hpp.md)


```C++
#ifndef ROBOT_DART_GUI_MAGNUM_GS_HELPER_HPP
#define ROBOT_DART_GUI_MAGNUM_GS_HELPER_HPP

#include <robot_dart/gui/helper.hpp>

#include <vector>

#include <Magnum/Image.h>

namespace robot_dart {
    namespace gui {
        namespace magnum {
            namespace gs {
                Image rgb_from_image(Magnum::Image2D* image);
                GrayscaleImage depth_from_image(Magnum::Image2D* image, bool linearize = false, Magnum::Float near_plane = 0.f, Magnum::Float far_plane = 100.f);
                DepthImage depth_array_from_image(Magnum::Image2D* image, Magnum::Float near_plane = 0.f, Magnum::Float far_plane = 100.f);
            } // namespace gs
        } // namespace magnum
    } // namespace gui
} // namespace robot_dart

#endif
```


