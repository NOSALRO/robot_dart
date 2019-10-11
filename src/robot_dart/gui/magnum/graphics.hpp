#ifndef ROBOT_DART_GUI_MAGNUM_GRAPHICS_HPP
#define ROBOT_DART_GUI_MAGNUM_GRAPHICS_HPP

#include <robot_dart/gui/base.hpp>
#include <robot_dart/gui/magnum/gs/helper.hpp>
#include <robot_dart/gui/magnum/sdl2_application.hpp>

// We need this for CORRADE_RESOURCE_INITIALIZE
#include <Corrade/Utility/Resource.h>

static void robot_dart_initialize_magnum_resources()
{
    CORRADE_RESOURCE_INITIALIZE(RobotDARTShaders);
}

namespace robot_dart {
    namespace gui {
        namespace magnum {
            template <typename T = Sdl2Application>
            class Graphics : public Base {
            public:
                Graphics(const dart::simulation::WorldPtr& world, unsigned int width = 640, unsigned int height = 480, bool shadowed = true, const std::string& title = "DART")
                    : _world(world), _width(width), _height(height), _frame_counter(0), _enabled(true)
                {
                    Corrade::Utility::Debug magnum_silence_output{nullptr};
                    robot_dart_initialize_magnum_resources();
                    _magnum_app.reset(make_application<T>(world, width, height, title, shadowed));
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
                    _render_period = std::floor((1. / 60.) / dt);
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

                std::vector<gs::Light>& lights()
                {
                    return _magnum_app->lights();
                }

                size_t num_lights() const
                {
                    return _magnum_app->numLights();
                }

                magnum::gs::Light& light(size_t i)
                {
                    return _magnum_app->light(i);
                }

                void set_recording(bool recording, bool recording_depth = false) { _magnum_app->record(recording, recording_depth); }
                bool recording() { return _magnum_app->isRecording(); }
                bool recordingDepth() { return _magnum_app->isDepthRecording(); }

                bool is_shadowed() const { return _magnum_app->isShadowed(); }
                void enable_shadows(bool enable = true) { _magnum_app->enableShadows(enable); }

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

                GrayscaleImage depth_image() override { return _magnum_app->depthImage(); }
                GrayscaleImage raw_depth_image() override { return _magnum_app->rawDepthImage(); }

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
