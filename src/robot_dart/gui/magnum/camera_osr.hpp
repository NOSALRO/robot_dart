#ifndef ROBOT_DART_GUI_MAGNUM_CAMERA_OSR_HPP
#define ROBOT_DART_GUI_MAGNUM_CAMERA_OSR_HPP

#include <robot_dart/gui/base.hpp>
#include <robot_dart/gui/magnum/base_application.hpp>
#include <robot_dart/gui/magnum/gs/helper.hpp>

#include <Magnum/GL/Framebuffer.h>
#include <Magnum/PixelFormat.h>

namespace robot_dart {
    namespace gui {
        namespace magnum {
            class CameraOSR : public Base {
            public:
                CameraOSR(const dart::simulation::WorldPtr& world, BaseApplication* app, size_t width, size_t height);
                ~CameraOSR() {}

                bool done() const override
                {
                    return _done;
                }

                void refresh() override
                {
                    if (!_enabled)
                        return;

                    // process next frame
                    if (_frame_counter % _render_period == 0)
                        render();
                    _frame_counter++;
                }

                virtual void set_render_period(double dt) override
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
                    float cx = static_cast<float>(camera_pos[0]);
                    float cy = static_cast<float>(camera_pos[1]);
                    float cz = static_cast<float>(camera_pos[2]);

                    float lx = static_cast<float>(look_at[0]);
                    float ly = static_cast<float>(look_at[1]);
                    float lz = static_cast<float>(look_at[2]);

                    float ux = static_cast<float>(up[0]);
                    float uy = static_cast<float>(up[1]);
                    float uz = static_cast<float>(up[2]);

                    _camera->lookAt(Magnum::Vector3{cx, cy, cz},
                        Magnum::Vector3{lx, ly, lz},
                        Magnum::Vector3{ux, uy, uz});
                }

                void render();

                void set_recording(bool recording) { _camera->record(recording); }
                bool recording() { return _camera->isRecording(); }

                Magnum::Image2D* magnum_image()
                {
                    if (_camera->image())
                        return &(*_camera->image());
                    return nullptr;
                }

                Image image() override
                {
                    auto image = magnum_image();
                    if (image)
                        return gs::rgb_from_image(image);
                    return Image();
                }

            protected:
                Magnum::GL::Framebuffer _framebuffer{Magnum::NoCreate};
                Magnum::PixelFormat _format;

                BaseApplication* _magnum_app;
                size_t _render_period, _width, _height, _frame_counter;
                bool _enabled, _done;

                std::unique_ptr<gs::Camera> _camera;
            };
        } // namespace magnum
    } // namespace gui
} // namespace robot_dart

#endif