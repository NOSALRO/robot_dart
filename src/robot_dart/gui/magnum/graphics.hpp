#ifndef ROBOT_DART_GUI_MAGNUM_GRAPHICS_HPP
#define ROBOT_DART_GUI_MAGNUM_GRAPHICS_HPP

#include <robot_dart/gui/magnum/base_graphics.hpp>

namespace robot_dart {
    namespace gui {
        namespace magnum {
            class Graphics : public BaseGraphics<GlfwApplication> {
            public:
                Graphics(const GraphicsConfiguration& configuration = GraphicsConfiguration()) : BaseGraphics<GlfwApplication>(configuration) {}
                ~Graphics() {}

                void set_simu(RobotDARTSimu* simu) override;
            };
        } // namespace magnum
    } // namespace gui
} // namespace robot_dart

#endif