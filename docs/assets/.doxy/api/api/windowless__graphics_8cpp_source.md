

# File windowless\_graphics.cpp

[**File List**](files.md) **>** [**gui**](dir_6a9d4b7ec29c938d1d9a486c655cfc8a.md) **>** [**magnum**](dir_5d18adecbc10cabf3ca51da31f2acdd1.md) **>** [**windowless\_graphics.cpp**](windowless__graphics_8cpp.md)

[Go to the documentation of this file](windowless__graphics_8cpp.md)

```C++

#include <robot_dart/gui/magnum/windowless_graphics.hpp>

namespace robot_dart {
    namespace gui {
        namespace magnum {
            void WindowlessGraphics::set_simu(RobotDARTSimu* simu)
            {
                BaseGraphics<WindowlessGLApplication>::set_simu(simu);
                // we should not synchronize by default if we want windowless graphics (usually used only for sensors)
                simu->scheduler().set_sync(false);
                // disable summary text when windowless graphics activated
                simu->enable_text_panel(false);
                simu->enable_status_bar(false);
            }

            GraphicsConfiguration WindowlessGraphics::default_configuration()
            {
                GraphicsConfiguration config;
                // by default we do not draw text in windowless mode
                config.draw_debug = false;
                config.draw_text = false;

                return config;
            }
        } // namespace magnum
    } // namespace gui
} // namespace robot_dart

```

