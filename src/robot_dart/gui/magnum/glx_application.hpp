#ifndef ROBOT_DART_GUI_MAGNUM_GLX_APPLICATION_HPP
#define ROBOT_DART_GUI_MAGNUM_GLX_APPLICATION_HPP

#include <robot_dart/gui/magnum/base_application.hpp>

#include <Corrade/Containers/Optional.h>
#include <Magnum/GL/Framebuffer.h>
#include <Magnum/Image.h>

namespace robot_dart {
    namespace gui {
        namespace magnum {
            class GLXApplication : public BaseApplication, public Magnum::Platform::WindowlessApplication {
            public:
                explicit GLXApplication(int argc, char** argv, const dart::simulation::WorldPtr& world, size_t width, size_t height, const std::string& title = "DART");
                ~GLXApplication();

                void render() override;

                Corrade::Containers::Optional<Magnum::Image2D>& image() { return _image; }

            protected:
                Magnum::GL::Framebuffer _framebuffer{Magnum::NoCreate};
                Corrade::Containers::Optional<Magnum::Image2D> _image;
                // size_t _index = 0;

                virtual int exec() override { return 0; }

                Corrade::Containers::Optional<Magnum::PixelFormat> _getPixelFormat(Magnum::GL::AbstractFramebuffer& framebuffer);
            };
        } // namespace magnum
    } // namespace gui
} // namespace robot_dart

#endif