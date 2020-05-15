#ifndef ROBOT_DART_GUI_MAGNUM_GLX_APPLICATION_HPP
#define ROBOT_DART_GUI_MAGNUM_GLX_APPLICATION_HPP

#include <robot_dart/gui/magnum/base_application.hpp>

#include <Magnum/GL/Renderbuffer.h>
#include <Magnum/PixelFormat.h>

namespace robot_dart {
    namespace gui {
        namespace magnum {
            class WindowlessGLApplication : public BaseApplication, public Magnum::Platform::WindowlessApplication {
            public:
                explicit WindowlessGLApplication(int argc, char** argv, RobotDARTSimu* simu, const GraphicsConfiguration& configuration = GraphicsConfiguration());
                ~WindowlessGLApplication();

                void render() override;

            protected:
                bool _draw_main_camera, _draw_ghosts;

                Magnum::GL::Framebuffer _framebuffer{Magnum::NoCreate};
                Magnum::PixelFormat _format;
                Magnum::GL::Renderbuffer _color{Magnum::NoCreate}, _depth{Magnum::NoCreate};
                // size_t _index = 0;

                virtual int exec() override { return 0; }
            };
        } // namespace magnum
    } // namespace gui
} // namespace robot_dart

#endif