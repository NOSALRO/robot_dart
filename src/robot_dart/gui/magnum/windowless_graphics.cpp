#include <robot_dart/gui/magnum/windowless_graphics.hpp>

namespace robot_dart {
    namespace gui {
        namespace magnum {
            WindowlessGraphics::WindowlessGraphics(RobotDARTSimu* simu, const GraphicsConfiguration& configuration) : BaseGraphics(simu, configuration)
            {
                // we should not synchronize by default if we want windowless graphics (usually used only for sensors)
                simu->scheduler().set_sync(false);
                // disable summary text when windowless graphics activated
                simu->enable_text_panel(false);
                simu->enable_status_bar(false);
            }
        } // namespace magnum
    } // namespace gui
} // namespace robot_dart