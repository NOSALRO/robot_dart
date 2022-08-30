#include "robot_dart.hpp"
#include "utils_headers_dart.hpp"
#include "utils_headers_pybind11.hpp"

#include <robot_dart/robot_dart_simu.hpp>

#ifdef GRAPHIC
#include <robot_dart/gui/magnum/graphics.hpp>
#include <robot_dart/gui/magnum/sensor/camera.hpp>
#include <robot_dart/gui/magnum/windowless_graphics.hpp>
#endif

namespace robot_dart {
    namespace python {
#ifdef GRAPHIC
        void py_gui(py::module& m)
        {
            auto sm = m.def_submodule("gui");
            auto gsmodule = sm.def_submodule("gs");
            auto sensormodule = m.def_submodule("sensor");

            using namespace robot_dart;

            // Helper definitions and classes
            using BaseWindowedGraphics = gui::magnum::BaseGraphics<gui::magnum::GlfwApplication>;
            using BaseWindowlessGraphics = gui::magnum::BaseGraphics<gui::magnum::WindowlessGLApplication>;
            using GraphicsConfiguration = gui::magnum::GraphicsConfiguration;

            using Object3D = gui::magnum::Object3D;
            using Camera = gui::magnum::gs::Camera;

            py::class_<Camera>(gsmodule, "Camera")
                .def(py::init<Object3D&, Magnum::Int, Magnum::Int>())

                .def("set_speed", &Camera::set_speed)
                .def("set_near_plane", &Camera::set_near_plane)
                .def("set_far_plane", &Camera::set_far_plane)
                .def("set_fov", &Camera::set_fov)
                .def("set_camera_params", &Camera::set_camera_params)

                .def("speed", &Camera::speed)
                .def("near_plane", &Camera::near_plane)
                .def("far_plane", &Camera::far_plane)
                .def("fov", &Camera::fov)

                .def("width", &Camera::width)
                .def("height", &Camera::height)

                // We would need to include Magnum for this
                // .def("look_at", &Camera::look_at,
                //     py::arg("camera"),
                //     py::arg("center"),
                //     py::arg("up") = Magnum::Vector3::zAxis())

                .def("transform_lights", &Camera::transform_lights)

                .def("record", &Camera::record,
                    py::arg("recording"),
                    py::arg("recording_depth") = false)
                .def("record_video", &Camera::record_video)
                .def("recording", &Camera::recording)
                .def("recording_depth", &Camera::recording_depth);

            py::class_<gui::Image>(sm, "Image")
                .def(py::init<size_t, size_t, size_t>(),
                    py::arg("width") = 0,
                    py::arg("height") = 0,
                    py::arg("channels") = 3)

                .def_readwrite("width", &gui::Image::width)
                .def_readwrite("height", &gui::Image::height)
                .def_readwrite("channels", &gui::Image::channels)
                .def_readwrite("data", &gui::Image::data);

            py::class_<gui::GrayscaleImage>(sm, "GrayscaleImage")
                .def(py::init<size_t, size_t>(),
                    py::arg("width") = 0,
                    py::arg("height") = 0)

                .def_readwrite("width", &gui::GrayscaleImage::width)
                .def_readwrite("height", &gui::GrayscaleImage::height)
                .def_readwrite("data", &gui::GrayscaleImage::data);

            py::class_<gui::DepthImage>(sm, "DepthImage")
                .def(py::init<size_t, size_t>(),
                    py::arg("width") = 0,
                    py::arg("height") = 0)

                .def_readwrite("width", &gui::DepthImage::width)
                .def_readwrite("height", &gui::DepthImage::height)
                .def_readwrite("data", &gui::DepthImage::data);

            py::class_<GraphicsConfiguration>(sm, "GraphicsConfiguration")
                .def(py::init<size_t, size_t, const std::string&, bool, bool, size_t, size_t, double, bool, bool, bool, const Eigen::Vector4d&>(),
                    py::arg("width") = 640,
                    py::arg("height") = 480,
                    py::arg("title") = "DART",
                    py::arg("shadowed") = true,
                    py::arg("transparent_shadows") = true,
                    py::arg("shadow_map_size") = 1024,
                    py::arg("max_lights") = 3,
                    py::arg("specular_strength") = 0.25,
                    py::arg("draw_main_camera") = true,
                    py::arg("draw_debug") = true,
                    py::arg("draw_text") = true,
                    py::arg("bg_color") = Eigen::Vector4d(0.0, 0.0, 0.0, 1.0))

                .def_readwrite("width", &GraphicsConfiguration::width)
                .def_readwrite("height", &GraphicsConfiguration::height)
                .def_readwrite("title", &GraphicsConfiguration::title)

                .def_readwrite("shadowed", &GraphicsConfiguration::shadowed)
                .def_readwrite("transparent_shadows", &GraphicsConfiguration::transparent_shadows)
                .def_readwrite("shadow_map_size", &GraphicsConfiguration::shadow_map_size)

                .def_readwrite("max_lights", &GraphicsConfiguration::max_lights)
                .def_readwrite("specular_strength", &GraphicsConfiguration::specular_strength)

                .def_readwrite("draw_main_camera", &GraphicsConfiguration::draw_main_camera)
                .def_readwrite("draw_debug", &GraphicsConfiguration::draw_debug)
                .def_readwrite("draw_text", &GraphicsConfiguration::draw_text)

                .def_readwrite("bg_color", &GraphicsConfiguration::bg_color);

            py::class_<gui::Base, std::shared_ptr<gui::Base>>(sm, "Base");
            py::class_<BaseWindowedGraphics, gui::Base, std::shared_ptr<BaseWindowedGraphics>>(sm, "BaseWindowedGraphics");
            py::class_<BaseWindowlessGraphics, gui::Base, std::shared_ptr<BaseWindowlessGraphics>>(sm, "BaseWindowlessGraphics");
            py::class_<gui::magnum::BaseApplication>(sm, "BaseApplication");

            using namespace robot_dart::gui::magnum;
            // Graphics class
            py::class_<Graphics, BaseWindowedGraphics, std::shared_ptr<Graphics>>(sm, "Graphics")
                .def(py::init<const GraphicsConfiguration&>(),
                    py::arg("configuration") = Graphics::default_configuration())

                .def("done", &Graphics::done)
                .def("refresh", &Graphics::refresh)
                .def("set_render_period", &Graphics::set_render_period)
                .def("set_enable", &Graphics::set_enable)

                .def("set_simu", &Graphics::set_simu)
                .def("simu", &Graphics::simu, py::return_value_policy::reference)

                .def("look_at", &Graphics::look_at,
                    py::arg("camera_pos"),
                    py::arg("look_at") = Eigen::Vector3d(0, 0, 0),
                    py::arg("up") = Eigen::Vector3d(0, 0, 1))

                .def("width", &BaseWindowedGraphics::width)
                .def("height", &BaseWindowedGraphics::height)

                .def("clear_lights", &Graphics::clear_lights)
                .def("add_light", &Graphics::add_light, py::keep_alive<2, 1>())
                .def("lights", &Graphics::lights)
                .def("num_lights", &Graphics::num_lights)
                .def("light", &Graphics::light)

                .def("record_video", &Graphics::record_video,
                    py::arg("video_fname"),
                    py::arg("fps") = -1)

                .def("shadowed", &Graphics::shadowed)
                .def("transparent_shadows", &Graphics::transparent_shadows)
                .def("enable_shadows", &Graphics::enable_shadows,
                    py::arg("enable") = true,
                    py::arg("transparent") = true)

                // Magnum::Image2D* magnum_image()
                .def("image", &Graphics::image)
                .def("depth_image", &Graphics::depth_image)
                .def("raw_depth_image", &Graphics::raw_depth_image)
                .def("depth_array", &Graphics::depth_array)

                .def("camera", static_cast<Camera& (Graphics::*)()>(&Graphics::camera), py::return_value_policy::reference)
                .def("camera_intrinsic_matrix", &Graphics::camera_intrinsic_matrix)
                .def("camera_extrinsic_matrix", &Graphics::camera_extrinsic_matrix)

                .def("magnum_app", static_cast<gui::magnum::BaseApplication* (Graphics::*)()>(&Graphics::magnum_app), py::return_value_policy::reference)

                .def_static("default_configuration", &Graphics::default_configuration);

            // WindowlessGraphics class
            py::class_<WindowlessGraphics, BaseWindowlessGraphics, std::shared_ptr<WindowlessGraphics>>(sm, "WindowlessGraphics")
                .def(py::init<const GraphicsConfiguration&>(),
                    py::arg("configuration") = WindowlessGraphics::default_configuration())

                .def("done", &WindowlessGraphics::done)
                .def("refresh", &WindowlessGraphics::refresh)
                .def("set_render_period", &WindowlessGraphics::set_render_period)
                .def("set_enable", &WindowlessGraphics::set_enable)

                .def("look_at", &WindowlessGraphics::look_at,
                    py::arg("camera_pos"),
                    py::arg("look_at") = Eigen::Vector3d(0, 0, 0),
                    py::arg("up") = Eigen::Vector3d(0, 0, 1))

                .def("width", &BaseWindowlessGraphics::width)
                .def("height", &BaseWindowlessGraphics::height)

                .def("clear_lights", &WindowlessGraphics::clear_lights)
                .def("add_light", &WindowlessGraphics::add_light, py::keep_alive<2, 1>())
                .def("lights", &WindowlessGraphics::lights)
                .def("num_lights", &WindowlessGraphics::num_lights)
                .def("light", &WindowlessGraphics::light)

                .def("record_video", &WindowlessGraphics::record_video,
                    py::arg("video_fname"),
                    py::arg("fps") = -1)

                .def("shadowed", &WindowlessGraphics::shadowed)
                .def("transparent_shadows", &WindowlessGraphics::transparent_shadows)
                .def("enable_shadows", &WindowlessGraphics::enable_shadows,
                    py::arg("enable") = true,
                    py::arg("transparent") = true)

                // Magnum::Image2D* magnum_image()
                .def("image", &WindowlessGraphics::image)
                .def("depth_image", &WindowlessGraphics::depth_image)
                .def("raw_depth_image", &WindowlessGraphics::raw_depth_image)
                .def("depth_array", &WindowlessGraphics::depth_array)

                .def("camera", static_cast<Camera& (WindowlessGraphics::*)()>(&WindowlessGraphics::camera), py::return_value_policy::reference)
                .def("camera_intrinsic_matrix", &WindowlessGraphics::camera_intrinsic_matrix)
                .def("camera_extrinsic_matrix", &WindowlessGraphics::camera_extrinsic_matrix)

                .def("magnum_app", static_cast<gui::magnum::BaseApplication* (WindowlessGraphics::*)()>(&WindowlessGraphics::magnum_app), py::return_value_policy::reference)

                .def_static("default_configuration", &WindowlessGraphics::default_configuration);

            sm.def(
                "run_with_gl_context", +[](const std::function<void()>& func, size_t wait_ms) {
                    get_gl_context_with_sleep(my_context, wait_ms);
                    /* Acquire GIL before calling Python code */
                    py::gil_scoped_acquire acquire;
                    func();
                    py::gil_scoped_release release;
                    release_gl_context(my_context);
                });

            sm.def(
                "set_max_contexts", +[](size_t num_contexts) {
                    gui::magnum::GlobalData::instance()->set_max_contexts(num_contexts);
                });

            // Camera sensor class
            py::class_<gui::magnum::sensor::Camera, robot_dart::sensor::Sensor, std::shared_ptr<gui::magnum::sensor::Camera>>(sensormodule, "Camera")
                .def(py::init<gui::magnum::BaseApplication*, size_t, size_t, size_t, bool>(),
                    py::arg("app"),
                    py::arg("width"),
                    py::arg("height"),
                    py::arg("freq") = 30,
                    py::arg("draw_ghost") = false)

                .def("init", &gui::magnum::sensor::Camera::init)

                .def("calculate", &gui::magnum::sensor::Camera::calculate)

                .def("type", &gui::magnum::sensor::Camera::type)

                .def("attach_to_body", &gui::magnum::sensor::Camera::attach_to_body,
                    py::arg("body"),
                    py::arg("tf") = Eigen::Isometry3d::Identity())

                .def("attach_to_joint", &gui::magnum::sensor::Camera::attach_to_joint,
                    py::arg("joint"),
                    py::arg("tf") = Eigen::Isometry3d::Identity())

                .def("camera", static_cast<Camera& (gui::magnum::sensor::Camera::*)()>(&gui::magnum::sensor::Camera::camera), py::return_value_policy::reference)
                .def("camera_intrinsic_matrix", &gui::magnum::sensor::Camera::camera_intrinsic_matrix)
                .def("camera_extrinsic_matrix", &gui::magnum::sensor::Camera::camera_extrinsic_matrix)

                .def("drawing_debug", &gui::magnum::sensor::Camera::drawing_debug)
                .def("draw_debug", &gui::magnum::sensor::Camera::draw_debug,
                    py::arg("draw") = true)

                .def("look_at", &gui::magnum::sensor::Camera::look_at,
                    py::arg("camera_pos"),
                    py::arg("look_at") = Eigen::Vector3d(0, 0, 0),
                    py::arg("up") = Eigen::Vector3d(0, 0, 1))

                .def("record_video", &gui::magnum::sensor::Camera::record_video,
                    py::arg("video_fname"))

                // Magnum::Image2D* magnum_image()
                // Magnum::Image2D* magnum_depth_image()

                .def("image", &gui::magnum::sensor::Camera::image)
                .def("depth_image", &gui::magnum::sensor::Camera::depth_image)
                .def("raw_depth_image", &gui::magnum::sensor::Camera::raw_depth_image)
                .def("depth_array", &gui::magnum::sensor::Camera::depth_array);

            // Helper functions
            sm.def("save_png_image", static_cast<void (*)(const std::string&, const gui::Image&)>(&gui::save_png_image));
            sm.def("save_png_image", static_cast<void (*)(const std::string&, const gui::GrayscaleImage&)>(&gui::save_png_image));
            sm.def("convert_rgb_to_grayscale", gui::convert_rgb_to_grayscale);
            sm.def("point_cloud_from_depth_array", static_cast<std::vector<Eigen::Vector3d> (*)(const gui::DepthImage&, const Eigen::Matrix3d&, const Eigen::Matrix4d&, double)>(&gui::point_cloud_from_depth_array),
                py::arg("depth_image"),
                py::arg("intrinsic_matrix"),
                py::arg("tf"),
                py::arg("far_plane") = 1000.);
            sm.def("point_cloud_from_depth_array", static_cast<void (*)(Eigen::Ref<Eigen::MatrixXd>, const gui::DepthImage&, const Eigen::Matrix3d&, const Eigen::Matrix4d&, double)>(&gui::point_cloud_from_depth_array),
                py::arg("point_cloud"),
                py::arg("depth_image"),
                py::arg("intrinsic_matrix"),
                py::arg("tf"),
                py::arg("far_plane") = 1000.);

            // Material class
            using Material = gui::magnum::gs::Material;
            py::class_<Material>(sm, "Material")
                .def(py::init<const Magnum::Color4&, const Magnum::Color4&, const Magnum::Color4&, Magnum::Float>());
            // TO-DO: Add more functions
            using Light = gui::magnum::gs::Light;
            py::class_<Light>(sm, "Light")
                .def(py::init<const Magnum::Vector4&, const Material&, const Magnum::Vector3&, Magnum::Float, Magnum::Float, const Magnum::Vector4&, bool>());
            // TO-DO: Add more functions

            sm.def("create_point_light", &gui::magnum::gs::create_point_light);
            sm.def("create_spot_light", &gui::magnum::gs::create_spot_light);
            sm.def("create_directional_light", &gui::magnum::gs::create_directional_light);
        }
#endif
    } // namespace python
} // namespace robot_dart
