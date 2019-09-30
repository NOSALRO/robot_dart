#include "base_application.hpp"

#include <dart/dynamics/SoftBodyNode.hpp>
#include <dart/dynamics/SoftMeshShape.hpp>

#include <Magnum/GL/DefaultFramebuffer.h>
#include <Magnum/GL/Mesh.h>
#include <Magnum/GL/Renderer.h>
#include <Magnum/GL/Texture.h>
#include <Magnum/Trade/PhongMaterialData.h>

#include <Magnum/GL/AbstractFramebuffer.h>
#include <Magnum/GL/GL.h>
#include <Magnum/GL/PixelFormat.h>
#include <Magnum/ImageView.h>
#include <Magnum/PixelFormat.h>

namespace robot_dart {
    namespace gui {
        namespace magnum {
            // GlobalData
            Corrade::PluginManager::Manager<Magnum::Trade::AbstractImporter>& GlobalData::plugin_manager() { return _plugin_manager; }

            Magnum::Platform::WindowlessGLContext* GlobalData::get_gl_context()
            {
                std::lock_guard<std::mutex> lg(_context_mutex);
                if (_gl_contexts.size() == 0)
                    _create_contexts();

                for (size_t i = 0; i < _max_contexts; i++) {
                    if (!_used[i]) {
                        _used[i] = true;
                        return &_gl_contexts[i];
                    }
                }

                return nullptr;
            }

            void GlobalData::free_gl_context(Magnum::Platform::WindowlessGLContext* context)
            {
                std::lock_guard<std::mutex> lg(_context_mutex);
                for (size_t i = 0; i < _gl_contexts.size(); i++) {
                    if (&_gl_contexts[i] == context) {
                        _used[i] = false;
                        break;
                    }
                }
            }

            std::mutex& GlobalData::mutex() { return _mutex; }

            void GlobalData::set_max_contexts(size_t N)
            {
                std::lock_guard<std::mutex> lg(_context_mutex);
                _max_contexts = N;
                _create_contexts();
            }

            void GlobalData::_create_contexts()
            {
                _used.clear();
                _gl_contexts.clear();
                _gl_contexts.reserve(_max_contexts);
                for (size_t i = 0; i < _max_contexts; i++) {
                    _used.push_back(false);
                    _gl_contexts.push_back(std::move(Magnum::Platform::WindowlessGLContext{{}}));
                }
            }

