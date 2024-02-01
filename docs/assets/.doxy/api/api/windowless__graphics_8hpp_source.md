

# File windowless\_graphics.hpp

[**File List**](files.md) **>** [**gui**](dir_6a9d4b7ec29c938d1d9a486c655cfc8a.md) **>** [**magnum**](dir_5d18adecbc10cabf3ca51da31f2acdd1.md) **>** [**windowless\_graphics.hpp**](windowless__graphics_8hpp.md)

[Go to the documentation of this file](windowless__graphics_8hpp.md)

```C++

#ifndef ROBOT_DART_GUI_MAGNUM_WINDOWLESS_GRAPHICS_HPP
#define ROBOT_DART_GUI_MAGNUM_WINDOWLESS_GRAPHICS_HPP

#include <robot_dart/gui/magnum/base_graphics.hpp>
#include <robot_dart/gui/magnum/windowless_gl_application.hpp>

namespace robot_dart {
    namespace gui {
        namespace magnum {
            class WindowlessGraphics : public BaseGraphics<WindowlessGLApplication> {
            public:
                WindowlessGraphics(const GraphicsConfiguration& configuration = default_configuration()) : BaseGraphics<WindowlessGLApplication>(configuration) {}
                ~WindowlessGraphics() {}

                void set_simu(RobotDARTSimu* simu) override;

                static GraphicsConfiguration default_configuration();
            };
        } // namespace magnum
    } // namespace gui
} // namespace robot_dart

#endif

```

