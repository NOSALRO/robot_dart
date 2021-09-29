#ifndef ROBOT_DART_GUI_MAGNUM_SENSOR_CAMERA_HPP
#define ROBOT_DART_GUI_MAGNUM_SENSOR_CAMERA_HPP

#include <robot_dart/gui/magnum/base_application.hpp>
#include <robot_dart/gui/magnum/gs/helper.hpp>
#include <robot_dart/sensor/sensor.hpp>

#include <Magnum/GL/Framebuffer.h>
#include <Magnum/GL/Renderbuffer.h>
#include <Magnum/PixelFormat.h>

namespace robot_dart {
    namespace gui {
        namespace magnum {
            namespace sensor {
                class Camera : public robot_dart::sensor::Sensor {
                public:
                    Camera(BaseApplication* app, size_t width, size_t height, size_t freq = 30, bool draw_debug = false);
                    ~Camera() {}

                    void init() override;

                    void calculate(double) override;

                    std::string type() const override;

                    void attach_to_body(dart::dynamics::BodyNode* body, const Eigen::Isometry3d& tf = Eigen::Isometry3d::Identity()) override;

                    void attach_to_joint(dart::dynamics::Joint*, const Eigen::Isometry3d&) override
                    {
                        ROBOT_DART_WARNING(true, "You cannot attach a camera to a joint!");
                    }

                    gs::Camera& camera() { return *_camera; }
                    const gs::Camera& camera() const { return *_camera; }

                    Eigen::Matrix3d camera_intrinsic_matrix() const;
                    Eigen::Matrix4d camera_extrinsic_matrix() const;

                    bool drawing_debug() const { return _draw_debug; }
                    void draw_debug(bool draw = true) { _draw_debug = draw; }

                    void look_at(const Eigen::Vector3d& camera_pos, const Eigen::Vector3d& look_at = Eigen::Vector3d(0, 0, 0), const Eigen::Vector3d& up = Eigen::Vector3d(0, 0, 1));

                    // this will use the default FPS of the camera if fps == -1
                    void record_video(const std::string& video_fname)
                    {
                        _camera->record_video(video_fname, _frequency);
                    }

                    Magnum::Image2D* magnum_image()
                    {
                        if (_camera->image())
                            return &(*_camera->image());
                        return nullptr;
                    }

                    Image image()
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
                    GrayscaleImage depth_image();

                    // Image filled with depth buffer values
                    GrayscaleImage raw_depth_image();

                    // "Image" filled with depth buffer values (this returns an array of doubles)
                    DepthImage depth_array();

                protected:
                    Magnum::GL::Framebuffer _framebuffer{Magnum::NoCreate};
                    Magnum::PixelFormat _format;
                    Magnum::GL::Renderbuffer _color, _depth;

                    BaseApplication* _magnum_app;
                    size_t _width, _height;

                    std::unique_ptr<gs::Camera> _camera;

                    bool _draw_debug;
                };
            } // namespace sensor
        } // namespace magnum
    } // namespace gui

    namespace sensor {
        using gui::magnum::sensor::Camera;
    }
} // namespace robot_dart

#endif