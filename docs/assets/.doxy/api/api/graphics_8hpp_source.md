

# File graphics.hpp

[**File List**](files.md) **>** [**gui**](dir_6a9d4b7ec29c938d1d9a486c655cfc8a.md) **>** [**magnum**](dir_5d18adecbc10cabf3ca51da31f2acdd1.md) **>** [**graphics.hpp**](graphics_8hpp.md)

[Go to the documentation of this file](graphics_8hpp.md)


```C++
#ifndef ROBOT_DART_GUI_MAGNUM_GRAPHICS_HPP
#define ROBOT_DART_GUI_MAGNUM_GRAPHICS_HPP

#include <robot_dart/gui/magnum/base_graphics.hpp>

namespace robot_dart {
    namespace gui {
        namespace magnum {
            class Graphics : public BaseGraphics<GlfwApplication> {
            public:
                Graphics(const GraphicsConfiguration& configuration = default_configuration()) : BaseGraphics<GlfwApplication>(configuration) {}
                ~Graphics() {}

                void set_simu(RobotDARTSimu* simu) override;

                static GraphicsConfiguration default_configuration();
            };
        } // namespace magnum
    } // namespace gui
} // namespace robot_dart

#endif
```


