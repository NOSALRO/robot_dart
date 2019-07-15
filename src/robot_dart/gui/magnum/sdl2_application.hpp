#ifndef ROBOT_DART_GUI_MAGNUM_SDL2_APPLICATION_HPP
#define ROBOT_DART_GUI_MAGNUM_SDL2_APPLICATION_HPP

#include <robot_dart/gui/magnum/base_application.hpp>

#include <Magnum/Platform/Sdl2Application.h>

namespace robot_dart {
    namespace gui {
        namespace magnum {
            class Sdl2Application : public BaseApplication, public Magnum::Platform::Application {
            public:
                explicit Sdl2Application(int argc, char** argv, const dart::simulation::WorldPtr& world, size_t width, size_t height, const std::string& title = "DART")
                    : BaseApplication(), Magnum::Platform::Application({argc, argv}, Magnum::NoCreate)
                {
                    /* Try 16x MSAA */
                    Configuration conf;
                    GLConfiguration glConf;
                    conf.setTitle(title);
                    conf.setSize({static_cast<int>(width), static_cast<int>(height)});
                    conf.setWindowFlags(Configuration::WindowFlag::Resizable);
                    glConf.setSampleCount(8);
                    if (!tryCreate(conf, glConf))
                        create(conf, glConf.setSampleCount(0));

                    Magnum::GL::Renderer::enable(Magnum::GL::Renderer::Feature::DepthTest);
                    Magnum::GL::Renderer::enable(Magnum::GL::Renderer::Feature::FaceCulling);

                    this->init(world);

                    /* Change default clear color to black */
                    Magnum::GL::Renderer::setClearColor(Magnum::Vector4{0.f, 0.f, 0.f, 1.f});

                    /* Loop at 60 Hz max */
                    setSwapInterval(1);
                    setMinimalLoopPeriod(16);

                    redraw();
                }

                void render() override
                {
                    mainLoopIteration();
                }

            private:
                void viewportEvent(const Magnum::Vector2i& size) override
                {
                    Magnum::GL::defaultFramebuffer.setViewport({{}, size});

                    _camera->setViewport(size);
                }

                void drawEvent() override
                {
                    Magnum::GL::defaultFramebuffer.clear(
                        Magnum::GL::FramebufferClear::Color | Magnum::GL::FramebufferClear::Depth);

                    /* Update graphic meshes/materials and render */
                    updateGraphics();
                    /* Update lights transformations */
                    updateLights();
                    _camera->draw(_drawables);

                    swapBuffers();
                    redraw();
                }

                void keyPressEvent(KeyEvent& event) override
                {
                    if (event.key() == KeyEvent::Key::Down)
                        _cameraObject->rotateX(Magnum::Deg(5.0f));
                    else if (event.key() == KeyEvent::Key::Up)
                        _cameraObject->rotateX(Magnum::Deg(-5.0f));
                    else if (event.key() == KeyEvent::Key::Left)
                        _cameraRig->rotateY(Magnum::Deg(-5.0f));
                    else if (event.key() == KeyEvent::Key::Right)
                        _cameraRig->rotateY(Magnum::Deg(5.0f));
                    else
                        return;

                    event.setAccepted();
                }

                void exitEvent(ExitEvent& event) override
                {
                    _done = true;
                    event.setAccepted();
                }
            };
        } // namespace magnum
    } // namespace gui
} // namespace robot_dart

#endif