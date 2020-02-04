#include <pybind11/eigen.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include <robot_dart/robot.hpp>
#include <robot_dart/robot_dart_simu.hpp>

#include <robot_dart/control/pd_control.hpp>
#include <robot_dart/control/robot_control.hpp>

#ifdef GRAPHIC
#include <robot_dart/gui/magnum/camera_osr.hpp>
#include <robot_dart/gui/magnum/graphics.hpp>
#include <robot_dart/gui/magnum/windowless_gl_application.hpp>
#endif

namespace py = pybind11;

void py_robot(py::module& m)
{
    using namespace robot_dart;
    // Robot class
    py::class_<Robot, std::shared_ptr<Robot>>(m, "Robot")
        .def(py::init<const std::string&, const std::vector<std::pair<std::string, std::string>>&, const std::string&, bool>())
        .def(py::init<const std::string&, const std::string&, bool>())
        .def(py::init<dart::dynamics::SkeletonPtr, const std::string&>())

        .def("clone", &Robot::clone)
        .def("skeleton", &Robot::skeleton)

        .def("name", &Robot::name)

        .def("update", &Robot::update)

        .def("reinit_controllers", &Robot::reinit_controllers)
        .def("num_controllers", &Robot::num_controllers)
        .def("controllers", &Robot::controllers)
        .def("active_controllers", &Robot::active_controllers)
        .def("controller", &Robot::controller)
        .def("add_controller", &Robot::add_controller)
        .def("remove_controller", (void (Robot::*)(const std::shared_ptr<control::RobotControl>&)) & Robot::remove_controller)
        .def("remove_controller", (void (Robot::*)(size_t)) & Robot::remove_controller)
        .def("clear_controllers", &Robot::clear_controllers)

        .def("fix_to_world", &Robot::fix_to_world)
        .def("free_from_world", &Robot::free_from_world)
        .def("fixed", &Robot::fixed)
        .def("free", &Robot::free)

        .def("set_actuator_type", &Robot::set_actuator_type)
        .def("set_actuator_types", (void (Robot::*)(const std::vector<dart::dynamics::Joint::ActuatorType>&)) & Robot::set_actuator_types)
        .def("set_actuator_types", (void (Robot::*)(dart::dynamics::Joint::ActuatorType)) & Robot::set_actuator_types)

        .def("actuator_type", &Robot::actuator_type)
        .def("actuator_types", &Robot::actuator_types)

        .def("set_position_enforced", (void (Robot::*)(size_t, bool)) & Robot::set_position_enforced)
        .def("set_position_enforced", (void (Robot::*)(const std::vector<bool>&)) & Robot::set_position_enforced)
        .def("set_position_enforced", (void (Robot::*)(bool)) & Robot::set_position_enforced)

        .def("position_enforced", (bool (Robot::*)(size_t) const) & Robot::position_enforced)
        .def("position_enforced", (std::vector<bool>(Robot::*)() const) & Robot::position_enforced)

        .def("set_damping_coeff", &Robot::set_damping_coeff)
        .def("set_damping_coeffs", (void (Robot::*)(const std::vector<double>&)) & Robot::set_damping_coeffs)
        .def("set_damping_coeffs", (void (Robot::*)(double)) & Robot::set_damping_coeffs)

        .def("damping_coeff", &Robot::damping_coeff)
        .def("damping_coeffs", &Robot::damping_coeffs)

        .def("set_cfriction_coeff", &Robot::set_cfriction_coeff)
        .def("set_cfriction_coeffs", (void (Robot::*)(const std::vector<double>&)) & Robot::set_cfriction_coeffs)
        .def("set_cfriction_coeffs", (void (Robot::*)(double)) & Robot::set_cfriction_coeffs)

        .def("cfriction_coeff", &Robot::cfriction_coeff)
        .def("cfriction_coeffs", &Robot::cfriction_coeffs)

        .def("set_base_pose", &Robot::set_base_pose)

        .def("body_pos", &Robot::body_pos)
        .def("body_rot", &Robot::body_rot)
        .def("body_trans", &Robot::body_trans)

        .def("set_color_mode", (void (Robot::*)(dart::dynamics::MeshShape::ColorMode)) & Robot::set_color_mode)
        .def("set_color_mode", (void (Robot::*)(dart::dynamics::MeshShape::ColorMode, const std::string&)) & Robot::set_color_mode)

        .def_static("create_box", &Robot::create_box)
        .def_static("create_ellipsoid", &Robot::create_ellipsoid);
}

