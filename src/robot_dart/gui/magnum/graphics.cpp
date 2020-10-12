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
        } // namespace magnum
    } // namespace gui
} // namespace robot_dart