#ifndef ROBOT_DART_GUI_MAGNUM_GRAPHICS_HPP
#define ROBOT_DART_GUI_MAGNUM_GRAPHICS_HPP

#include <robot_dart/gui/base.hpp>
#include <robot_dart/gui/magnum/sdl2_application.hpp>

namespace robot_dart {
    namespace gui {
        namespace magnum {
            template <typename T = Sdl2Application>
            class Graphics : public Base {
            public:
                Graphics(const dart::simulation::WorldPtr& world, unsigned int width = 640, unsigned int height = 480, bool shadowed = true, const std::string& title = "DART")
                    : _world(world), _width(width), _height(height), _frame_counter(0), _enabled(true)
                {
                    _magnum_app.reset(make_application<T>(world, width, height, title));
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
                    // we want to display at around 60Hz of simulated time
                    _render_period = std::floor(0.015 / dt);
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
                    _magnum_app->lookAt(camera_pos, look_at, up);
                }

                void clear_lights()
                {
                    _magnum_app->clearLights();
                }

                void add_light(const magnum::gs::Light& light)
                {
                    _magnum_app->addLight(light);
                }

                size_t num_lights() const
                {
                    return _magnum_app->numLights();
                }

                magnum::gs::Light& light(size_t i)
                {
                    return _magnum_app->light(i);
                }

                void set_recording(bool recording) { _magnum_app->record(recording); }
                bool recording() { return _magnum_app->isRecording(); }

                Magnum::Image2D* image()
                {
                    if (_magnum_app->image())
                        return &(*_magnum_app->image());
                    return nullptr;
                }

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