            // DrawableObject
            DrawableObject::DrawableObject(
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

            DrawableObject& DrawableObject::setMeshes(const std::vector<std::reference_wrapper<Magnum::GL::Mesh>>& meshes)
            {
                _meshes = meshes;
                return *this;
            }

            DrawableObject& DrawableObject::setMaterials(const std::vector<gs::Material>& materials)
            {
                _materials = materials;
                return *this;
            }

            DrawableObject& DrawableObject::setSoftBodies(const std::vector<bool>& softBody)
            {
                _isSoftBody = softBody;
                return *this;
            }

            DrawableObject& DrawableObject::setScalings(const std::vector<Magnum::Vector3>& scalings)
            {
                _scalings = scalings;
                return *this;
            }

            DrawableObject& DrawableObject::setColorShader(std::reference_wrapper<gs::PhongMultiLight> shader)
            {
                _color_shader = shader;
                return *this;
            }

            DrawableObject& DrawableObject::setTextureShader(std::reference_wrapper<gs::PhongMultiLight> shader)
            {
                _texture_shader = shader;
                return *this;
            }

            void DrawableObject::draw(const Magnum::Matrix4& transformationMatrix, Magnum::SceneGraph::Camera3D& camera)
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

            // BaseApplication
            void BaseApplication::init(const dart::simulation::WorldPtr& world, size_t width, size_t height)
            {
                /* Anything not related to GL */
                /* Camera setup */
                _camera.reset(
                    new gs::Camera(_scene, static_cast<int>(width), static_cast<int>(height)));

                /* Create our DARTIntegration object/world */
                GlobalData::instance()->mutex().lock(); /* Need to lock for plugin manager not being thread-safe */
                auto dartObj = new Object3D{&_scene};
                _dartWorld.reset(new Magnum::DartIntegration::World(GlobalData::instance()->plugin_manager(), *dartObj, *world));
                GlobalData::instance()->mutex().unlock();

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

            void BaseApplication::clearLights()
            {
                _lights.clear();
                /* Reset lights in shaders */
                gs::Light light;
                for (int i = 0; i < _color_shader->maxLights(); i++)
                    _color_shader->setLight(i, light);
                for (int i = 0; i < _texture_shader->maxLights(); i++)
                    _texture_shader->setLight(i, light);
            }

            void BaseApplication::addLight(const gs::Light& light)
            {
                _lights.push_back(light);
            }

            gs::Light& BaseApplication::light(size_t i)
            {
                assert(i < _lights.size());
                return _lights[i];
            }

            std::vector<gs::Light>& BaseApplication::lights()
            {
                return _lights;
            }

            size_t BaseApplication::numLights() const
            {
                return _lights.size();
            }

            bool BaseApplication::done() const
            {
                return _done;
            }

            void BaseApplication::lookAt(const Eigen::Vector3d& camera_pos,
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

                // _cameraObject->setTransformation(
                //     Magnum::Matrix4::lookAt(Magnum::Vector3{cx, cy, cz},
                //         Magnum::Vector3{lx, ly, lz},
                //         Magnum::Vector3{ux, uy, uz}));
                _camera->lookAt(Magnum::Vector3{cx, cy, cz},
                    Magnum::Vector3{lx, ly, lz},
                    Magnum::Vector3{ux, uy, uz});
            }

            void BaseApplication::updateLights(const gs::Camera& camera)
            {
                /* Update lights transformations */
                camera.transformLights(_lights);

                /* Set the shader information */
                for (size_t i = 0; i < _lights.size(); i++) {
                    _color_shader->setLight(i, _lights[i]);
                    _texture_shader->setLight(i, _lights[i]);
                }
            }

            void BaseApplication::updateGraphics()
            {
                /* Refresh the graphical models */
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

            void BaseApplication::GLCleanUp()
            {
                /* Clean up GL because of destructor order */
                _color_shader.reset();
                _texture_shader.reset();

                _camera.reset();

                _dartWorld.reset();
                _drawableObjects.clear();
                _dartObjs.clear();
                _lights.clear();
            }

            Corrade::Containers::Optional<Magnum::PixelFormat> BaseApplication::getPixelFormat(Magnum::GL::AbstractFramebuffer& framebuffer)
            {
                /* Get the implementation-specific color read format for given framebuffer */
                /* TO-DO: This is not working as it should if more than one framebuffers are there! */
                const Magnum::GL::PixelFormat format = framebuffer.implementationColorReadFormat();
                const Magnum::GL::PixelType type = framebuffer.implementationColorReadType();
                // clang-format off
                auto genericFormat = [](Magnum::GL::PixelFormat format, Magnum::GL::PixelType type) -> Corrade::Containers::Optional<Magnum::PixelFormat> {
                    #define _c(generic, glFormat, glType, glTextureFormat)                                        \
                        if (format == Magnum::GL::PixelFormat::glFormat && type == Magnum::GL::PixelType::glType) \
                            return Magnum::PixelFormat::generic;
                    #define _n(generic, glFormat, glType)                                                         \
                        if (format == Magnum::GL::PixelFormat::glFormat && type == Magnum::GL::PixelType::glType) \
                            return Magnum::PixelFormat::generic;
                    #define _s(generic) return {};
                    #include "pixel_format_mapping.hpp"
                    #undef _c
                    #undef _n
                    #undef _s
                    return {};
                }(format, type);
                // clang-format on

                return genericFormat;
            }
        } // namespace magnum
    } // namespace gui
} // namespace robot_dart