void py_simu(py::module& m)
{
    using namespace robot_dart;
    // RobotDARTSimu class
    py::class_<RobotDARTSimu>(m, "RobotDARTSimu")
        .def(py::init<double>())

        .def("run", &RobotDARTSimu::run)

        .def("graphics", &RobotDARTSimu::graphics)
        .def("set_graphics", &RobotDARTSimu::set_graphics)

        .def("world", &RobotDARTSimu::world)

        // .def("add_descriptor", (void (RobotDARTSimu::*)(const std::shared_ptr<descriptor::BaseDescriptor>&)) & RobotDARTSimu::add_descriptor)
        // .def("descriptors", &RobotDARTSimu::descriptors)
        // .def("descriptor", &RobotDARTSimu::descriptor)

        // .def("remove_descriptor", (void (RobotDARTSimu::*)(const std::shared_ptr<descriptor::BaseDescriptor>&)) & RobotDARTSimu::remove_descriptor)
        // .def("remove_descriptor", (void (RobotDARTSimu::*)(size_t)) & RobotDARTSimu::remove_descriptor)
        // .def("clear_descriptors", &RobotDARTSimu::clear_descriptors)

        .def("add_camera", &RobotDARTSimu::add_camera)
        .def("cameras", &RobotDARTSimu::cameras)
        .def("camera", &RobotDARTSimu::camera)

        .def("remove_camera", (void (RobotDARTSimu::*)(const std::shared_ptr<gui::Base>&)) & RobotDARTSimu::remove_camera)
        .def("remove_camera", (void (RobotDARTSimu::*)(size_t)) & RobotDARTSimu::remove_camera)
        .def("clear_cameras", &RobotDARTSimu::clear_cameras)

        .def("step", &RobotDARTSimu::step)
        .def("set_step", &RobotDARTSimu::set_step)

        .def("stop_sim", &RobotDARTSimu::stop_sim)
        .def("halted_sim", &RobotDARTSimu::halted_sim)

        .def("num_robots", &RobotDARTSimu::num_robots)
        .def("robots", &RobotDARTSimu::robots)
        .def("robot", &RobotDARTSimu::robot)

        .def("add_robot", &RobotDARTSimu::add_robot)
        .def("remove_robot", (void (RobotDARTSimu::*)(const std::shared_ptr<Robot>&)) & RobotDARTSimu::remove_robot)
        .def("remove_robot", (void (RobotDARTSimu::*)(size_t)) & RobotDARTSimu::remove_robot)
        .def("clear_robots", &RobotDARTSimu::clear_robots)

        .def("add_floor", &RobotDARTSimu::add_floor)
        .def("add_checkerboard_floor", &RobotDARTSimu::add_checkerboard_floor);
}

void py_control(py::module& m)
{
    using namespace robot_dart;
    using namespace robot_dart::control;

    class PyRobotControl : public RobotControl {
    public:
        using RobotControl::RobotControl;

        /* Trampolines */
        void configure() override
        {
            PYBIND11_OVERLOAD_PURE(
                void, /* Return type */
                RobotControl, /* Parent class */
                configure /* Name of function in C++ (must match Python name) */
            );
        }

        Eigen::VectorXd calculate(double t) override
        {
            PYBIND11_OVERLOAD_PURE(
                Eigen::VectorXd, /* Return type */
                RobotControl, /* Parent class */
                calculate, /* Name of function in C++ (must match Python name) */
                t);
        }

        std::shared_ptr<RobotControl> clone() const override
        {
            PYBIND11_OVERLOAD_PURE(
                std::shared_ptr<RobotControl>, /* Return type */
                RobotControl, /* Parent class */
                clone /* Name of function in C++ (must match Python name) */
            );
        }
    };

    class PublicistRobotControl : public RobotControl {
    public:
        using RobotControl::_active;
        using RobotControl::_ctrl;
        using RobotControl::_robot;
    };

    // RobotControl class
    py::class_<RobotControl, PyRobotControl, std::shared_ptr<RobotControl>>(m, "RobotControl")
        .def(py::init<const std::vector<double>&, bool>())

        .def_readwrite("_active", &PublicistRobotControl::_active)
        .def_readwrite("_ctrl", &PublicistRobotControl::_ctrl)
        .def_readonly("_robot", &PublicistRobotControl::_robot)

        .def("set_parameters", &RobotControl::set_parameters)
        .def("parameters", &RobotControl::parameters)

        .def("init", &RobotControl::init)

        .def("set_robot", &RobotControl::set_robot)
        .def("robot", &RobotControl::robot)

        .def("activate", &RobotControl::activate)
        .def("active", &RobotControl::active)

        .def("fully_controlled", &RobotControl::fully_controlled)
        .def("set_full_control", &RobotControl::set_full_control)

        .def("weight", &RobotControl::weight)
        .def("set_weight", &RobotControl::set_weight)

        // virtual void configure() = 0;
        .def("configure", &RobotControl::configure)

        .def("commands", &RobotControl::commands)

        // virtual Eigen::VectorXd calculate(double t) = 0;
        .def("calculate", &RobotControl::calculate)
        // virtual std::shared_ptr<RobotControl> clone() const = 0;
        .def("clone", &RobotControl::clone)

        .def("get_positions", &RobotControl::get_positions)
        .def("set_positions", &RobotControl::set_positions)

        .def("get_velocities", &RobotControl::get_velocities)
        .def("set_velocities", &RobotControl::set_velocities)

        .def("get_accelerations", &RobotControl::get_accelerations)
        .def("set_accelerations", &RobotControl::set_accelerations)

        .def("get_forces", &RobotControl::get_forces)
        .def("set_forces", &RobotControl::set_forces);

    // PDControl class
    py::class_<PDControl, RobotControl, std::shared_ptr<PDControl>>(m, "PDControl")
        .def(py::init<const std::vector<double>&, bool>())

        .def("configure", &PDControl::configure)
        .def("calculate", &PDControl::calculate)

        .def("set_pd", (void (PDControl::*)(double, double)) & PDControl::set_pd)
        .def("set_pd", (void (PDControl::*)(const Eigen::VectorXd&, const Eigen::VectorXd&)) & PDControl::set_pd)

        .def("pd", &PDControl::pd)

        .def("clone", &PDControl::clone);
}

