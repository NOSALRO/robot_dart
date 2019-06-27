#ifndef ROBOT_DART_GRAPHICS_PBUFFER_MANAGER_HPP
#define ROBOT_DART_GRAPHICS_PBUFFER_MANAGER_HPP

#include <memory>
#include <mutex>

#include <dart/gui/osg/osg.hpp>

namespace robot_dart {
    namespace graphics {

        class PbufferManager;

        namespace global {
            std::unique_ptr<PbufferManager> pbufferManager;
        }

        // PBuffer manager: used to create and allocate pbuffer to different thread to avoid creating multiple buffers at the same time
        class PbufferManager {

        public:
            static void createPbufferManager(size_t nb_threads = 8)
            {
                ::osg::setNotifyLevel(::osg::WARN); // used to remove INFO messages that are coming from realising a context already realized
                global::pbufferManager = std::unique_ptr<PbufferManager>(new PbufferManager(nb_threads));
            }

            PbufferManager(size_t nb_threads = 8) : _nb_threads(nb_threads)
            {
                //graphics context
                ::osg::ref_ptr<::osg::GraphicsContext::Traits> traits = new ::osg::GraphicsContext::Traits;
                traits->readDISPLAY();

                traits->x = 0;
                traits->y = 0;
                traits->width = 512;
                traits->height = 512;
                traits->red = 8;
                traits->green = 8;
                traits->blue = 8;
                traits->alpha = 0;
                //traits->depth = 24;
                traits->windowDecoration = false;
                traits->pbuffer = true;
                traits->doubleBuffer = true;
                traits->sharedContext = 0;
                traits->setUndefinedScreenDetailsToDefaultScreen();
                for (size_t i = 0; i < _nb_threads; i++)
                    _pbuffers[::osg::GraphicsContext::createGraphicsContext(traits.get())] = false;
            }

            ::osg::ref_ptr<::osg::GraphicsContext> get_pbuffer()
            {
                auto it = _pbuffers.begin();
                std::lock_guard<std::mutex> lock(g_i_mutex);
                while (it != _pbuffers.end() && it->second == true)
                    it++;
                if (it == _pbuffers.end()) {
                    std::cout << "All the pbuffers are already used... Please create more" << std::endl;
                    assert(0);
                }
                else {
                    it->second = true;
                    //just checking
                    if (_pbuffers[it->first] != true) {
                        std::cout << "State not changed in the pbuffer map" << std::endl;
                        assert(0);
                    }
                    return it->first;
                }
            }

            void release_pbuffer(::osg::ref_ptr<::osg::GraphicsContext> pbuffer)
            {
                std::lock_guard<std::mutex> lock(g_i_mutex);
                auto it = _pbuffers.find(pbuffer);
                if (it == _pbuffers.end()) {
                    std::cout << "Pbuffer not found in the pbuffer map" << std::endl;
                    assert(0);
                }
                else
                    it->second = false;
            }

            ~PbufferManager()
            {
            }

        protected:
            size_t _nb_threads;
            // if the value is true, this means that the pbuffer has been given to a thread.
            std::map<::osg::ref_ptr<::osg::GraphicsContext>, bool> _pbuffers;

            std::mutex g_i_mutex;
        };

    } // namespace graphics
} // namespace robot_dart

#endif
