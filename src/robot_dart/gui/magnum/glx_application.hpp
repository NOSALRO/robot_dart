#ifndef ROBOT_DART_GUI_MAGNUM_GLX_APPLICATION_HPP
#define ROBOT_DART_GUI_MAGNUM_GLX_APPLICATION_HPP

#include <robot_dart/gui/magnum/base_application.hpp>

#include <Magnum/Platform/WindowlessGlxApplication.h>

namespace robot_dart {
    namespace gui {
        namespace magnum {
            class GLXApplication : public BaseApplication, public Magnum::Platform::WindowlessApplication {
            public:
                explicit GLXApplication(int argc, char** argv, const dart::simulation::WorldPtr& world, size_t width, size_t height, const std::string& title = "DART")
                    : BaseApplication(), Magnum::Platform::WindowlessApplication({argc, argv}, Magnum::NoCreate)
                {
                    // tryCreateContext(Configuration());
                    GlobalData::instance()->gl_context().makeCurrent();
                    if (!tryCreateContext(Configuration())) {
                        Corrade::Utility::Error{} << "Could not create context!";
                        return;
                    }
                    else
                        Corrade::Utility::Debug{} << "Created context with: " << Magnum::GL::Context::current().versionString();
                    Magnum::GL::Renderer::enable(Magnum::GL::Renderer::Feature::DepthTest);
                    Magnum::GL::Renderer::enable(Magnum::GL::Renderer::Feature::FaceCulling);

                    this->init(world);

                    /* Change default clear color to black */
                    Magnum::GL::Renderer::setClearColor(Magnum::Vector4{0.f, 0.f, 0.f, 1.f});

                    // /* Loop at 60 Hz max */
                    // setSwapInterval(1);
                    // setMinimalLoopPeriod(16);

                    // redraw();
                }

                void render() override
                {
                    Magnum::GL::defaultFramebuffer.clear(
                        Magnum::GL::FramebufferClear::Color | Magnum::GL::FramebufferClear::Depth);

                    /* Update graphic meshes/materials and render */
                    updateGraphics();
                    /* Update lights transformations */
                    updateLights();
                    _camera->draw(_drawables);

                    // swapBuffers();
                    // redraw();
                }

            private:
                int exec() override
                {
                    return 0;
                }
            };
        } // namespace magnum
    } // namespace gui
} // namespace robot_dart

#endif