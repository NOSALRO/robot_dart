

# File graphics.cpp

[**File List**](files.md) **>** [**gui**](dir_6a9d4b7ec29c938d1d9a486c655cfc8a.md) **>** [**magnum**](dir_5d18adecbc10cabf3ca51da31f2acdd1.md) **>** [**graphics.cpp**](graphics_8cpp.md)

[Go to the documentation of this file](graphics_8cpp.md)

```C++

#include <robot_dart/gui/magnum/graphics.hpp>

namespace robot_dart {
    namespace gui {
        namespace magnum {
            void Graphics::set_simu(RobotDARTSimu* simu)
            {
                BaseGraphics<GlfwApplication>::set_simu(simu);
                // we synchronize by default if we have the graphics activated
                simu->scheduler().set_sync(true);
                // enable summary text when graphics activated
                simu->enable_text_panel(true);
                simu->enable_status_bar(true);
            }

            GraphicsConfiguration Graphics::default_configuration()
            {
                return GraphicsConfiguration();
            }
        } // namespace magnum
    } // namespace gui
} // namespace robot_dart

```

