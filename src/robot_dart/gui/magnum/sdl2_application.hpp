#ifndef ROBOT_DART_GUI_MAGNUM_SDL2_APPLICATION_HPP
#define ROBOT_DART_GUI_MAGNUM_SDL2_APPLICATION_HPP

#include <robot_dart/gui/magnum/base_application.hpp>

#include <Magnum/Platform/Sdl2Application.h>

namespace robot_dart {
    namespace gui {
        namespace magnum {
            class Sdl2Application : public BaseApplication, public Magnum::Platform::Application {
            public:
                explicit Sdl2Application(int argc, char** argv, const dart::simulation::WorldPtr& world, size_t width, size_t height, const std::string& title = "DART", bool isShadowed = true);

                ~Sdl2Application();

                void render() override;

            protected:
                Magnum::Float _speedMove, _speedStrafe;

                static constexpr Magnum::Float _speed = 0.05f;

                void viewportEvent(const Magnum::Vector2i& size) override;

                void drawEvent() override;

                virtual void keyReleaseEvent(KeyEvent& event) override;
                virtual void keyPressEvent(KeyEvent& event) override;

                virtual void mouseScrollEvent(MouseScrollEvent& event) override;
                virtual void mouseMoveEvent(MouseMoveEvent& event) override;

                void exitEvent(ExitEvent& event) override;
            };
        } // namespace magnum
    } // namespace gui
} // namespace robot_dart

#endif