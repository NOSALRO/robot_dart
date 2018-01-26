#ifndef ROBOT_DART_GRAPHICS_HPP
#define ROBOT_DART_GRAPHICS_HPP

#include <robot_dart/macros.hpp>
#include <robot_dart/no_graphics.hpp>

#include <dart/gui/osg/osg.hpp>

namespace robot_dart {

    namespace defaults {
        struct graphics {
            RS_PARAM_ARRAY(int, resolution, 640, 480);
            RS_PARAM(bool, fullscreen, false);
        };
    } // namespace defaults

    template <typename Params>
    class Graphics : public No_Graphics {
    public:
        Graphics() {}

        template <typename World>
        Graphics(World world)
        {
            _enabled = true;
            _fixed_camera = false;
            _free_camera = false;
            _osg_world_node = new dart::gui::osg::WorldNode(world);
            set_render_period(world->getTimeStep());
            _osg_viewer.addWorldNode(_osg_world_node);
        }

        bool done() const
        {
            return _osg_viewer.done();
        }

        template <typename Simu>
        void refresh(Simu& simu)
        {
            static int i = 0;

            if (!_enabled)
                return;

            if (!_osg_viewer.isRealized()) {
                _osg_viewer.setUpViewInWindow(0, 0, Params::graphics::resolution(0), Params::graphics::resolution(1));
                if (Params::graphics::fullscreen())
                    _osg_viewer.setUpViewOnSingleScreen();
                _osg_viewer.realize();
            }

            // if (!_free_camera && !_fixed_camera) {
            //     auto COM = simu.robot()->skeleton()->getCOM();
            //     // set camera to follow hexapod
            //     _osg_viewer.getCameraManipulator()->setHomePosition(
            //         osg::Vec3d(-0.5, 3, 1), osg::Vec3d(COM(0), COM(1), COM(2)), osg::Vec3d(0, 0, 1));
            //     _osg_viewer.home();
            // }
            // process next frame
            if (i % _render_period == 0)
                _osg_viewer.frame();
            i++;
        }

        void set_render_period(double dt)
        {
            // we want to display at around 60Hz of simulated time
            _render_period = std::floor(0.015 / dt);
            if (_render_period < 1)
                _render_period = 1;
        }

        void set_enable(bool enable)
        {
            _enabled = enable;
        }

        void fixed_camera(const Eigen::Vector3d& camera_pos,
            const Eigen::Vector3d& look_at = Eigen::Vector3d(0, 0, 0),
            const Eigen::Vector3d& up = Eigen::Vector3d(0, 0, 1))
        {
            _fixed_camera = true;
            _camera_pos = camera_pos;
            _look_at = look_at;
            _camera_up = up;

            // set camera position
            _osg_viewer.getCameraManipulator()->setHomePosition(
                osg::Vec3d(_camera_pos(0), _camera_pos(1), _camera_pos(2)), osg::Vec3d(_look_at(0), _look_at(1), _look_at(2)), osg::Vec3d(_camera_up(0), _camera_up(1), _camera_up(2)));
            _osg_viewer.home();
        }

        // void follow_robot()
        // {
        //     _fixed_camera = false;
        //     _free_camera = false;
        // }

        void free_camera()
        {
            _free_camera = true;
            _fixed_camera = false;
        }

    protected:
        bool _fixed_camera;
        bool _free_camera;
        Eigen::Vector3d _look_at;
        Eigen::Vector3d _camera_pos;
        Eigen::Vector3d _camera_up;
        osg::ref_ptr<dart::gui::osg::WorldNode> _osg_world_node;
        dart::gui::osg::Viewer _osg_viewer;
        int _render_period;
        bool _enabled;
    };
} // namespace robot_dart

#endif
