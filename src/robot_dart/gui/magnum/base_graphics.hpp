#ifndef ROBOT_DART_GUI_MAGNUM_BASE_GRAPHICS_HPP
#define ROBOT_DART_GUI_MAGNUM_BASE_GRAPHICS_HPP

#include <robot_dart/gui/base.hpp>
#include <robot_dart/gui/magnum/glfw_application.hpp>
#include <robot_dart/gui/magnum/gs/helper.hpp>
#include <robot_dart/robot_dart_simu.hpp>

// We need this for CORRADE_RESOURCE_INITIALIZE
#include <Corrade/Utility/Resource.h>

#include <Magnum/EigenIntegration/Integration.h>

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
                BaseGraphics(const GraphicsConfiguration& configuration = GraphicsConfiguration())
                    : _configuration(configuration), _enabled(true)
                {
                    robot_dart_initialize_magnum_resources();
                }

                virtual ~BaseGraphics() {}

                virtual void set_simu(RobotDARTSimu* simu) override
                {
                    ROBOT_DART_EXCEPTION_ASSERT(simu, "Simulation pointer is null!");
                    _simu = simu;
                    _magnum_app.reset(make_application<T>(simu, _configuration));
                }

                size_t width() const override
                {
                    ROBOT_DART_EXCEPTION_ASSERT(_magnum_app, "MagnumApp pointer is null!");
                    return _magnum_app->camera().width();
                }

                size_t height() const override
                {
                    ROBOT_DART_EXCEPTION_ASSERT(_magnum_app, "MagnumApp pointer is null!");
                    return _magnum_app->camera().height();
                }

                bool done() const override
                {
                    ROBOT_DART_EXCEPTION_ASSERT(_magnum_app, "MagnumApp pointer is null!");
                    return _magnum_app->done();
                }

                void refresh() override
                {
                    if (!_enabled)
                        return;

                    ROBOT_DART_EXCEPTION_ASSERT(_magnum_app, "MagnumApp pointer is null!");
                    _magnum_app->render();
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
                    ROBOT_DART_EXCEPTION_ASSERT(_magnum_app, "MagnumApp pointer is null!");
                    _magnum_app->look_at(camera_pos, look_at, up);
                }

                void clear_lights()
                {
                    ROBOT_DART_EXCEPTION_ASSERT(_magnum_app, "MagnumApp pointer is null!");
                    _magnum_app->clear_lights();
                }

                void add_light(const magnum::gs::Light& light)
                {
                    ROBOT_DART_EXCEPTION_ASSERT(_magnum_app, "MagnumApp pointer is null!");
                    _magnum_app->add_light(light);
                }

                std::vector<gs::Light>& lights()
                {
                    ROBOT_DART_EXCEPTION_ASSERT(_magnum_app, "MagnumApp pointer is null!");
                    return _magnum_app->lights();
                }

                size_t num_lights() const
                {
                    ROBOT_DART_EXCEPTION_ASSERT(_magnum_app, "MagnumApp pointer is null!");
                    return _magnum_app->num_lights();
                }

                magnum::gs::Light& light(size_t i)
                {
                    ROBOT_DART_EXCEPTION_ASSERT(_magnum_app, "MagnumApp pointer is null!");
                    return _magnum_app->light(i);
                }

                void record_video(const std::string& video_fname, int fps = -1)
                {
                    int fps_computed = (fps == -1) ? _fps : fps;
                    ROBOT_DART_EXCEPTION_ASSERT(fps_computed != -1, "Video FPS not set!");
                    ROBOT_DART_EXCEPTION_ASSERT(_magnum_app, "MagnumApp pointer is null!");

                    _magnum_app->record_video(video_fname, fps_computed);
                }

                bool shadowed() const
                {
                    ROBOT_DART_EXCEPTION_ASSERT(_magnum_app, "MagnumApp pointer is null!");
                    return _magnum_app->shadowed();
                }

                bool transparent_shadows() const
                {
                    ROBOT_DART_EXCEPTION_ASSERT(_magnum_app, "MagnumApp pointer is null!");
                    return _magnum_app->transparent_shadows();
                }

                void enable_shadows(bool enable = true, bool transparent = true)
                {
                    ROBOT_DART_EXCEPTION_ASSERT(_magnum_app, "MagnumApp pointer is null!");
                    _magnum_app->enable_shadows(enable, transparent);
                }

                Magnum::Image2D* magnum_image()
                {
                    ROBOT_DART_EXCEPTION_ASSERT(_magnum_app, "MagnumApp pointer is null!");
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

                GrayscaleImage depth_image() override
                {
                    ROBOT_DART_EXCEPTION_ASSERT(_magnum_app, "MagnumApp pointer is null!");
                    return _magnum_app->depth_image();
                }

                GrayscaleImage raw_depth_image() override
                {
                    ROBOT_DART_EXCEPTION_ASSERT(_magnum_app, "MagnumApp pointer is null!");
                    return _magnum_app->raw_depth_image();
                }

                gs::Camera& camera()
                {
                    ROBOT_DART_EXCEPTION_ASSERT(_magnum_app, "MagnumApp pointer is null!");
                    return _magnum_app->camera();
                }

                const gs::Camera& camera() const
                {
                    ROBOT_DART_EXCEPTION_ASSERT(_magnum_app, "MagnumApp pointer is null!");
                    return _magnum_app->camera();
                }

                Eigen::Matrix3d camera_intrinsic_matrix() const
                {
                    ROBOT_DART_EXCEPTION_ASSERT(_magnum_app, "MagnumApp pointer is null!");
                    return Magnum::EigenIntegration::cast<Eigen::Matrix3d>(Magnum::Matrix3d(_magnum_app->camera().intrinsic_matrix()));
                }

                Eigen::Matrix4d camera_extrinsic_matrix() const
                {
                    ROBOT_DART_EXCEPTION_ASSERT(_magnum_app, "MagnumApp pointer is null!");
                    return Magnum::EigenIntegration::cast<Eigen::Matrix4d>(Magnum::Matrix4d(_magnum_app->camera().extrinsic_matrix()));
                }

                BaseApplication* magnum_app()
                {
                    ROBOT_DART_EXCEPTION_ASSERT(_magnum_app, "MagnumApp pointer is null!");
                    return &*_magnum_app;
                }

                const BaseApplication* magnum_app() const
                {
                    ROBOT_DART_EXCEPTION_ASSERT(_magnum_app, "MagnumApp pointer is null!");
                    return &*_magnum_app;
                }

            protected:
                GraphicsConfiguration _configuration;
                bool _enabled;
                int _fps;
                std::unique_ptr<BaseApplication> _magnum_app;

                Corrade::Utility::Debug _magnum_silence_output{nullptr};
            };
        } // namespace magnum
    } // namespace gui
} // namespace robot_dart

#endif