#ifdef GRAPHIC
void py_gui(py::module& m)
{
    auto sm = m.def_submodule("gui");

    using namespace robot_dart;

    // Helper definitions and classes
    using BaseGraphics = gui::magnum::Graphics<gui::magnum::GlfwApplication>;
    using BaseWindowlessGraphics = gui::magnum::Graphics<gui::magnum::WindowlessGLApplication>;

    class Graphics : public BaseGraphics {
    public:
        using BaseGraphics::BaseGraphics;
    };

    class WindowlessGraphics : public BaseWindowlessGraphics {
    public:
        using BaseWindowlessGraphics::BaseWindowlessGraphics;
    };

    py::class_<gui::Base, std::shared_ptr<gui::Base>>(sm, "Base");
    py::class_<BaseGraphics, gui::Base, std::shared_ptr<BaseGraphics>>(sm, "BaseGraphics");
    py::class_<BaseWindowlessGraphics, gui::Base, std::shared_ptr<BaseWindowlessGraphics>>(sm, "BaseWindowlessGraphics");
    py::class_<gui::magnum::BaseApplication>(sm, "BaseApplication");

    // Graphics class
    py::class_<Graphics, BaseGraphics, std::shared_ptr<Graphics>>(sm, "Graphics")
        .def(py::init<const dart::simulation::WorldPtr&, unsigned int, unsigned int, bool, bool, const std::string&>())

        .def("done", &Graphics::done)
        .def("refresh", &Graphics::refresh)
        .def("set_render_period", &Graphics::set_render_period)
        .def("set_enable", &Graphics::set_enable)

        .def("look_at", &Graphics::look_at)

        .def("clear_lights", &Graphics::clear_lights)
        .def("add_light", &Graphics::add_light)
        .def("lights", &Graphics::lights)
        .def("num_lights", &Graphics::num_lights)
        .def("light", &Graphics::light)

        .def("set_recording", &Graphics::set_recording)
        .def("recording", &Graphics::recording)
        .def("recording_depth", &Graphics::recording_depth)

        .def("is_shadowed", &Graphics::is_shadowed)
        .def("enable_shadows", &Graphics::enable_shadows)

        // Magnum::Image2D* magnum_image()
        .def("image", &Graphics::image)
        .def("depth_image", &Graphics::depth_image)
        .def("raw_depth_image", &Graphics::raw_depth_image)

        .def("set_speed", &Graphics::set_speed)
        .def("set_near_plane", &Graphics::set_near_plane)
        .def("set_far_plane", &Graphics::set_far_plane)
        .def("set_fov", &Graphics::set_fov)
        .def("set_camera_params", &Graphics::set_camera_params)

        .def("speed", &Graphics::speed)
        .def("near_plane", &Graphics::near_plane)
        .def("far_plane", &Graphics::far_plane)
        .def("fov", &Graphics::fov)

        .def("magnum_app", &Graphics::magnum_app, py::return_value_policy::reference);

    // WindowlessGraphics class
    py::class_<WindowlessGraphics, BaseWindowlessGraphics, std::shared_ptr<WindowlessGraphics>>(sm, "WindowlessGraphics")
        .def(py::init<const dart::simulation::WorldPtr&, unsigned int, unsigned int, bool, bool, const std::string&>())

        .def("done", &WindowlessGraphics::done)
        .def("refresh", &WindowlessGraphics::refresh)
        .def("set_render_period", &WindowlessGraphics::set_render_period)
        .def("set_enable", &WindowlessGraphics::set_enable)

        .def("look_at", &WindowlessGraphics::look_at)

        .def("clear_lights", &WindowlessGraphics::clear_lights)
        .def("add_light", &WindowlessGraphics::add_light)
        .def("lights", &WindowlessGraphics::lights)
        .def("num_lights", &WindowlessGraphics::num_lights)
        .def("light", &WindowlessGraphics::light)

        .def("set_recording", &WindowlessGraphics::set_recording)
        .def("recording", &WindowlessGraphics::recording)
        .def("recording_depth", &WindowlessGraphics::recording_depth)

        .def("is_shadowed", &WindowlessGraphics::is_shadowed)
        .def("enable_shadows", &WindowlessGraphics::enable_shadows)

        // Magnum::Image2D* magnum_image()
        .def("image", &WindowlessGraphics::image)
        .def("depth_image", &WindowlessGraphics::depth_image)
        .def("raw_depth_image", &WindowlessGraphics::raw_depth_image)

        .def("set_speed", &WindowlessGraphics::set_speed)
        .def("set_near_plane", &WindowlessGraphics::set_near_plane)
        .def("set_far_plane", &WindowlessGraphics::set_far_plane)
        .def("set_fov", &WindowlessGraphics::set_fov)
        .def("set_camera_params", &WindowlessGraphics::set_camera_params)

        .def("speed", &WindowlessGraphics::speed)
        .def("near_plane", &WindowlessGraphics::near_plane)
        .def("far_plane", &WindowlessGraphics::far_plane)
        .def("fov", &WindowlessGraphics::fov);

    // get_gl_context
    // release_gl_context

    // CameraOSR class
    py::class_<gui::magnum::CameraOSR, gui::Base, std::shared_ptr<gui::magnum::CameraOSR>>(sm, "CameraOSR")
        // .def(py::init<const dart::simulation::WorldPtr&, gui::magnum::BaseApplication*, size_t, size_t>())
        .def(py::init(+[](const dart::simulation::WorldPtr& world, Graphics& gr, size_t w, size_t h) {
            return std::make_shared<gui::magnum::CameraOSR>(world, gr.magnum_app(), w, h);
        }))
        .def(py::init(+[](const dart::simulation::WorldPtr& world, WindowlessGraphics& gr, size_t w, size_t h) {
            return std::make_shared<gui::magnum::CameraOSR>(world, gr.magnum_app(), w, h);
        }))

        .def("done", &gui::magnum::CameraOSR::done)
        .def("refresh", &gui::magnum::CameraOSR::refresh)
        .def("set_render_period", &gui::magnum::CameraOSR::set_render_period)
        .def("set_enable", &gui::magnum::CameraOSR::set_enable)

        .def("look_at", &gui::magnum::CameraOSR::look_at)

        .def("render", &gui::magnum::CameraOSR::render)

        .def("set_recording", &gui::magnum::CameraOSR::set_recording)
        .def("recording", &gui::magnum::CameraOSR::recording)
        .def("recording_depth", &gui::magnum::CameraOSR::recording_depth)

        // Magnum::Image2D* magnum_image()
        // Magnum::Image2D* magnum_depth_image()

        .def("image", &gui::magnum::CameraOSR::image)
        .def("depth_image", &gui::magnum::CameraOSR::depth_image)
        .def("raw_depth_image", &gui::magnum::CameraOSR::raw_depth_image)

        .def("attach_to", &gui::magnum::CameraOSR::attach_to)

        .def("set_speed", &gui::magnum::CameraOSR::set_speed)
        .def("set_near_plane", &gui::magnum::CameraOSR::set_near_plane)
        .def("set_far_plane", &gui::magnum::CameraOSR::set_far_plane)
        .def("set_fov", &gui::magnum::CameraOSR::set_fov)
        .def("set_camera_params", &gui::magnum::CameraOSR::set_camera_params)

        .def("speed", &gui::magnum::CameraOSR::speed)
        .def("near_plane", &gui::magnum::CameraOSR::near_plane)
        .def("far_plane", &gui::magnum::CameraOSR::far_plane)
        .def("fov", &gui::magnum::CameraOSR::fov);

    // Helper functions
    sm.def("save_png_image", (void (*)(const std::string&, const gui::Image&)) & gui::save_png_image);
    sm.def("save_png_image", (void (*)(const std::string&, const gui::GrayscaleImage&)) & gui::save_png_image);
}
#endif

PYBIND11_MODULE(RobotDART, m)
{
    // Load dartpy
    py::module::import("dartpy");

    m.doc() = "RobotDART: Python API of robot_dart";

    py_simu(m);
    py_robot(m);
    py_control(m);

#ifdef GRAPHIC
    py_gui(m);
#endif
}