#ifndef ROBOT_DART_GUI_MAGNUM_BASE_APPLICATION_HPP
#define ROBOT_DART_GUI_MAGNUM_BASE_APPLICATION_HPP

#include <mutex>
#include <unordered_map>

#include <robot_dart/gui/magnum/gs/camera.hpp>
#include <robot_dart/gui/magnum/gs/phong_multi_light.hpp>
#include <robot_dart/gui/magnum/types.hpp>

#include <dart/simulation/World.hpp>

#include <Magnum/Platform/GLContext.h>
#include <Magnum/Platform/WindowlessGlxApplication.h>
#include <Magnum/SceneGraph/Drawable.h>

#include <Magnum/DartIntegration/World.h>

#define get_glx_context(name)                                                     \
    /* Create/Get GLContext */                                                    \
    Magnum::Platform::WindowlessGLContext* name = nullptr;                        \
    while (name == nullptr)                                                       \
        name = robot_dart::gui::magnum::GlobalData::instance()->get_gl_context(); \
    while (!name->makeCurrent()) {}                                               \
                                                                                  \
    Magnum::Platform::GLContext name##_magnum_context;

#define release_glx_context(name) robot_dart::gui::magnum::GlobalData::instance()->free_gl_context(name);

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

                Corrade::PluginManager::Manager<Magnum::Trade::AbstractImporter>& plugin_manager();

                Magnum::Platform::WindowlessGLContext* get_gl_context();
                void free_gl_context(Magnum::Platform::WindowlessGLContext* context);

                std::mutex& mutex();

                /* You should call this before starting to draw or after finished */
                void set_max_contexts(size_t N);

            private:
                GlobalData() {}
                ~GlobalData() {}

                void _create_contexts();

                // ViewerResourceManager _resourceManager;
                Corrade::PluginManager::Manager<Magnum::Trade::AbstractImporter> _plugin_manager;
                std::vector<Magnum::Platform::WindowlessGLContext> _gl_contexts;
                std::vector<bool> _used;
                std::mutex _mutex, _context_mutex;
                size_t _max_contexts = 4;
            };

            class DrawableObject : public Object3D, Magnum::SceneGraph::Drawable3D {
            public:
                explicit DrawableObject(
                    const std::vector<std::reference_wrapper<Magnum::GL::Mesh>>& meshes,
                    const std::vector<gs::Material>& materials,
                    std::reference_wrapper<gs::PhongMultiLight> color,
                    std::reference_wrapper<gs::PhongMultiLight> texture,
                    Object3D* parent,
                    Magnum::SceneGraph::DrawableGroup3D* group);

                DrawableObject& setMeshes(const std::vector<std::reference_wrapper<Magnum::GL::Mesh>>& meshes);
                DrawableObject& setMaterials(const std::vector<gs::Material>& materials);
                DrawableObject& setSoftBodies(const std::vector<bool>& softBody);
                DrawableObject& setScalings(const std::vector<Magnum::Vector3>& scalings);

                DrawableObject& setColorShader(std::reference_wrapper<gs::PhongMultiLight> shader);
                DrawableObject& setTextureShader(std::reference_wrapper<gs::PhongMultiLight> shader);

            private:
                void draw(const Magnum::Matrix4& transformationMatrix, Magnum::SceneGraph::Camera3D& camera) override;

                std::vector<std::reference_wrapper<Magnum::GL::Mesh>> _meshes;
                std::reference_wrapper<gs::PhongMultiLight> _color_shader;
                std::reference_wrapper<gs::PhongMultiLight> _texture_shader;
                std::vector<gs::Material> _materials;
                std::vector<Magnum::Vector3> _scalings;
                std::vector<bool> _isSoftBody;
            };

            class BaseApplication {
            public:
                BaseApplication() {}
                virtual ~BaseApplication() {}

                void init(const dart::simulation::WorldPtr& world, size_t width, size_t height);

                void clearLights();
                void addLight(const gs::Light& light);
                gs::Light& light(size_t i);
                std::vector<gs::Light>& lights();
                size_t numLights() const;

                Magnum::SceneGraph::DrawableGroup3D& drawables() { return _drawables; }
                Scene3D& scene() { return _scene; }

                bool done() const;

                void lookAt(const Eigen::Vector3d& camera_pos,
                    const Eigen::Vector3d& look_at,
                    const Eigen::Vector3d& up);

                virtual void render() {}

                void updateLights(const gs::Camera& camera);
                void updateGraphics();

                void record(bool recording) { _camera->record(recording); }
                bool isRecording() { return _camera->isRecording(); }

                Corrade::Containers::Optional<Magnum::Image2D>& image() { return _camera->image(); }

            protected:
                /* Magnum */
                Scene3D _scene;
                Magnum::SceneGraph::DrawableGroup3D _drawables;
                std::unique_ptr<gs::PhongMultiLight> _color_shader, _texture_shader;

                std::unique_ptr<gs::Camera> _camera;

                bool _done = false;

                /* DART */
                std::unique_ptr<Magnum::DartIntegration::World> _dartWorld;
                std::unordered_map<Magnum::DartIntegration::Object*, DrawableObject*> _drawableObjects;
                std::vector<Object3D*> _dartObjs;
                std::vector<gs::Light> _lights;

                void GLCleanUp();
            };

            template <typename T>
            inline BaseApplication* make_application(const dart::simulation::WorldPtr& world, size_t width, size_t height, const std::string& title = "DART")
            {
                int argc = 0;
                char** argv = NULL;

                return new T(argc, argv, world, width, height, title);
            }
        } // namespace magnum
    } // namespace gui
} // namespace robot_dart

#endif