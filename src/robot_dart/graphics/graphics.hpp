#ifndef ROBOT_DART_GRAPHICS_GRAPHICS_HPP
#define ROBOT_DART_GRAPHICS_GRAPHICS_HPP

#include <robot_dart/graphics/base_graphics.hpp>

#include <dart/gui/osg/osg.hpp>

namespace robot_dart {
    namespace graphics {
        class Graphics : public BaseGraphics {
        public:
            Graphics(dart::simulation::WorldPtr world, bool shadowed = true)
            {
                _enabled = true;
                _world = world;
                _osg_viewer = new dart::gui::osg::Viewer;
                if (shadowed)
                    _osg_viewer->enableShadows();
                _osg_world_node = new dart::gui::osg::WorldNode(world);
                set_render_period(world->getTimeStep());
                _osg_viewer->addWorldNode(_osg_world_node);
                _osg_viewer->switchHeadlights(true);
            }

            bool done() const override
            {
                return _osg_viewer->done();
            }

            void refresh() override
            {
                static int i = 0;

                if (!_enabled)
                    return;

                if (!_osg_viewer->isRealized()) {
                    _osg_viewer->setUpViewInWindow(0, 0, 640, 480);
                    // if (Params::graphics::fullscreen())
                    //     _osg_viewer.setUpViewOnSingleScreen();
                    _osg_viewer->realize();
                }

                // process next frame
                if (i % _render_period == 0)
                    _osg_viewer->frame();
                i++;
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
                _camera_pos = camera_pos;
                _look_at = look_at;
                _camera_up = up;

                // set camera position
                _osg_viewer->getCameraManipulator()->setHomePosition(
                    osg::Vec3d(_camera_pos(0), _camera_pos(1), _camera_pos(2)), osg::Vec3d(_look_at(0), _look_at(1), _look_at(2)), osg::Vec3d(_camera_up(0), _camera_up(1), _camera_up(2)));
                _osg_viewer->home();
            }

            void enable_default_lights(bool enable = true)
            {
                _osg_viewer->switchHeadlights(enable);
            }

        protected:
            Eigen::Vector3d _look_at;
            Eigen::Vector3d _camera_pos;
            Eigen::Vector3d _camera_up;
            osg::ref_ptr<dart::gui::osg::WorldNode> _osg_world_node;
            osg::ref_ptr<dart::gui::osg::Viewer> _osg_viewer;
            dart::simulation::WorldPtr _world;
            int _render_period;
            bool _enabled;
        };
    } // namespace graphics
} // namespace robot_dart

#endif
