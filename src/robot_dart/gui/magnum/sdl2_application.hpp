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
                    : BaseApplication(), Magnum::Platform::Application({argc, argv}, Magnum::NoCreate), _speedMove(0.f), _speedStrafe(0.f)
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

                    /* Initialize DART world */
                    this->init(world);

                    /* Change default clear color to black */
                    Magnum::GL::Renderer::setClearColor(Magnum::Vector4{0.f, 0.f, 0.f, 1.f});

                    /* Loop at 60 Hz max */
                    setSwapInterval(1);
                    setMinimalLoopPeriod(16);

                    redraw();
                }

                ~Sdl2Application()
                {
                    GLCleanUp();
                }

                void render() override
                {
                    mainLoopIteration();
                }

            private:
                Magnum::Float _speedMove, _speedStrafe;

                static constexpr Magnum::Float _speed = 0.05f;

                void viewportEvent(const Magnum::Vector2i& size) override
                {
                    Magnum::GL::defaultFramebuffer.setViewport({{}, size});

                    _camera->camera().setViewport(size);
                }

                void drawEvent() override
                {
                    Magnum::GL::defaultFramebuffer.clear(
                        Magnum::GL::FramebufferClear::Color | Magnum::GL::FramebufferClear::Depth);

                    _camera->forward(_speedMove);
                    _camera->strafe(_speedStrafe);

                    /* Update graphic meshes/materials and render */
                    updateGraphics();
                    /* Update lights transformations */
                    updateLights();
                    _camera->camera().draw(_drawables);

                    swapBuffers();

                    redraw();
                }

                void keyReleaseEvent(KeyEvent& event) override
                {
                    _speedMove = 0.f;
                    _speedStrafe = 0.f;

                    event.setAccepted();
                }

                void keyPressEvent(KeyEvent& event) override
                {
                    if (event.key() == KeyEvent::Key::W) {
                        _speedMove = _speed;
                    }
                    else if (event.key() == KeyEvent::Key::S) {
                        _speedMove = -_speed;
                    }
                    else if (event.key() == KeyEvent::Key::A) {
                        _speedStrafe = -_speed;
                    }
                    else if (event.key() == KeyEvent::Key::D) {
                        _speedStrafe = _speed;
                    }
                    else if (event.key() == KeyEvent::Key::Q || event.key() == KeyEvent::Key::Esc) {
                        exit();
                    }

                    event.setAccepted();
                }

                void mouseScrollEvent(MouseScrollEvent& event)
                {
                    if (!event.offset().y())
                        return;

                    Magnum::Float perc = 0.f;
                    if (event.offset().y() > 0)
                        perc = 0.1f;
                    else
                        perc = -0.1f;

                    _camera->zoom(perc);

                    event.setAccepted();
                }

                void mouseMoveEvent(MouseMoveEvent& event)
                {
                    if (event.buttons() == MouseMoveEvent::Button::Left) {
                        _camera->move(event.relativePosition());
                    }

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