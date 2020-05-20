#ifndef ROBOT_DART_GUI_MAGNUM_CAMERA_OSR_HPP
#define ROBOT_DART_GUI_MAGNUM_CAMERA_OSR_HPP

#include <robot_dart/gui/base.hpp>
#include <robot_dart/gui/magnum/base_application.hpp>
#include <robot_dart/gui/magnum/gs/helper.hpp>

#include <Magnum/GL/Framebuffer.h>
#include <Magnum/GL/Renderbuffer.h>
#include <Magnum/PixelFormat.h>

namespace robot_dart {
    namespace gui {
        namespace magnum {
            class CameraOSR : public Base {
            public:
                CameraOSR(RobotDARTSimu* simu, BaseApplication* app, size_t width, size_t height, bool draw_ghost = false);
                ~CameraOSR() {}

                bool done() const override { return _done; }
                virtual void refresh() override;
                virtual void set_render_period(double dt) override;
                void set_enable(bool enable) override { _enabled = enable; }

                void look_at(const Eigen::Vector3d& camera_pos, const Eigen::Vector3d& look_at = Eigen::Vector3d(0, 0, 0), const Eigen::Vector3d& up = Eigen::Vector3d(0, 0, 1));

                void render();

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

                Magnum::Image2D* magnum_depth_image()
                {
                    if (_camera->depth_image())
                        return &(*_camera->depth_image());
                    return nullptr;
                }

                // This is for visualization purposes
                GrayscaleImage depth_image() override;

                // Image filled with depth buffer values
                GrayscaleImage raw_depth_image() override;

                // // Point-cloud made from each pixel in depth image
                // // all transformations are according to the camera frame
                // std::vector<Eigen::Vector3d> point_cloud();

                virtual void attach_to(const std::string& name, const Eigen::Isometry3d& tf);

                gs::Camera& camera() { return _magnum_app->camera(); }
                const gs::Camera& camera() const { return _magnum_app->camera(); }

                bool drawing_ghosts() const { return _draw_ghosts; }
                void draw_ghost(bool draw = true) { _draw_ghosts = draw; }

            protected:
                Magnum::GL::Framebuffer _framebuffer{Magnum::NoCreate};
                Magnum::PixelFormat _format;
                Magnum::GL::Renderbuffer _color, _depth;

                BaseApplication* _magnum_app;
                size_t _render_period, _width, _height, _frame_counter;
                bool _enabled, _done;
                bool _attaching = false, _attached = false;
                std::string _attach_to;
                Eigen::Isometry3d _attached_tf;

                std::unique_ptr<gs::Camera> _camera;

                bool _draw_ghosts;
            };
        } // namespace magnum
    } // namespace gui
} // namespace robot_dart

#endif