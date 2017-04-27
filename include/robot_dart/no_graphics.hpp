#ifndef ROBOT_DART_NO_GRAPHICS_HPP
#define ROBOT_DART_NO_GRAPHICS_HPP

#include <robot_dart/macros.hpp>

namespace robot_dart {
    class No_Graphics {
    public:
        No_Graphics() {}

        template <typename World>
        No_Graphics(World world) {}

        bool done() const
        {
            return false;
        }

        template <typename Simu>
        void refresh(Simu& simu) {}

        void set_render_period(double dt) {}
    };
}

#endif
