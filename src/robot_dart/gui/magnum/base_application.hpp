#ifndef ROBOT_DART_GUI_MAGNUM_BASE_APPLICATION_HPP
#define ROBOT_DART_GUI_MAGNUM_BASE_APPLICATION_HPP

#include <robot_dart/gui/magnum/gs/phong_multi_light.hpp>

#include <dart/dynamics/SoftBodyNode.hpp>
#include <dart/dynamics/SoftMeshShape.hpp>

#include <Corrade/Utility/Directory.h>

#include <Magnum/GL/Buffer.h>
#include <Magnum/GL/DefaultFramebuffer.h>
#include <Magnum/GL/Mesh.h>
#include <Magnum/GL/OpenGLTester.h>
#include <Magnum/GL/Texture.h>
#include <Magnum/Platform/GLContext.h>
#include <Magnum/ResourceManager.h>
#include <Magnum/SceneGraph/Camera.h>
#include <Magnum/SceneGraph/Drawable.h>
#include <Magnum/SceneGraph/MatrixTransformation3D.h>
#include <Magnum/SceneGraph/Object.hpp>
#include <Magnum/SceneGraph/SceneGraph.h>
#include <Magnum/Shaders/Phong.h>
#include <Magnum/Trade/PhongMaterialData.h>

#include <Magnum/DartIntegration/ConvertShapeNode.h>
#include <Magnum/DartIntegration/World.h>

namespace robot_dart {
    namespace gui {
        namespace magnum {
            // using ViewerResourceManager = Magnum::ResourceManager<Magnum::GL::Buffer, Magnum::GL::Mesh, Magnum::Shaders::Phong, gs::PhongMultiLight>;
            using Object3D = Magnum::SceneGraph::Object<Magnum::SceneGraph::MatrixTransformation3D>;
            using Scene3D = Magnum::SceneGraph::Scene<Magnum::SceneGraph::MatrixTransformation3D>;

            struct GlobalData {
            public:
                static GlobalData* instance()
                {
                    static GlobalData* gdata = new GlobalData();

                    return gdata;
                }

                GlobalData(const GlobalData&) = delete;
                void operator=(const GlobalData&) = delete;

                Corrade::PluginManager::Manager<Magnum::Trade::AbstractImporter>& plugin_manager() { return _plugin_manager; }
                Magnum::Platform::WindowlessGLContext& gl_context() { return _gl_context; }

            private:
                GlobalData() {}

                ~GlobalData()
                {
                    auto inst = instance();
                    delete inst;
                }

                // ViewerResourceManager _resourceManager;
                Corrade::PluginManager::Manager<Magnum::Trade::AbstractImporter> _plugin_manager;
                Magnum::Platform::WindowlessGLContext _gl_context{{}};
            };

            class DrawableObject : public Object3D, Magnum::SceneGraph::Drawable3D {
            public:
                explicit DrawableObject(
                    const std::vector<std::reference_wrapper<Magnum::GL::Mesh>>& meshes,
                    const std::vector<gs::Material>& materials,
                    std::reference_wrapper<gs::PhongMultiLight> color,
                    std::reference_wrapper<gs::PhongMultiLight> texture,
                    Object3D* parent,
                    Magnum::SceneGraph::DrawableGroup3D* group)
                    : Object3D{parent},
                      Magnum::SceneGraph::Drawable3D{*this, group},
                      _meshes{meshes},
                      _color_shader{color},
                      _texture_shader{texture},
                      _materials(materials)
                {
                    assert(_materials.size() >= meshes.size());
                    _isSoftBody.resize(_meshes.size(), false);
                }

                DrawableObject& setMeshes(const std::vector<std::reference_wrapper<Magnum::GL::Mesh>>& meshes)
                {
                    _meshes = meshes;
                    return *this;
                }

                DrawableObject& setMaterials(const std::vector<gs::Material>& materials)
                {
                    _materials = materials;
                    return *this;
                }

                DrawableObject& setSoftBodies(const std::vector<bool>& softBody)
                {
                    _isSoftBody = softBody;
                    return *this;
                }

                DrawableObject& setScalings(const std::vector<Magnum::Vector3>& scalings)
                {
                    _scalings = scalings;
                    return *this;
                }

                DrawableObject& setColorShader(std::reference_wrapper<gs::PhongMultiLight> shader)
                {
                    _color_shader = shader;
                    return *this;
                }

                DrawableObject& setTextureShader(std::reference_wrapper<gs::PhongMultiLight> shader)
                {
                    _texture_shader = shader;
                    return *this;
                }

