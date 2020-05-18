#ifndef ROBOT_DART_GUI_MAGNUM_BASE_APPLICATION_HPP
#define ROBOT_DART_GUI_MAGNUM_BASE_APPLICATION_HPP

#include <mutex>
#include <unistd.h>
#include <unordered_map>

#include <robot_dart/gui/helper.hpp>
#include <robot_dart/gui/magnum/drawables.hpp>
#include <robot_dart/gui/magnum/gs/camera.hpp>
#include <robot_dart/gui/magnum/gs/cube_map.hpp>
#include <robot_dart/gui/magnum/gs/cube_map_color.hpp>
#include <robot_dart/gui/magnum/gs/phong_multi_light.hpp>
#include <robot_dart/gui/magnum/gs/shadow_map.hpp>
#include <robot_dart/gui/magnum/gs/shadow_map_color.hpp>
#include <robot_dart/gui/magnum/types.hpp>

#include <dart/simulation/World.hpp>

#include <Magnum/GL/CubeMapTextureArray.h>
#include <Magnum/GL/Framebuffer.h>
#include <Magnum/GL/TextureArray.h>
#include <Magnum/Platform/GLContext.h>
#ifndef MAGNUM_MAC_OSX
#include <Magnum/Platform/WindowlessGlxApplication.h>
#else
#include <Magnum/Platform/WindowlessCglApplication.h>
#endif

#include <Magnum/DartIntegration/World.h>

#define get_gl_context_with_sleep(name, ms_sleep)                             \
    /* Create/Get GLContext */                                                \
    Corrade::Utility::Debug name##_magnum_silence_output{nullptr};            \
    Magnum::Platform::WindowlessGLContext* name = nullptr;                    \
    while (name == nullptr) {                                                 \
        name = robot_dart::gui::magnum::GlobalData::instance()->gl_context(); \
        /* Sleep for some ms */                                               \
        usleep(ms_sleep * 1000);                                              \
    }                                                                         \
    while (!name->makeCurrent()) {                                            \
        /* Sleep for some ms */                                               \
        usleep(ms_sleep * 1000);                                              \
    }                                                                         \
                                                                              \
    Magnum::Platform::GLContext name##_magnum_context;

#define get_gl_context(name) get_gl_context_with_sleep(name, 0)

#define release_gl_context(name) robot_dart::gui::magnum::GlobalData::instance()->free_gl_context(name);

namespace robot_dart {
    namespace gui {
        namespace magnum {
            struct GlobalData {
            public:
                static GlobalData* instance()
                {
                    static GlobalData gdata;
                    return &gdata;
                }

                GlobalData(const GlobalData&) = delete;
                void operator=(const GlobalData&) = delete;

                Magnum::Platform::WindowlessGLContext* gl_context();
                void free_gl_context(Magnum::Platform::WindowlessGLContext* context);

                /* You should call this before starting to draw or after finished */
                void set_max_contexts(size_t N);

            private:
                GlobalData() = default;
                ~GlobalData() = default;

                void _create_contexts();

                std::vector<Magnum::Platform::WindowlessGLContext> _gl_contexts;
                std::vector<bool> _used;
                std::mutex _context_mutex;
                size_t _max_contexts = 4;
            };

            struct GraphicsConfiguration {
                // General
                size_t width = 640;
                size_t height = 480;
                std::string title = "DART";

                // Shadows
                bool shadowed = true;
                bool transparent_shadows = true;
                size_t shadow_map_size = 512;

                // Lights
                size_t max_lights = 3;
            };

            class BaseApplication {
            public:
                BaseApplication(const GraphicsConfiguration& configuration = GraphicsConfiguration());
                virtual ~BaseApplication() {}

                void init(const dart::simulation::WorldPtr& world, size_t width, size_t height);

                void clear_lights();
                void add_light(const gs::Light& light);
                gs::Light& light(size_t i);
                std::vector<gs::Light>& lights();
                size_t num_lights() const;

