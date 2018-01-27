#ifndef ROBOT_DART_BASE_GRAPHICS_HPP
#define ROBOT_DART_BASE_GRAPHICS_HPP

#include <dart/simulation/World.hpp>

namespace robot_dart {
    class BaseGraphics {
    public:
        BaseGraphics() {}

        BaseGraphics(dart::simulation::WorldPtr world) {}

        virtual bool done() const
        {
            return false;
        }

        virtual void refresh() {}

        virtual void set_render_period(double dt) {}

        virtual void set_enable(bool enable) {}
    };
}

#endif
