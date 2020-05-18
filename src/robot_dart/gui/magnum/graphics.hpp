#ifndef ROBOT_DART_GUI_MAGNUM_GRAPHICS_HPP
#define ROBOT_DART_GUI_MAGNUM_GRAPHICS_HPP

#include <robot_dart/gui/base.hpp>
#include <robot_dart/gui/magnum/glfw_application.hpp>
#include <robot_dart/gui/magnum/gs/helper.hpp>

// We need this for CORRADE_RESOURCE_INITIALIZE
#include <Corrade/Utility/Resource.h>

static void robot_dart_initialize_magnum_resources()
{
    CORRADE_RESOURCE_INITIALIZE(RobotDARTShaders);
}

namespace robot_dart {
    namespace gui {
        namespace magnum {
            template <typename T = GlfwApplication>
            class Graphics : public Base {
            public:
                static constexpr int FPS = 40;
                Graphics(const dart::simulation::WorldPtr& world, const GraphicsConfiguration& configuration = GraphicsConfiguration())
                    : _world(world), _width(configuration.width), _height(configuration.height), _frame_counter(0), _enabled(true)
                {
                    Corrade::Utility::Debug magnum_silence_output{nullptr};
                    robot_dart_initialize_magnum_resources();
                    _magnum_app.reset(make_application<T>(world, configuration));
                    set_render_period(world->getTimeStep());
                }

                ~Graphics() {}

                bool done() const override
                {
                    return _magnum_app->done();
                }

                void refresh() override
                {
                    if (!_enabled)
                        return;

                    // process next frame
                    if (_frame_counter % _render_period == 0)
                        _magnum_app->render();
                    _frame_counter++;
                }

                void set_render_period(double dt) override
                {
                    // we want to display at around 40Hz of simulated time
                    _render_period = std::floor((1. / FPS) / dt);
                    if (_render_period < 1)
                        _render_period = 1;
                }

                void set_enable(bool enable) override
                {
                    _enabled = enable;
                }

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

                void set_recording(bool recording, bool recording_depth = false) { _magnum_app->record(recording, recording_depth); }
                bool recording() { return _magnum_app->recording(); }
                bool recording_depth() { return _magnum_app->recording_depth(); }
                void record_video(const std::string& video_fname, int fps = -1) {
                    int fps_computed = (fps == -1) ? FPS : fps;
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

                void set_speed(const Magnum::Vector2& speed) { _magnum_app->camera().set_speed(speed); }
                void set_near_plane(double near_plane) { _magnum_app->camera().set_near_plane(near_plane); }
                void set_far_plane(double far_plane) { _magnum_app->camera().set_far_plane(far_plane); }
                void set_fov(double fov) { _magnum_app->camera().set_fov(fov); }
                void set_camera_params(double near_plane, double far_plane, double fov, size_t width, size_t height) { _magnum_app->camera().set_camera_params(near_plane, far_plane, fov, width, height); }

                Magnum::Vector2 speed() const { return _magnum_app->camera().speed(); }
                double near_plane() const { return _magnum_app->camera().near_plane(); }
                double far_plane() const { return _magnum_app->camera().far_plane(); }
                double fov() const { return _magnum_app->camera().fov(); }

                BaseApplication* magnum_app() { return &*_magnum_app; }

            protected:
                dart::simulation::WorldPtr _world;
                size_t _render_period, _width, _height, _frame_counter;
                bool _enabled;

                std::unique_ptr<BaseApplication> _magnum_app;
            };
        } // namespace magnum
    } // namespace gui
} // namespace robot_dart

#endif
