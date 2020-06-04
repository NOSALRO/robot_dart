#ifndef ROBOT_DART_GUI_BASE_HPP
#define ROBOT_DART_GUI_BASE_HPP

#include <robot_dart/gui/helper.hpp>

namespace robot_dart {
    class RobotDARTSimu;

    namespace gui {
        class Base {
        public:
            Base() {}

            Base(RobotDARTSimu*) {}

            virtual ~Base() {}

            virtual bool done() const { return false; }

            virtual void refresh() {}

            virtual void set_render_period(double) {}

            virtual void set_enable(bool) {}
            virtual void set_fps(int) {}

            virtual Image image() { return Image(); }
            virtual GrayscaleImage depth_image() { return GrayscaleImage(); }
            virtual GrayscaleImage raw_depth_image() { return GrayscaleImage(); }
        };
    } // namespace gui
} // namespace robot_dart

#endif