            private:
                void draw(const Magnum::Matrix4& transformationMatrix, Magnum::SceneGraph::Camera3D& camera) override
                {
                    for (size_t i = 0; i < _meshes.size(); i++) {
                        Magnum::GL::Mesh& mesh = _meshes[i];
                        Magnum::Matrix4 scalingMatrix = Magnum::Matrix4::scaling(_scalings[i]);
                        bool isColor = !_materials[i].hasDiffuseTexture();
                        if (isColor) {
                            _color_shader.get().setMaterial(_materials[i]).setTransformationMatrix(transformationMatrix * scalingMatrix).setNormalMatrix((transformationMatrix * scalingMatrix).rotation()).setProjectionMatrix(camera.projectionMatrix());
                        }
                        else {
                            _texture_shader.get().setMaterial(_materials[i]).setTransformationMatrix(transformationMatrix * scalingMatrix).setNormalMatrix((transformationMatrix * scalingMatrix).rotation()).setProjectionMatrix(camera.projectionMatrix());
                        }

                        if (_isSoftBody[i])
                            Magnum::GL::Renderer::disable(Magnum::GL::Renderer::Feature::FaceCulling);
                        if (isColor)
                            mesh.draw(_color_shader);
                        else
                            mesh.draw(_texture_shader);
                        if (_isSoftBody[i])
                            Magnum::GL::Renderer::enable(Magnum::GL::Renderer::Feature::FaceCulling);
                    }
                }

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
                ~BaseApplication() {}

                void init(const dart::simulation::WorldPtr& world)
                {
                    /* Anything not related to GL */
                    /* Camera setup */
                    (_cameraRig = new Object3D(&_scene));
                    (_cameraObject = new Object3D(_cameraRig));
                    (_camera = new Magnum::SceneGraph::Camera3D(*_cameraObject))
                        ->setAspectRatioPolicy(Magnum::SceneGraph::AspectRatioPolicy::Extend)
                        .setProjectionMatrix(Magnum::Matrix4::perspectiveProjection(Magnum::Deg(35.0f), 1.0f, 0.001f, 100.0f))
                        .setViewport(Magnum::GL::defaultFramebuffer.viewport().size());
                    /* DART has +Z-axis as up direction*/
                    /* Default look at */
                    _cameraObject->setTransformation(Magnum::Matrix4::lookAt(Magnum::Vector3{0.f, 2.f, 1.5f}, Magnum::Vector3{0.f, 0.f, 0.5f}, Magnum::Vector3{0.f, 0.f, 1.f}));

                    /* Create our DARTIntegration object/world */
                    auto dartObj = new Object3D{&_scene};
                    _dartWorld.reset(new Magnum::DartIntegration::World(GlobalData::instance()->plugin_manager(), *dartObj, *world));

                    /* Phong shaders */
                    _color_shader.reset(new gs::PhongMultiLight{{}, 10});
                    _texture_shader.reset(new gs::PhongMultiLight{{gs::PhongMultiLight::Flag::DiffuseTexture}, 10});

                    /* Add default lights (2 directional lights) */
                    gs::Material mat;
                    mat.diffuseColor() = {1.f, 1.f, 1.f, 1.f};
                    mat.specularColor() = {1.f, 1.f, 1.f, 1.f};
                    // gs::Light light = gs::createPointLight({0.f, 0.f, 2.f}, mat, 2.f, {0.f, 0.f, 1.f});
                    // gs::Light light = gs::createSpotLight(
                    //     {0.f, 0.f, 3.f}, mat, {0.f, 0.f, -1.f}, 1.f, Magnum::Math::Constants<Magnum::Float>::piHalf() / 5.f, 2.f, {0.f, 0.f, 1.f});
                    Magnum::Vector3 dir = {-1.f, -1.f, -1.f};
                    gs::Light light = gs::createDirectionalLight(dir, mat);
                    _lights.push_back(light);
                    dir = {1.f, 1.f, -1.f};
                    light = gs::createDirectionalLight(dir, mat);
                    _lights.push_back(light);
                }

                void clearLights()
                {
                    _lights.clear();
                    /* Reset lights in shaders */
                    gs::Light light;
                    for (int i = 0; i < _color_shader->maxLights(); i++)
                        _color_shader->setLight(i, light);
                    for (int i = 0; i < _texture_shader->maxLights(); i++)
                        _texture_shader->setLight(i, light);
                }

                void addLight(const gs::Light& light)
                {
                    _lights.push_back(light);
                }

                gs::Light& light(size_t i)
                {
                    assert(i < _lights.size());
                    return _lights[i];
                }

                bool done() const
                {
                    return _done;
                }

