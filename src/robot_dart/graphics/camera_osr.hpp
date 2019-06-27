#ifndef ROBOT_DART_GRAPHICS_CAMERAOSR_HPP
#define ROBOT_DART_GRAPHICS_CAMERAOSR_HPP
#include <unistd.h>

#include <robot_dart/graphics/base_graphics.hpp>
#include <robot_dart/graphics/pbuffer_manager.hpp>
#include <robot_dart/utils.hpp>

#include <dart/gui/osg/osg.hpp>

#include <osgDB/WriteFile>

namespace robot_dart {
    namespace graphics {
        template <typename Viewer_t>
        class GetScreen : public ::osg::Camera::DrawCallback {
        public:
            GetScreen(Viewer_t* viewer, ::osg::ref_ptr<::osg::Camera> cam) : _viewer(viewer), _osg_camera(cam) {}

            virtual void operator()(::osg::RenderInfo& renderInfo) const
            {
                ::osg::Camera::DrawCallback::operator()(renderInfo);

                int x, y;
                unsigned int width, height;
                ::osg::ref_ptr<::osg::Viewport> vp = _osg_camera->getViewport();
                x = vp->x();
                y = vp->y();
                width = vp->width();
                height = vp->height();

                _viewer->image()->readPixels(x, y, width, height, GL_LUMINANCE, GL_UNSIGNED_BYTE);

                if (_viewer->recording()) {
                    if (!_viewer->filename().empty()) {
                        bool saved = ::osgDB::writeImageFile(*_viewer->image(), _viewer->filename());
                        ROBOT_DART_WARNING(!saved, "GetScreen unable to save image to file '" + _viewer->filename() + "'");
                    }
                }
                _viewer->set_shot_done(true);
            }

        protected:
            Viewer_t* _viewer;
            ::osg::ref_ptr<::osg::Camera> _osg_camera;
        };

        // Camera-OffScreen-Rendering
        class CameraOSR : public BaseGraphics {

        public:
            CameraOSR(const dart::simulation::WorldPtr& world, unsigned int width = 640, unsigned int height = 480, bool shadowed = true) : _world(world), _width(width), _height(height), _frame_counter(0), _enabled(true), _buffer_valid(true), _recording(false), _filename("camera_record.png"), _image(new osg::Image), _shot_done(false)

            {
                if (!global::pbufferManager) {
                    std::cout << "Error: pbufferManager seems not to be started" << std::endl;
                    assert(0);
                }

                _osg_viewer = new dart::gui::osg::Viewer;
                _osg_viewer->setThreadingModel(osgViewer::ViewerBase::ThreadingModel::SingleThreaded);
                _osg_viewer->setUseConfigureAffinity(false);
                _pbuffer = global::pbufferManager->get_pbuffer();

                if (_pbuffer.valid()) {
                    ::osg::ref_ptr<osg::Camera> camera = _osg_viewer->getCamera();
                    camera->setGraphicsContext(_pbuffer.get());
                    camera->setViewport(new ::osg::Viewport(0, 0, width, height));
                    //camera->setClearColor(::osg::Vec4(0.5f,0.5f,0.5f,0.0f));
                    camera->setDrawBuffer(GL_BACK);
                    camera->setReadBuffer(GL_BACK);
                    _osg_viewer->realize();
                    _osg_world_node = new dart::gui::osg::WorldNode(world);
                    if (shadowed)
                        _osg_world_node->setShadowTechnique(dart::gui::osg::WorldNode::createDefaultShadowTechnique(_osg_viewer));
                    set_render_period(world->getTimeStep());
                    _osg_viewer->addWorldNode(_osg_world_node);
                    _osg_viewer->switchHeadlights(true);
                    _osg_viewer->getCamera()->setFinalDrawCallback(new GetScreen<CameraOSR>(this, _osg_viewer->getCamera()));
                }
                else {
                    ROBOT_DART_WARNING(true, "Error configuring pbuffer in CameraOSR! Camera will be disabled!");
                    _enabled = false;
                    _buffer_valid = false;
                }
            }

            ~CameraOSR()
            {
                _osg_viewer->removeWorldNode(_osg_world_node); // This line fixes a memory leak from DART
                // This following lines will still be necessary after DART fixes their side.
                _osg_viewer->getCamera()->setFinalDrawCallback(0);
                _osg_viewer->getCamera()->setGraphicsContext(0);
                _osg_world_node = NULL;
                _osg_viewer = NULL;
                _image = NULL;

                global::pbufferManager->release_pbuffer(_pbuffer);
                _pbuffer = NULL;
            }

            bool done() const override
            {
                return _osg_viewer->done();
            }

            void refresh() override
            {
                if (!_buffer_valid || !_enabled)
                    return;

                if (!_osg_viewer->isRealized()) {
                    _osg_viewer->realize();
                }

                // process next frame
                if (_frame_counter % _render_period == 0) {
                    _osg_viewer->frame();
                }
                _frame_counter++;
            }

            void take_single_shot()
            {
                if (!_buffer_valid)
                    return;
                _shot_done = false;
                if (!_osg_viewer->isRealized()) {
                    _osg_viewer->realize();
                }
                _osg_viewer->frame();
            }

            bool shot_done() const { return _shot_done; }
            void set_shot_done(bool val) { _shot_done = val; }
            void set_render_period(double dt) override
            {
                // we want to display at around 60Hz of simulated time
                _render_period = std::floor(0.015 / dt);
                if (_render_period < 1)
                    _render_period = 1;
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

            void set_enable(bool enable) override
            {
                _enabled = enable;
            }

            void set_filename(std::string filename) { _filename = filename; }
            std::string filename() { return _filename; }
            void set_recording(bool recording) { _recording = recording; }
            bool recording() { return _recording; }
            bool valid() { return _buffer_valid; }

            osg::ref_ptr<osg::Image> image() { return _image; }

        protected:
            Eigen::Vector3d _look_at;
            Eigen::Vector3d _camera_pos;
            Eigen::Vector3d _camera_up;
            osg::ref_ptr<dart::gui::osg::WorldNode> _osg_world_node;
            osg::ref_ptr<dart::gui::osg::Viewer> _osg_viewer;
            ::osg::ref_ptr<::osg::GraphicsContext> _pbuffer;
            dart::simulation::WorldPtr _world;
            size_t _render_period, _width, _height, _frame_counter;
            bool _enabled, _buffer_valid;
            bool _recording;
            std::string _filename;

            osg::ref_ptr<osg::Image> _image;
            bool _shot_done;
        };

    } // namespace graphics
} // namespace robot_dart

#endif
