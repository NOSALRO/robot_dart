#ifndef ROBOT_DART_GUI_BASE_HPP
#define ROBOT_DART_GUI_BASE_HPP

#include <robot_dart/gui/helper.hpp>

#include <dart/simulation/World.hpp>

namespace robot_dart {
    namespace gui {
        class Base {
        public:
            Base() {}

            Base(const dart::simulation::WorldPtr&) {}

            virtual ~Base() {}

            virtual bool done() const
            {
                return false;
            }

            virtual void refresh() {}

            virtual void set_render_period(double) {}

            virtual void set_enable(bool) {}

            virtual Image image() { return Image(); }
        };
    } // namespace gui
} // namespace robot_dart

#endif