                void lookAt(const Eigen::Vector3d& camera_pos,
                    const Eigen::Vector3d& look_at,
                    const Eigen::Vector3d& up)
                {
                    float cx = static_cast<float>(camera_pos[0]);
                    float cy = static_cast<float>(camera_pos[1]);
                    float cz = static_cast<float>(camera_pos[2]);

                    float lx = static_cast<float>(look_at[0]);
                    float ly = static_cast<float>(look_at[1]);
                    float lz = static_cast<float>(look_at[2]);

                    float ux = static_cast<float>(up[0]);
                    float uy = static_cast<float>(up[1]);
                    float uz = static_cast<float>(up[2]);

                    _cameraObject->setTransformation(
                        Magnum::Matrix4::lookAt(Magnum::Vector3{cx, cy, cz},
                            Magnum::Vector3{lx, ly, lz},
                            Magnum::Vector3{ux, uy, uz}));
                }

                virtual void render() {}

            protected:
                /* Magnum */
                Scene3D _scene;
                Magnum::SceneGraph::DrawableGroup3D _drawables;
                Magnum::SceneGraph::Camera3D* _camera;
                std::unique_ptr<magnum::gs::PhongMultiLight> _color_shader, _texture_shader;

                Object3D *_cameraRig, *_cameraObject;

                bool _done = false;

                /* DART */
                std::unique_ptr<Magnum::DartIntegration::World> _dartWorld;
                std::unordered_map<Magnum::DartIntegration::Object*, DrawableObject*> _drawableObjects;
                std::vector<Object3D*> _dartObjs;
                std::vector<gs::Light> _lights;

                void updateGraphics()
                {
                    /* We refresh the graphical models at 60Hz */
                    _dartWorld->refresh();

                    /* For each update object */
                    for (Magnum::DartIntegration::Object& object : _dartWorld->updatedShapeObjects()) {
                        /* Get material information */
                        std::vector<gs::Material> materials;
                        std::vector<std::reference_wrapper<Magnum::GL::Mesh>> meshes;
                        std::vector<bool> isSoftBody;
                        // std::vector<Containers::Optional<GL::Texture2D>> textures;
                        std::vector<Magnum::Vector3> scalings;

                        for (size_t i = 0; i < object.drawData().meshes.size(); i++) {
                            bool isColor = true;
                            gs::Material mat;

                            if (object.drawData().materials[i].flags() & Magnum::Trade::PhongMaterialData::Flag::DiffuseTexture) {
                                mat.setDiffuseTexture(&(*object.drawData().textures[object.drawData().materials[i].diffuseTexture()]));
                                isColor = false;
                            }
                            mat.ambientColor() = object.drawData().materials[i].ambientColor();
                            if (isColor)
                                mat.diffuseColor() = object.drawData().materials[i].diffuseColor();
                            mat.specularColor() = object.drawData().materials[i].specularColor();
                            mat.shininess() = object.drawData().materials[i].shininess();

                            scalings.push_back(object.drawData().scaling);

                            /* Get the modified mesh */
                            Magnum::GL::Mesh& mesh = object.drawData().meshes[i];

                            meshes.push_back(mesh);
                            materials.push_back(mat);
                            if (object.shapeNode()->getShape()->getType() == dart::dynamics::SoftMeshShape::getStaticType())
                                isSoftBody.push_back(true);
                            else
                                isSoftBody.push_back(false);
                        }

                        /* Check if we already have it */
                        auto it = _drawableObjects.insert(std::make_pair(&object, nullptr));
                        if (it.second) {
                            /* If not, create a new object and add it to our drawables list */
                            auto drawableObject = new DrawableObject(meshes, materials, *_color_shader, *_texture_shader, static_cast<Object3D*>(&(object.object())), &_drawables);
                            drawableObject->setSoftBodies(isSoftBody);
                            drawableObject->setScalings(scalings);
                            it.first->second = drawableObject;
                        }
                        else {
                            /* Otherwise, update the mesh and the material data */
                            it.first->second->setMeshes(meshes).setMaterials(materials).setSoftBodies(isSoftBody).setScalings(scalings).setColorShader(*_color_shader).setTextureShader(*_texture_shader);
                        }
                    }

                    _dartWorld->clearUpdatedShapeObjects();
                }

                void updateLights()
                {
                    /* Update lights transformations */
                    for (size_t i = 0; i < _lights.size(); i++) {
                        Magnum::Vector4 old_pos = _lights[i].position();
                        Magnum::Vector3 pos;
                        /* Directional lights need only rotational transformation */
                        if (_lights[i].position().w() == 0.f)
                            pos = _camera->cameraMatrix().transformVector(old_pos.xyz());
                        /* Other light types, need full transformation */
                        else
                            pos = _camera->cameraMatrix().transformPoint(old_pos.xyz());
                        _lights[i].setTransformedPosition(Magnum::Vector4{pos, old_pos.w()});
                        /* Transform spotlight direction */
                        _lights[i].setTransformedSpotDirection(_camera->cameraMatrix().transformVector(_lights[i].spotDirection()));

                        _color_shader->setLight(i, _lights[i]);
                        _texture_shader->setLight(i, _lights[i]);
                    }
                }
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