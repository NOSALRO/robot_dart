#ifndef ROBOT_DART_GRAPHICS_CAMERAOSR_HPP
#define ROBOT_DART_GRAPHICS_CAMERAOSR_HPP

#include <dart/gui/osg/osg.hpp>
#include <osgDB/WriteFile>
#include <robot_dart/graphics/base_graphics.hpp>

namespace robot_dart {
    namespace graphics {
        template <typename Viewer_t>
        class GetScreen : public ::osg::Camera::DrawCallback {
        public:
            GetScreen(Viewer_t* viewer, ::osg::ref_ptr<::osg::Camera> cam)
                : _viewer(viewer),
                  mCamera(cam) {}

            virtual void operator()(::osg::RenderInfo& renderInfo) const
            {
                ::osg::Camera::DrawCallback::operator()(renderInfo);

                int x, y;
                unsigned int width, height;
                ::osg::ref_ptr<::osg::Viewport> vp = mCamera->getViewport();
                x = vp->x();
                y = vp->y();
                width = vp->width();
                height = vp->height();

                _viewer->get_image()->readPixels(x, y, width, height, GL_RGB, GL_UNSIGNED_BYTE);

                if (_viewer->recording()) {
                    if (!_viewer->filename().empty()) {
                        if (!::osgDB::writeImageFile(*_viewer->get_image(), _viewer->filename()))
                            dtwarn << "[SaveScreen::capture] Unable to save image to file named: "
                                   << _viewer->filename() << "\n";
                    }
                }
            }

        protected:
            Viewer_t* _viewer;
            ::osg::ref_ptr<::osg::Camera> mCamera;
        };

        // Camera-OffScreen-Rendering
        class CameraOSR : public BaseGraphics {

        public:
            CameraOSR(dart::simulation::WorldPtr world, unsigned int width = 640, unsigned int height = 480, bool shadowed = true) : _world(world), _width(width), _height(height), _enabled(true), _recording(false), _filename("camera_record.png"), _image(new osg::Image)

            {
                _osg_viewer = new dart::gui::osg::Viewer;
                //graphics context
                ::osg::ref_ptr<::osg::GraphicsContext::Traits> traits = new ::osg::GraphicsContext::Traits;
                traits->readDISPLAY();
                //if (traits->displayNum<0) traits->displayNum = 0;

                traits->x = 0;
                traits->y = 0;
                traits->width = width;
                traits->height = height;
                traits->red = 8;
                traits->green = 8;
                traits->blue = 8;
                traits->alpha = 0;
                //traits->depth = 24;
                traits->windowDecoration = false;
                traits->pbuffer = true;
                traits->doubleBuffer = true;
                traits->sharedContext = 0;
                traits->readDISPLAY();
                traits->setUndefinedScreenDetailsToDefaultScreen();

                ::osg::ref_ptr<::osg::GraphicsContext> pbuffer = ::osg::GraphicsContext::createGraphicsContext(traits.get());

                if (pbuffer.valid()) {
                    ::osg::ref_ptr<osg::Camera> camera = _osg_viewer->getCamera();
                    camera->setGraphicsContext(pbuffer.get());
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
                    _osg_viewer->setThreadingModel(osgViewer::ViewerBase::ThreadingModel::SingleThreaded);
                }
                else {
                    std::cout << "Error configuring pbuffer\n";
                    exit(0);
                }
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
                    _osg_viewer->realize();
                }

                // process next frame
                if (i % _render_period == 0) {
                    _osg_viewer->frame();
                }
                i++;
            }

            void take_single_shot()
            {
                if (!_osg_viewer->isRealized()) {
                    _osg_viewer->realize();
                }
                _osg_viewer->frame();
            }

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

            osg::ref_ptr<osg::Image> get_image() { return _image; }

        protected:
            Eigen::Vector3d _look_at;
            Eigen::Vector3d _camera_pos;
            Eigen::Vector3d _camera_up;
            osg::ref_ptr<dart::gui::osg::WorldNode> _osg_world_node;
            osg::ref_ptr<dart::gui::osg::Viewer> _osg_viewer;
            dart::simulation::WorldPtr _world;
            unsigned int _render_period, _width, _height;
            bool _enabled;
            bool _recording;
            std::string _filename;

            osg::ref_ptr<osg::Image> _image;
        };

    } // namespace graphics
} // namespace robot_dart

#endif
