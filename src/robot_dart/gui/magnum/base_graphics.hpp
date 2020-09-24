#ifndef ROBOT_DART_GUI_MAGNUM_BASE_GRAPHICS_HPP
#define ROBOT_DART_GUI_MAGNUM_BASE_GRAPHICS_HPP

#include <robot_dart/gui/base.hpp>
#include <robot_dart/gui/magnum/glfw_application.hpp>
#include <robot_dart/gui/magnum/gs/helper.hpp>
#include <robot_dart/robot_dart_simu.hpp>

// We need this for CORRADE_RESOURCE_INITIALIZE
#include <Corrade/Utility/Resource.h>

inline static void robot_dart_initialize_magnum_resources()
{
    CORRADE_RESOURCE_INITIALIZE(RobotDARTShaders);
}

namespace robot_dart {
    namespace gui {
        namespace magnum {
            template <typename T = GlfwApplication>
            class BaseGraphics : public Base {
            public:
                BaseGraphics(RobotDARTSimu* simu, const GraphicsConfiguration& configuration = GraphicsConfiguration())
                    : _simu(simu), _world(simu->world()), _width(configuration.width), _height(configuration.height), _frame_counter(0), _enabled(true)
                {
                    Corrade::Utility::Debug magnum_silence_output{nullptr};
                    robot_dart_initialize_magnum_resources();
                    _magnum_app.reset(make_application<T>(simu, configuration));
                }

                virtual ~BaseGraphics() {}

                size_t width() const override { return _magnum_app->camera().width(); }
                size_t height() const override { return _magnum_app->camera().height(); }

                bool done() const override
                {
                    return _magnum_app->done();
                }

                void refresh() override
                {
                    if (!_enabled)
                        return;

                    _magnum_app->render();
                    _frame_counter++;
                }

                void set_enable(bool enable) override
                {
                    _enabled = enable;
                }

                void set_fps(int fps) override { _fps = fps; }

                void look_at(const Eigen::Vector3d& camera_pos,
                    const Eigen::Vector3d& look_at = Eigen::Vector3d(0, 0, 0),
                    const Eigen::Vector3d& up = Eigen::Vector3d(0, 0, 1))
                {
                    _magnum_app->look_at(camera_pos, look_at, up);
                }

                void clear_lights()
                {
                    _magnum_app->clear_lights();
                }

                void add_light(const magnum::gs::Light& light)
                {
                    _magnum_app->add_light(light);
                }

                std::vector<gs::Light>& lights()
                {
                    return _magnum_app->lights();
                }

                size_t num_lights() const
                {
                    return _magnum_app->num_lights();
                }

                magnum::gs::Light& light(size_t i)
                {
                    return _magnum_app->light(i);
                }

                void record_video(const std::string& video_fname, int fps = -1)
                {
                    int fps_computed = (fps == -1) ? _fps : fps;
                    ROBOT_DART_EXCEPTION_INTERNAL_ASSERT(fps_computed != -1 && "Video FPS not set!");

                    _magnum_app->record_video(video_fname, fps_computed);
                }

                bool shadowed() const { return _magnum_app->shadowed(); }
                bool transparent_shadows() const { return _magnum_app->transparent_shadows(); }
                void enable_shadows(bool enable = true, bool transparent = true) { _magnum_app->enable_shadows(enable, transparent); }

                Magnum::Image2D* magnum_image()
                {
                    if (_magnum_app->image())
                        return &(*_magnum_app->image());
                    return nullptr;
                }

                Image image() override
                {
                    auto image = magnum_image();
                    if (image)
                        return gs::rgb_from_image(image);
                    return Image();
                }

                GrayscaleImage depth_image() override { return _magnum_app->depth_image(); }
                GrayscaleImage raw_depth_image() override { return _magnum_app->raw_depth_image(); }

                gs::Camera& camera() { return _magnum_app->camera(); }
                const gs::Camera& camera() const { return _magnum_app->camera(); }

                BaseApplication* magnum_app() { return &*_magnum_app; }
                const BaseApplication* magnum_app() const { return &*_magnum_app; }

            protected:
                RobotDARTSimu* _simu;
                dart::simulation::WorldPtr _world;
                size_t _width, _height, _frame_counter;
                bool _enabled;
                int _fps;
                std::unique_ptr<BaseApplication> _magnum_app;
            };
        } // namespace magnum
    } // namespace gui
} // namespace robot_dart

#endif
