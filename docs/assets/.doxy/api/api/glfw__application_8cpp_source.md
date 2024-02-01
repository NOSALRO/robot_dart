

# File glfw\_application.cpp

[**File List**](files.md) **>** [**gui**](dir_6a9d4b7ec29c938d1d9a486c655cfc8a.md) **>** [**magnum**](dir_5d18adecbc10cabf3ca51da31f2acdd1.md) **>** [**glfw\_application.cpp**](glfw__application_8cpp.md)

[Go to the documentation of this file](glfw__application_8cpp.md)

```C++

#include "glfw_application.hpp"
#include "robot_dart/utils.hpp"

#include <Magnum/GL/DefaultFramebuffer.h>
#include <Magnum/GL/Renderer.h>
#include <Magnum/GL/Version.h>

#include <Magnum/PixelFormat.h>

namespace robot_dart {
    namespace gui {
        namespace magnum {
            GlfwApplication::GlfwApplication(int argc, char** argv, RobotDARTSimu* simu, const GraphicsConfiguration& configuration)
                : BaseApplication(configuration),
                  Magnum::Platform::Application({argc, argv}, Magnum::NoCreate),
                  _simu(simu),
                  _speed_move(0.f),
                  _speed_strafe(0.f),
                  _draw_main_camera(configuration.draw_main_camera),
                  _draw_debug(configuration.draw_debug),
                  _bg_color(configuration.bg_color[0],
                      configuration.bg_color[1],
                      configuration.bg_color[2],
                      configuration.bg_color[3])
            {
                /* Try 16x MSAA */
                Configuration conf;
                GLConfiguration glConf;
                conf.setTitle(configuration.title);
                conf.setSize({static_cast<int>(configuration.width), static_cast<int>(configuration.height)});
                conf.setWindowFlags(Configuration::WindowFlag::Resizable);
                glConf.setSampleCount(8);
                if (!tryCreate(conf, glConf))
                    create(conf, glConf.setSampleCount(0));
                ROBOT_DART_EXCEPTION_ASSERT(Magnum::GL::Context::current().version() >= Magnum::GL::Version::GL320, "robot_dart requires at least OpenGL 3.2 for rendering!");

                /* Initialize DART world */
                GraphicsConfiguration config = configuration;
                config.width = Magnum::GL::defaultFramebuffer.viewport().size()[0];
                config.height = Magnum::GL::defaultFramebuffer.viewport().size()[1];
                init(simu, config);

                /* No VSync */
                setSwapInterval(0);

                redraw();
            }

            GlfwApplication::~GlfwApplication()
            {
                _gl_clean_up();
            }

            void GlfwApplication::render()
            {
                mainLoopIteration();
            }

            void GlfwApplication::viewportEvent(ViewportEvent& event)
            {
                Magnum::GL::defaultFramebuffer.setViewport({{}, event.framebufferSize()});

                _camera->set_viewport(event.framebufferSize());
            }

            void GlfwApplication::drawEvent()
            {
                if (_draw_main_camera) {
                    /* Update graphic meshes/materials and render */
                    update_graphics();
                    /* Update lights transformations --- this also draws the shadows if enabled */
                    update_lights(*_camera);

                    Magnum::GL::Renderer::enable(Magnum::GL::Renderer::Feature::DepthTest);
                    Magnum::GL::Renderer::enable(Magnum::GL::Renderer::Feature::FaceCulling);
                    Magnum::GL::Renderer::enable(Magnum::GL::Renderer::Feature::Blending);
                    Magnum::GL::Renderer::setBlendFunction(Magnum::GL::Renderer::BlendFunction::SourceAlpha, Magnum::GL::Renderer::BlendFunction::OneMinusSourceAlpha);
                    Magnum::GL::Renderer::setBlendEquation(Magnum::GL::Renderer::BlendEquation::Add);

                    /* Change clear color to _bg_color */
                    Magnum::GL::Renderer::setClearColor(_bg_color);

                    Magnum::GL::defaultFramebuffer.bind();
                    Magnum::GL::defaultFramebuffer.clear(
                        Magnum::GL::FramebufferClear::Color | Magnum::GL::FramebufferClear::Depth);

                    _camera->forward(_speed_move);
                    _camera->strafe(_speed_strafe);

                    /* Draw with main camera */
                    _camera->draw(_drawables, Magnum::GL::defaultFramebuffer, Magnum::PixelFormat::RGB8Unorm, _simu, debug_draw_data(), _draw_debug);

                    swapBuffers();
                }

                redraw();
            }

            void GlfwApplication::keyReleaseEvent(KeyEvent& event)
            {
                _speed_move = 0.f;
                _speed_strafe = 0.f;

                event.setAccepted();
            }

            void GlfwApplication::keyPressEvent(KeyEvent& event)
            {
                if (event.key() == KeyEvent::Key::W) {
                    _speed_move = _speed;
                }
                else if (event.key() == KeyEvent::Key::S) {
                    _speed_move = -_speed;
                }
                else if (event.key() == KeyEvent::Key::A) {
                    _speed_strafe = -_speed;
                }
                else if (event.key() == KeyEvent::Key::D) {
                    _speed_strafe = _speed;
                }
                else if (event.key() == KeyEvent::Key::Q || event.key() == KeyEvent::Key::Esc) {
                    exit();
                }

                event.setAccepted();
            }

            void GlfwApplication::mouseScrollEvent(MouseScrollEvent& event)
            {
                if (!event.offset().y())
                    return;

                Magnum::Float perc = 0.f;
                if (event.offset().y() > 0)
                    perc = 0.1f;
                else
                    perc = -0.1f;

                _camera->forward(perc);

                event.setAccepted();
            }

            void GlfwApplication::mouseMoveEvent(MouseMoveEvent& event)
            {
                if (event.buttons() == MouseMoveEvent::Button::Left) {
                    _camera->move(event.relativePosition());
                }

                event.setAccepted();
            }

            void GlfwApplication::exitEvent(ExitEvent& event)
            {
                _done = true;
                event.setAccepted();
            }
        } // namespace magnum
    } // namespace gui
} // namespace robot_dart

```

