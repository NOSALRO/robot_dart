#ifndef ROBOT_DART_GRAPHICS_BASE_GRAPHICS_HPP
#define ROBOT_DART_GRAPHICS_BASE_GRAPHICS_HPP

#include <dart/simulation/World.hpp>

namespace robot_dart {
    namespace graphics {
        class BaseGraphics {
        public:
            BaseGraphics() {}

            BaseGraphics(const dart::simulation::WorldPtr&) {}

            virtual ~BaseGraphics() {}

            virtual bool done() const
            {
                return false;
            }

            virtual void refresh() {}

            virtual void set_render_period(double) {}

            virtual void set_enable(bool) {}
        };
    } // namespace graphics
} // namespace robot_dart

#endif
