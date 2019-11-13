#include "glfw_application.hpp"

#include <Magnum/GL/DefaultFramebuffer.h>
#include <Magnum/GL/Renderer.h>
#include <Magnum/GL/Version.h>

#include <Magnum/PixelFormat.h>

namespace robot_dart {
    namespace gui {
        namespace magnum {
            GlfwApplication::GlfwApplication(int argc, char** argv, const dart::simulation::WorldPtr& world, size_t width, size_t height, const std::string& title, bool isShadowed)
                : BaseApplication(isShadowed), Magnum::Platform::Application({argc, argv}, Magnum::NoCreate), _speedMove(0.f), _speedStrafe(0.f)
            {
                /* Try 16x MSAA */
                Configuration conf;
                GLConfiguration glConf;
                conf.setTitle(title);
                conf.setSize({static_cast<int>(width), static_cast<int>(height)});
                conf.setWindowFlags(Configuration::WindowFlag::Resizable);
                glConf.setSampleCount(8);
                glConf.setVersion(Magnum::GL::Version::GL320);
                if (!tryCreate(conf, glConf))
                    create(conf, glConf.setSampleCount(0));

                /* Initialize DART world */
                init(world, Magnum::GL::defaultFramebuffer.viewport().size()[0], Magnum::GL::defaultFramebuffer.viewport().size()[1]);

                /* Loop at 60 Hz max */
                setSwapInterval(1);

                redraw();
            }

            GlfwApplication::~GlfwApplication()
            {
                GLCleanUp();
            }

            void GlfwApplication::render()
            {
                mainLoopIteration();
            }

            void GlfwApplication::viewportEvent(const Magnum::Vector2i& size)
            {
                Magnum::GL::defaultFramebuffer.setViewport({{}, size});

                _camera->setViewport(size);
            }

            void GlfwApplication::drawEvent()
            {
                /* Update graphic meshes/materials and render */
                updateGraphics();
                /* Update lights transformations */
                updateLights(*_camera);

                Magnum::GL::Renderer::enable(Magnum::GL::Renderer::Feature::DepthTest);
                Magnum::GL::Renderer::enable(Magnum::GL::Renderer::Feature::FaceCulling);

                /* Change default clear color to black */
                Magnum::GL::Renderer::setClearColor(Magnum::Vector4{0.f, 0.f, 0.f, 1.f});

                Magnum::GL::defaultFramebuffer.bind();
                Magnum::GL::defaultFramebuffer.clear(
                    Magnum::GL::FramebufferClear::Color | Magnum::GL::FramebufferClear::Depth);

                _camera->forward(_speedMove);
                _camera->strafe(_speedStrafe);

                /* Draw with main camera */
                _camera->draw(_drawables, Magnum::GL::defaultFramebuffer, Magnum::PixelFormat::RGB8Unorm);

                swapBuffers();

                redraw();
            }

            void GlfwApplication::keyReleaseEvent(KeyEvent& event)
            {
                _speedMove = 0.f;
                _speedStrafe = 0.f;

                event.setAccepted();
            }

            void GlfwApplication::keyPressEvent(KeyEvent& event)
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