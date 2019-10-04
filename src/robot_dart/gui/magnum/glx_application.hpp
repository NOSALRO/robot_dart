#ifndef ROBOT_DART_GUI_MAGNUM_GLX_APPLICATION_HPP
#define ROBOT_DART_GUI_MAGNUM_GLX_APPLICATION_HPP

#include <robot_dart/gui/magnum/base_application.hpp>

#include <Magnum/PixelFormat.h>

namespace robot_dart {
    namespace gui {
        namespace magnum {
            class GLXApplication : public BaseApplication, public Magnum::Platform::WindowlessApplication {
            public:
                explicit GLXApplication(int argc, char** argv, const dart::simulation::WorldPtr& world, size_t width, size_t height, const std::string& title = "DART");
                ~GLXApplication();

                void render() override;

            protected:
                Magnum::GL::Framebuffer _framebuffer{Magnum::NoCreate};
                Magnum::PixelFormat _format;
                // size_t _index = 0;

                virtual int exec() override { return 0; }
            };
        } // namespace magnum
    } // namespace gui
} // namespace robot_dart

#endif