#include "robot_dart.hpp"

#include <pybind11/eigen.h>
#include <pybind11/functional.h>
#include <pybind11/stl.h>

#include <robot_dart/robot_dart_simu.hpp>

#ifdef GRAPHIC
#include <robot_dart/gui/magnum/camera_osr.hpp>
#include <robot_dart/gui/magnum/graphics.hpp>
#include <robot_dart/gui/magnum/windowless_gl_application.hpp>
#endif

namespace robot_dart {
    namespace python {
#ifdef GRAPHIC
        void py_gui(py::module& m)
        {
            auto sm = m.def_submodule("gui");
            auto gsmodule = sm.def_submodule("gs");

            using namespace robot_dart;

            // Helper definitions and classes
            using BaseGraphics = gui::magnum::Graphics<gui::magnum::GlfwApplication>;
            using BaseWindowlessGraphics = gui::magnum::Graphics<gui::magnum::WindowlessGLApplication>;
            using GraphicsConfiguration = gui::magnum::GraphicsConfiguration;

            using Object3D = gui::magnum::Object3D;
            using Camera = gui::magnum::gs::Camera;

            py::class_<Camera>(gsmodule, "Camera")
                .def(py::init<Object3D&, Magnum::Int, Magnum::Int>())

                .def("record", &Camera::record)
                .def("recording", &Camera::recording)
                .def("recording_depth", &Camera::recording_depth)

                .def("set_speed", &Camera::set_speed)
                .def("set_near_plane", &Camera::set_near_plane)
                .def("set_far_plane", &Camera::set_far_plane)
                .def("set_fov", &Camera::set_fov)
                .def("set_camera_params", &Camera::set_camera_params)

                .def("speed", &Camera::speed)
                .def("near_plane", &Camera::near_plane)
                .def("far_plane", &Camera::far_plane)
                .def("fov", &Camera::fov);

            class Graphics : public BaseGraphics {
            public:
                using BaseGraphics::BaseGraphics;
            };

            class WindowlessGraphics : public BaseWindowlessGraphics {
            public:
                using BaseWindowlessGraphics::BaseWindowlessGraphics;
            };

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

            py::class_<GraphicsConfiguration>(sm, "GraphicsConfiguration")
                .def(py::init<size_t, size_t, const std::string&, bool, bool, size_t, size_t, bool, bool>(),
                    py::arg("width") = 640,
                    py::arg("height") = 480,
                    py::arg("title") = "DART",
                    py::arg("shadowed") = true,
                    py::arg("transparent_shadows") = true,
                    py::arg("shadow_map_size") = 1024,
                    py::arg("max_lights") = 3,
                    py::arg("draw_main_camera") = true,
                    py::arg("draw_ghosts") = true)

                .def_readwrite("width", &GraphicsConfiguration::width)
                .def_readwrite("height", &GraphicsConfiguration::height)
                .def_readwrite("title", &GraphicsConfiguration::title)

                .def_readwrite("shadowed", &GraphicsConfiguration::shadowed)
                .def_readwrite("transparent_shadows", &GraphicsConfiguration::transparent_shadows)
                .def_readwrite("shadow_map_size", &GraphicsConfiguration::shadow_map_size)

                .def_readwrite("max_lights", &GraphicsConfiguration::max_lights);

            py::class_<gui::Base, std::shared_ptr<gui::Base>>(sm, "Base");
            py::class_<BaseGraphics, gui::Base, std::shared_ptr<BaseGraphics>>(sm, "BaseGraphics");
            py::class_<BaseWindowlessGraphics, gui::Base, std::shared_ptr<BaseWindowlessGraphics>>(sm, "BaseWindowlessGraphics");
            py::class_<gui::magnum::BaseApplication>(sm, "BaseApplication");

            // Graphics class
            py::class_<Graphics, BaseGraphics, std::shared_ptr<Graphics>>(sm, "Graphics")
                .def(py::init<RobotDARTSimu*, const GraphicsConfiguration&>())

                .def("done", &Graphics::done)
                .def("refresh", &Graphics::refresh)
                .def("set_render_period", &Graphics::set_render_period)
                .def("set_enable", &Graphics::set_enable)

                .def("look_at", &Graphics::look_at)

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
                .def("enable_shadows", &Graphics::enable_shadows)

                // Magnum::Image2D* magnum_image()
                .def("image", &Graphics::image)
                .def("depth_image", &Graphics::depth_image)
                .def("raw_depth_image", &Graphics::raw_depth_image)

                .def("camera", (Camera & (Graphics::*)()) & Graphics::camera, py::return_value_policy::reference)

                .def("magnum_app", (gui::magnum::BaseApplication * (Graphics::*)()) & Graphics::magnum_app, py::return_value_policy::reference);

            // WindowlessGraphics class
            py::class_<WindowlessGraphics, BaseWindowlessGraphics, std::shared_ptr<WindowlessGraphics>>(sm, "WindowlessGraphics")
                .def(py::init<RobotDARTSimu*, const GraphicsConfiguration&>())

                .def("done", &WindowlessGraphics::done)
                .def("refresh", &WindowlessGraphics::refresh)
                .def("set_render_period", &WindowlessGraphics::set_render_period)
                .def("set_enable", &WindowlessGraphics::set_enable)

                .def("look_at", &WindowlessGraphics::look_at)

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
                .def("enable_shadows", &WindowlessGraphics::enable_shadows)

                // Magnum::Image2D* magnum_image()
                .def("image", &WindowlessGraphics::image)
                .def("depth_image", &WindowlessGraphics::depth_image)
                .def("raw_depth_image", &WindowlessGraphics::raw_depth_image)

                .def("camera", (Camera & (WindowlessGraphics::*)()) & WindowlessGraphics::camera, py::return_value_policy::reference)

                .def("magnum_app", (gui::magnum::BaseApplication * (WindowlessGraphics::*)()) & WindowlessGraphics::magnum_app, py::return_value_policy::reference);

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

            // CameraOSR class
            py::class_<gui::magnum::CameraOSR, gui::Base, std::shared_ptr<gui::magnum::CameraOSR>>(sm, "CameraOSR")
                .def(py::init<RobotDARTSimu*, gui::magnum::BaseApplication*, size_t, size_t>())

                .def("done", &gui::magnum::CameraOSR::done)
                .def("refresh", &gui::magnum::CameraOSR::refresh)
                .def("set_render_period", &gui::magnum::CameraOSR::set_render_period)
                .def("set_enable", &gui::magnum::CameraOSR::set_enable)

                .def("look_at", &gui::magnum::CameraOSR::look_at)

                .def("render", &gui::magnum::CameraOSR::render)

                .def("record_video", &gui::magnum::CameraOSR::record_video,
                    py::arg("video_fname"),
                    py::arg("fps") = -1)

                // Magnum::Image2D* magnum_image()
                // Magnum::Image2D* magnum_depth_image()

                .def("image", &gui::magnum::CameraOSR::image)
                .def("depth_image", &gui::magnum::CameraOSR::depth_image)
                .def("raw_depth_image", &gui::magnum::CameraOSR::raw_depth_image)

                .def("attach_to", &gui::magnum::CameraOSR::attach_to)

                .def("camera", (Camera & (gui::magnum::CameraOSR::*)()) & gui::magnum::CameraOSR::camera, py::return_value_policy::reference);

            // Helper functions
            sm.def("save_png_image", (void (*)(const std::string&, const gui::Image&)) & gui::save_png_image);
            sm.def("save_png_image", (void (*)(const std::string&, const gui::GrayscaleImage&)) & gui::save_png_image);

            // Material class
            using Material = gui::magnum::gs::Material;
            py::class_<Material>(sm, "Material")
                .def(py::init<const Magnum::Color4&, const Magnum::Color4&, const Magnum::Color4&, Magnum::Float>());
            // TO-DO: Add more functions
            using Light = gui::magnum::gs::Light;
            py::class_<Light>(sm, "Light")
                .def(py::init<const Magnum::Vector4&, const Material&, const Magnum::Vector3&, Magnum::Float, Magnum::Float, const Magnum::Vector4&>());
            // TO-DO: Add more functions

            sm.def("create_point_light", &gui::magnum::gs::create_point_light);
            sm.def("create_spot_light", &gui::magnum::gs::create_spot_light);
            sm.def("create_directional_light", &gui::magnum::gs::create_directional_light);
        }
#endif
    } // namespace python
} // namespace robot_dart
