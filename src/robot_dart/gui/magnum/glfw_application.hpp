#ifndef ROBOT_DART_GUI_MAGNUM_GLFW_APPLICATION_HPP
#define ROBOT_DART_GUI_MAGNUM_GLFW_APPLICATION_HPP

#include <robot_dart/gui/magnum/base_application.hpp>

// Workaround for X11lib defines
#undef Button1
#undef Button2
#undef Button3
#undef Button4
#undef Button5
#include <Magnum/Platform/GlfwApplication.h>

namespace robot_dart {
    namespace gui {
        namespace magnum {
            class GlfwApplication : public BaseApplication, public Magnum::Platform::Application {
            public:
                explicit GlfwApplication(int argc, char** argv, const dart::simulation::WorldPtr& world, const GraphicsConfiguration& configuration = GraphicsConfiguration());

                ~GlfwApplication();

                void render() override;

            protected:
                Magnum::Float _speed_move, _speed_strafe;

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