                Magnum::SceneGraph::DrawableGroup3D& drawables() { return _drawables; }
                Scene3D& scene() { return _scene; }
                gs::Camera& camera() { return *_camera; }

                bool done() const;

                void look_at(const Eigen::Vector3d& camera_pos,
                    const Eigen::Vector3d& look_at,
                    const Eigen::Vector3d& up);

                virtual void render() {}

                void update_lights(const gs::Camera& camera);
                void update_graphics();
                void render_shadows();
                bool attach_camera(gs::Camera& camera, const std::string& name);

                // this naming is ambiguous : what do we record here? especially when we can record videos
                // maybe record_image & record_video ?
                void record(bool recording, bool depthRecording = false) { _camera->record(recording, depthRecording); }
                bool recording() { return _camera->recording(); }
                bool recording_depth() { return _camera->recording_depth(); }
                // video (FPS is mandatory here, see the Graphics class for automatic computation)
                void record_video(const std::string& video_fname, int fps) { _camera->record_video(video_fname, fps); }

                bool shadowed() const { return _shadowed; }
                bool transparent_shadows() const { return _transparent_shadows; }
                void enable_shadows(bool enable = true, bool drawTransparentShadows = false);

                Corrade::Containers::Optional<Magnum::Image2D>& image() { return _camera->image(); }

                // This is for visualization purposes
                GrayscaleImage depth_image();

                // Image filled with depth buffer values
                GrayscaleImage raw_depth_image();

            protected:
                /* Magnum */
                Scene3D _scene;
                Magnum::SceneGraph::DrawableGroup3D _drawables, _shadowed_drawables, _shadowed_color_drawables, _cubemap_drawables, _cubemap_color_drawables;
                std::unique_ptr<gs::PhongMultiLight> _color_shader, _texture_shader;

                std::unique_ptr<gs::Camera> _camera;

                bool _done = false;

                /* DART */
                std::unique_ptr<Magnum::DartIntegration::World> _dart_world;
                std::unordered_map<Magnum::DartIntegration::Object*, ObjectStruct*> _drawable_objects;
                std::vector<Object3D*> _dart_objects;
                std::vector<gs::Light> _lights;

                /* Shadows */
                bool _shadowed = true, _transparent_shadows = false;
                int _transparentSize = 0;
                std::unique_ptr<gs::ShadowMap> _shadow_shader, _shadow_texture_shader;
                std::unique_ptr<gs::ShadowMapColor> _shadow_color_shader, _shadow_texture_color_shader;
                std::unique_ptr<gs::CubeMap> _cubemap_shader, _cubemap_texture_shader;
                std::unique_ptr<gs::CubeMapColor> _cubemap_color_shader, _cubemap_texture_color_shader;
                std::vector<ShadowData> _shadow_data;
                std::unique_ptr<Magnum::GL::Texture2DArray> _shadow_texture, _shadow_color_texture;
                std::unique_ptr<Magnum::GL::CubeMapTextureArray> _shadow_cube_map, _shadow_color_cube_map;
                int _max_lights = 5;
                int _shadow_map_size = 512;
                std::unique_ptr<Camera3D> _shadow_camera;
                Object3D* _shadow_camera_object;
                Corrade::PluginManager::Manager<Magnum::Trade::AbstractImporter> _importer_manager;

                void _gl_clean_up();
                void _prepare_shadows();
            };

            template <typename T>
            inline BaseApplication* make_application(const dart::simulation::WorldPtr& world, const GraphicsConfiguration& configuration = GraphicsConfiguration())
            {
                int argc = 0;
                char** argv = NULL;

                return new T(argc, argv, world, configuration);
                // configuration.width, configuration.height, configuration.shadowed, configuration.transparent_shadows, configuration.max_lights, configuration.shadow_map_size);
            }
        } // namespace magnum
    } // namespace gui
} // namespace robot_dart

#endif