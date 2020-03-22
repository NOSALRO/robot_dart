#include "base_application.hpp"

#include <robot_dart/utils.hpp>

#include <dart/dynamics/SoftBodyNode.hpp>
#include <dart/dynamics/SoftMeshShape.hpp>

#include <Corrade/Containers/StridedArrayView.h>

#include <Magnum/GL/CubeMapTexture.h>
#include <Magnum/GL/DefaultFramebuffer.h>
#include <Magnum/GL/Mesh.h>
#include <Magnum/GL/Renderer.h>
#include <Magnum/GL/Texture.h>
#include <Magnum/GL/TextureFormat.h>
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
            Magnum::Platform::WindowlessGLContext* GlobalData::gl_context()
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
                    _gl_contexts.emplace_back(Magnum::Platform::WindowlessGLContext{{}});
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

            DrawableObject& DrawableObject::setTransparent(bool transparent)
            {
                _isTransparent = transparent;
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
                    if (_isSoftBody[i])
                        Magnum::GL::Renderer::disable(Magnum::GL::Renderer::Feature::FaceCulling);
                    if (isColor) {
                        _color_shader.get()
                            .setMaterial(_materials[i])
                            .setTransformationMatrix(absoluteTransformationMatrix() * scalingMatrix)
                            .setNormalMatrix((transformationMatrix * scalingMatrix).rotationScaling())
                            .setCameraMatrix(camera.cameraMatrix())
                            .setProjectionMatrix(camera.projectionMatrix())
                            .draw(mesh);
                    }
                    else {
                        _texture_shader.get()
                            .setMaterial(_materials[i])
                            .setTransformationMatrix(absoluteTransformationMatrix() * scalingMatrix)
                            .setNormalMatrix((transformationMatrix * scalingMatrix).rotationScaling())
                            .setCameraMatrix(camera.cameraMatrix())
                            .setProjectionMatrix(camera.projectionMatrix())
                            .draw(mesh);
                    }

                    if (_isSoftBody[i])
                        Magnum::GL::Renderer::enable(Magnum::GL::Renderer::Feature::FaceCulling);
                }
            }

            // ShadowedObject
            ShadowedObject::ShadowedObject(
                const std::vector<std::reference_wrapper<Magnum::GL::Mesh>>& meshes,
                std::reference_wrapper<gs::ShadowMap> shader,
                std::reference_wrapper<gs::ShadowMap> texture_shader,
                Object3D* parent,
                Magnum::SceneGraph::DrawableGroup3D* group)
                : Object3D{parent},
                  Magnum::SceneGraph::Drawable3D{*this, group},
                  _meshes{meshes},
                  _shader{shader},
                  _texture_shader(texture_shader) {}

            ShadowedObject& ShadowedObject::setMeshes(const std::vector<std::reference_wrapper<Magnum::GL::Mesh>>& meshes)
            {
                _meshes = meshes;
                return *this;
            }

            ShadowedObject& ShadowedObject::setMaterials(const std::vector<gs::Material>& materials)
            {
                _materials = materials;
                return *this;
            }

            ShadowedObject& ShadowedObject::setScalings(const std::vector<Magnum::Vector3>& scalings)
            {
                _scalings = scalings;
                return *this;
            }

            void ShadowedObject::draw(const Magnum::Matrix4& transformationMatrix, Magnum::SceneGraph::Camera3D& camera)
            {
                for (size_t i = 0; i < _meshes.size(); i++) {
                    Magnum::GL::Mesh& mesh = _meshes[i];
                    Magnum::Matrix4 scalingMatrix = Magnum::Matrix4::scaling(_scalings[i]);
                    bool isColor = !_materials[i].hasDiffuseTexture();
                    if (isColor) {
                        (_shader.get())
                            .setTransformationMatrix(transformationMatrix * scalingMatrix)
                            .setProjectionMatrix(camera.projectionMatrix())
                            .setMaterial(_materials[i])
                            .draw(mesh);
                    }
                    else {
                        (_texture_shader.get())
                            .setTransformationMatrix(transformationMatrix * scalingMatrix)
                            .setProjectionMatrix(camera.projectionMatrix())
                            .setMaterial(_materials[i])
                            .draw(mesh);
                    }
                }
            }

            // ShadowedColorObject
            ShadowedColorObject::ShadowedColorObject(
                const std::vector<std::reference_wrapper<Magnum::GL::Mesh>>& meshes,
                std::reference_wrapper<gs::ShadowMapColor> shader,
                std::reference_wrapper<gs::ShadowMapColor> texture_shader,
                Object3D* parent,
                Magnum::SceneGraph::DrawableGroup3D* group)
                : Object3D{parent},
                  Magnum::SceneGraph::Drawable3D{*this, group},
                  _meshes{meshes},
                  _shader{shader},
                  _texture_shader(texture_shader) {}

            ShadowedColorObject& ShadowedColorObject::setMeshes(const std::vector<std::reference_wrapper<Magnum::GL::Mesh>>& meshes)
            {
                _meshes = meshes;
                return *this;
            }

            ShadowedColorObject& ShadowedColorObject::setMaterials(const std::vector<gs::Material>& materials)
            {
                _materials = materials;
                return *this;
            }

            ShadowedColorObject& ShadowedColorObject::setScalings(const std::vector<Magnum::Vector3>& scalings)
            {
                _scalings = scalings;
                return *this;
            }

            void ShadowedColorObject::draw(const Magnum::Matrix4& transformationMatrix, Magnum::SceneGraph::Camera3D& camera)
            {
                for (size_t i = 0; i < _meshes.size(); i++) {
                    Magnum::GL::Mesh& mesh = _meshes[i];
                    Magnum::Matrix4 scalingMatrix = Magnum::Matrix4::scaling(_scalings[i]);
                    bool isColor = !_materials[i].hasDiffuseTexture();
                    if (isColor) {
                        (_shader.get())
                            .setTransformationMatrix(transformationMatrix * scalingMatrix)
                            .setProjectionMatrix(camera.projectionMatrix())
                            .setMaterial(_materials[i])
                            .draw(mesh);
                    }
                    else {
                        (_texture_shader.get())
                            .setTransformationMatrix(transformationMatrix * scalingMatrix)
                            .setProjectionMatrix(camera.projectionMatrix())
                            .setMaterial(_materials[i])
                            .draw(mesh);
                    }
                }
            }

            // CubeMapShadowedObject
            CubeMapShadowedObject::CubeMapShadowedObject(
                const std::vector<std::reference_wrapper<Magnum::GL::Mesh>>& meshes,
                std::reference_wrapper<gs::CubeMap> shader,
                std::reference_wrapper<gs::CubeMap> texture_shader,
                Object3D* parent,
                Magnum::SceneGraph::DrawableGroup3D* group)
                : Object3D{parent},
                  Magnum::SceneGraph::Drawable3D{*this, group},
                  _meshes{meshes},
                  _shader{shader},
                  _texture_shader(texture_shader) {}

            CubeMapShadowedObject& CubeMapShadowedObject::setMeshes(const std::vector<std::reference_wrapper<Magnum::GL::Mesh>>& meshes)
            {
                _meshes = meshes;
                return *this;
            }

            CubeMapShadowedObject& CubeMapShadowedObject::setMaterials(const std::vector<gs::Material>& materials)
            {
                _materials = materials;
                return *this;
            }

            CubeMapShadowedObject& CubeMapShadowedObject::setScalings(const std::vector<Magnum::Vector3>& scalings)
            {
                _scalings = scalings;
                return *this;
            }

            void CubeMapShadowedObject::draw(const Magnum::Matrix4&, Magnum::SceneGraph::Camera3D&)
            {
                for (size_t i = 0; i < _meshes.size(); i++) {
                    Magnum::GL::Mesh& mesh = _meshes[i];
                    Magnum::Matrix4 scalingMatrix = Magnum::Matrix4::scaling(_scalings[i]);
                    bool isColor = !_materials[i].hasDiffuseTexture();
                    if (isColor) {
                        (_shader.get())
                            .setTransformationMatrix(absoluteTransformation() * scalingMatrix)
                            .setMaterial(_materials[i])
                            .draw(mesh);
                    }
                    else {
                        (_texture_shader.get())
                            .setTransformationMatrix(absoluteTransformation() * scalingMatrix)
                            .setMaterial(_materials[i])
                            .draw(mesh);
                    }
                }
            }

            // CubeMapShadowedColorObject
            CubeMapShadowedColorObject::CubeMapShadowedColorObject(
                const std::vector<std::reference_wrapper<Magnum::GL::Mesh>>& meshes,
                std::reference_wrapper<gs::CubeMapColor> shader,
                std::reference_wrapper<gs::CubeMapColor> texture_shader,
                Object3D* parent,
                Magnum::SceneGraph::DrawableGroup3D* group)
                : Object3D{parent},
                  Magnum::SceneGraph::Drawable3D{*this, group},
                  _meshes{meshes},
                  _shader{shader},
                  _texture_shader(texture_shader) {}

            CubeMapShadowedColorObject& CubeMapShadowedColorObject::setMeshes(const std::vector<std::reference_wrapper<Magnum::GL::Mesh>>& meshes)
            {
                _meshes = meshes;
                return *this;
            }

            CubeMapShadowedColorObject& CubeMapShadowedColorObject::setMaterials(const std::vector<gs::Material>& materials)
            {
                _materials = materials;
                return *this;
            }

            CubeMapShadowedColorObject& CubeMapShadowedColorObject::setScalings(const std::vector<Magnum::Vector3>& scalings)
            {
                _scalings = scalings;
                return *this;
            }

            void CubeMapShadowedColorObject::draw(const Magnum::Matrix4&, Magnum::SceneGraph::Camera3D&)
            {
                for (size_t i = 0; i < _meshes.size(); i++) {
                    Magnum::GL::Mesh& mesh = _meshes[i];
                    Magnum::Matrix4 scalingMatrix = Magnum::Matrix4::scaling(_scalings[i]);
                    bool isColor = !_materials[i].hasDiffuseTexture();
                    if (isColor) {
                        (_shader.get())
                            .setTransformationMatrix(absoluteTransformation() * scalingMatrix)
                            .setMaterial(_materials[i])
                            .draw(mesh);
                    }
                    else {
                        (_texture_shader.get())
                            .setTransformationMatrix(absoluteTransformation() * scalingMatrix)
                            .setMaterial(_materials[i])
                            .draw(mesh);
                    }
                }
            }

            // BaseApplication
            BaseApplication::BaseApplication(bool isShadowed, bool drawTransparentShadows)
            {
                enableShadows(isShadowed, drawTransparentShadows);
            }

            void BaseApplication::init(const dart::simulation::WorldPtr& world, size_t width, size_t height)
            {
                /* Camera setup */
                _camera.reset(
                    new gs::Camera(_scene, static_cast<int>(width), static_cast<int>(height)));

                /* Shadow camera */
                _shadowCameraObject = new Object3D{&_scene};
                _shadowCamera.reset(new Camera3D{*_shadowCameraObject});

                /* Create our DARTIntegration object/world */
                auto dartObj = new Object3D{&_scene};
                _dartWorld.reset(new Magnum::DartIntegration::World(_importer_manager, *dartObj, *world)); /* Plugin manager is now thread-safe */

                /* Phong shaders */
                _color_shader.reset(new gs::PhongMultiLight{{}, _maxLights});
                _texture_shader.reset(new gs::PhongMultiLight{{gs::PhongMultiLight::Flag::DiffuseTexture}, _maxLights});

                /* Shadow shaders */
                _shadow_shader.reset(new gs::ShadowMap());
                _shadow_texture_shader.reset(new gs::ShadowMap(gs::ShadowMap::Flag::DiffuseTexture));
                _cubemap_shader.reset(new gs::CubeMap());
                _cubemap_texture_shader.reset(new gs::CubeMap(gs::CubeMap::Flag::DiffuseTexture));

                _shadow_color_shader.reset(new gs::ShadowMapColor());
                _shadow_texture_color_shader.reset(new gs::ShadowMapColor(gs::ShadowMapColor::Flag::DiffuseTexture));

                _cubemap_color_shader.reset(new gs::CubeMapColor());
                _cubemap_texture_color_shader.reset(new gs::CubeMapColor(gs::CubeMapColor::Flag::DiffuseTexture));

                /* Add default lights (2 directional lights) */
                gs::Material mat;
                mat.diffuseColor() = {0.8f, 0.8f, 0.8f, 1.f};
                mat.specularColor() = {1.f, 1.f, 1.f, 1.f};
                // gs::Light light = gs::createPointLight({0.f, 0.f, 2.f}, mat, 2.f, {0.f, 0.f, 1.f});
                // gs::Light light = gs::createSpotLight(
                //     {0.f, 0.f, 3.f}, mat, {0.f, 0.f, -1.f}, 1.f, Magnum::Math::Constants<Magnum::Float>::piHalf() / 5.f, 2.f, {0.f, 0.f, 1.f});
                Magnum::Vector3 dir = {-0.5f, -0.5f, -0.5f};
                gs::Light light = gs::createDirectionalLight(dir, mat);
                _lights.push_back(light);
                dir = {0.5f, 0.5f, -0.5f};
                light = gs::createDirectionalLight(dir, mat);
                _lights.push_back(light);
                // Magnum::Vector3 lpos = {0.f, 0.5f, 1.f};
                // Magnum::Vector3 ldir = {0.f, 0.f, -1.f};
                // Magnum::Float lexp = 1.f;
                // Magnum::Float lspot = M_PI / 3.;
                // Magnum::Float lint = 1.f;
                // Magnum::Vector3 latt = {0.f, 0.f, 1.f};
                // light = gs::createSpotLight(lpos, mat, ldir, lexp, lspot, lint, latt);
                // // _lights.push_back(light);
                // lpos = {0.5f, -0.5f, 0.6f};
                // light = gs::createPointLight(lpos, mat, lint, latt);
                // // _lights.push_back(light);
                // lpos = {0.5f, 0.5f, 0.6f};
                // light = gs::createPointLight(lpos, mat, lint, latt);
                // // _lights.push_back(light);

                // lpos = {0.5f, 1.5f, 2.f};
                // latt = {1.f, 0.2f, 0.f};
                // light = gs::createPointLight(lpos, mat, lint, latt);
                // _lights.push_back(light);
                // lpos = {-2.f, -1.f, 2.f};
                // light = gs::createPointLight(lpos, mat, lint, latt);
                // _lights.push_back(light);
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
                if (static_cast<int>(_lights.size()) < _maxLights)
                    _lights.push_back(light);
                // TO-DO: Output warning
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

                if (_shadowTexture) {
                    _color_shader->bindShadowTexture(*_shadowTexture);
                    _texture_shader->bindShadowTexture(*_shadowTexture);
                }

                if (_shadowColorTexture) {
                    _color_shader->bindShadowColorTexture(*_shadowColorTexture);
                    _texture_shader->bindShadowColorTexture(*_shadowColorTexture);
                }

                if (_shadowCubeMap) {
                    _color_shader->bindCubeMapTexture(*_shadowCubeMap);
                    _texture_shader->bindCubeMapTexture(*_shadowCubeMap);
                }

                if (_shadowColorCubeMap) {
                    _color_shader->bindCubeMapColorTexture(*_shadowColorCubeMap);
                    _texture_shader->bindCubeMapColorTexture(*_shadowColorCubeMap);
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
                    bool transparent = false;

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
                        if (!isColor || mat.diffuseColor().a() != 1.f)
                            transparent = true;
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
                        drawableObject->setTransparent(transparent);
                        auto shadowedObject = new ShadowedObject(meshes, *_shadow_shader, *_shadow_texture_shader, static_cast<Object3D*>(&(object.object())), &_shadowed_drawables);
                        shadowedObject->setScalings(scalings);
                        shadowedObject->setMaterials(materials);
                        auto cubeMapObject = new CubeMapShadowedObject(meshes, *_cubemap_shader, *_cubemap_texture_shader, static_cast<Object3D*>(&(object.object())), &_cubemap_drawables);
                        cubeMapObject->setScalings(scalings);
                        cubeMapObject->setMaterials(materials);
                        auto shadowedColorObject = new ShadowedColorObject(meshes, *_shadow_color_shader, *_shadow_texture_color_shader, static_cast<Object3D*>(&(object.object())), &_shadowed_color_drawables);
                        shadowedColorObject->setScalings(scalings);
                        shadowedColorObject->setMaterials(materials);
                        auto cubeMapColorObject = new CubeMapShadowedColorObject(meshes, *_cubemap_color_shader, *_cubemap_texture_color_shader, static_cast<Object3D*>(&(object.object())), &_cubemap_color_drawables);
                        cubeMapColorObject->setScalings(scalings);
                        cubeMapColorObject->setMaterials(materials);

                        auto obj = new ObjectStruct{};
                        obj->drawable = drawableObject;
                        obj->shadowed = shadowedObject;
                        obj->cubemapped = cubeMapObject;
                        obj->shadowed_color = shadowedColorObject;
                        obj->cubemapped_color = cubeMapColorObject;
                        it.first->second = obj;
                        _transparentSize++;
                    }
                    else {
                        /* Otherwise, update the mesh and the material data */
                        auto obj = it.first->second;

                        if (_drawTransparentShadows) {
                            /* Check if it was transparent before */
                            auto& mats = obj->drawable->materials();
                            bool any = false;
                            for (size_t j = 0; j < mats.size(); j++) {
                                // Assume textures are transparent objects so that everything gets drawn better
                                // TO-DO: Check if this is okay to do?
                                bool isTextured = mats[j].hasDiffuseTexture();
                                if (isTextured || mats[j].diffuseColor().a() != 1.f) {
                                    any = true;
                                    break;
                                }
                            }
                            /* if it wasn't transparent and now it is, increase number */
                            if (!any && transparent)
                                _transparentSize++;
                            /* else if it was transparent and now it isn't, decrease number */
                            else if (any && !transparent)
                                _transparentSize--;
                        }

                        // TO-DO: Do I need to re-set the shaders?!
                        obj->drawable->setMeshes(meshes).setMaterials(materials).setSoftBodies(isSoftBody).setScalings(scalings).setTransparent(transparent).setColorShader(*_color_shader).setTextureShader(*_texture_shader);
                        obj->shadowed->setMeshes(meshes).setMaterials(materials).setScalings(scalings);
                        obj->cubemapped->setMeshes(meshes).setMaterials(materials).setScalings(scalings);
                        obj->shadowed_color->setMeshes(meshes).setMaterials(materials).setScalings(scalings);
                        obj->cubemapped_color->setMeshes(meshes).setMaterials(materials).setScalings(scalings);
                    }
                }

                _dartWorld->clearUpdatedShapeObjects();

                if (_isShadowed)
                    prepareShadows();

                _color_shader->setIsShadowed(_isShadowed);
                _texture_shader->setIsShadowed(_isShadowed);
                _color_shader->setTransparentShadows(_drawTransparentShadows && _transparentSize > 0);
                _texture_shader->setTransparentShadows(_drawTransparentShadows && _transparentSize > 0);

                if (_isShadowed)
                    renderShadows();
            }

            void BaseApplication::renderShadows()
            {
                /* For each light */
                for (size_t i = 0; i < _lights.size(); i++) {
                    bool isPointLight = (_lights[i].position().w() > 0.f) && (_lights[i].spotCutOff() >= M_PI / 2.0);
                    bool cullFront = false;
                    Magnum::Matrix4 cameraMatrix;
                    Magnum::Float farPlane = 20.f, nearPlane = 0.001f;
                    if (!isPointLight) {
                        /* Directional lights */
                        if (_lights[i].position().w() == 0.f) {
                            cameraMatrix = Magnum::Matrix4::lookAt(-_lights[i].position().xyz().normalized() * (farPlane - nearPlane) / 2.f, {}, Magnum::Vector3::yAxis()); //_camera->cameraObject().transformation()[2].xyz()); //.invertedRigid();

                            (*_shadowCamera)
                                .setAspectRatioPolicy(Magnum::SceneGraph::AspectRatioPolicy::Extend)
                                .setProjectionMatrix(Magnum::Matrix4::orthographicProjection({10.f, 10.f}, nearPlane, farPlane))
                                .setViewport({_shadowMapSize, _shadowMapSize});
                            cullFront = true;
                        }
                        /* Spotlights */
                        else if (_lights[i].spotCutOff() < M_PI / 2.0) {
                            Magnum::Vector3 position = _lights[i].position().xyz();
                            cameraMatrix = Magnum::Matrix4::lookAt(position, position + _lights[i].spotDirection().normalized(), Magnum::Vector3::yAxis());

                            (*_shadowCamera)
                                .setAspectRatioPolicy(Magnum::SceneGraph::AspectRatioPolicy::Extend)
                                .setProjectionMatrix(Magnum::Matrix4::perspectiveProjection(Magnum::Rad(_lights[i].spotCutOff() * 2.f), 1.f, nearPlane, farPlane))
                                .setViewport({_shadowMapSize, _shadowMapSize});
                        }
                    }
                    /* Point lights */
                    else {
                        (*_shadowCamera)
                            .setAspectRatioPolicy(Magnum::SceneGraph::AspectRatioPolicy::Extend)
                            .setProjectionMatrix(Magnum::Matrix4::perspectiveProjection(Magnum::Rad(Magnum::Math::Constants<Magnum::Float>::piHalf()), 1.f, nearPlane, farPlane))
                            .setViewport({_shadowMapSize, _shadowMapSize});

                        Magnum::Vector3 lightPos = _lights[i].position().xyz();
                        Magnum::Matrix4 matrices[6];
                        matrices[0] = _shadowCamera->projectionMatrix() * Magnum::Matrix4::lookAt(lightPos, lightPos + Magnum::Vector3::xAxis(), -Magnum::Vector3::yAxis()).invertedRigid();
                        matrices[1] = _shadowCamera->projectionMatrix() * Magnum::Matrix4::lookAt(lightPos, lightPos - Magnum::Vector3::xAxis(), -Magnum::Vector3::yAxis()).invertedRigid();
                        matrices[2] = _shadowCamera->projectionMatrix() * Magnum::Matrix4::lookAt(lightPos, lightPos + Magnum::Vector3::yAxis(), Magnum::Vector3::zAxis()).invertedRigid();
                        matrices[3] = _shadowCamera->projectionMatrix() * Magnum::Matrix4::lookAt(lightPos, lightPos - Magnum::Vector3::yAxis(), -Magnum::Vector3::zAxis()).invertedRigid();
                        matrices[4] = _shadowCamera->projectionMatrix() * Magnum::Matrix4::lookAt(lightPos, lightPos + Magnum::Vector3::zAxis(), -Magnum::Vector3::yAxis()).invertedRigid();
                        matrices[5] = _shadowCamera->projectionMatrix() * Magnum::Matrix4::lookAt(lightPos, lightPos - Magnum::Vector3::zAxis(), -Magnum::Vector3::yAxis()).invertedRigid();

                        _cubemap_shader->setShadowMatrices(matrices);
                        _cubemap_shader->setLightPosition(lightPos);
                        _cubemap_shader->setFarPlane(farPlane);
                        _cubemap_shader->setLightIndex(i);

                        _cubemap_texture_shader->setShadowMatrices(matrices);
                        _cubemap_texture_shader->setLightPosition(lightPos);
                        _cubemap_texture_shader->setFarPlane(farPlane);
                        _cubemap_texture_shader->setLightIndex(i);

                        if (_drawTransparentShadows) {
                            _cubemap_color_shader->setShadowMatrices(matrices);
                            _cubemap_color_shader->setLightPosition(lightPos);
                            _cubemap_color_shader->setFarPlane(farPlane);
                            _cubemap_color_shader->setLightIndex(i);

                            _cubemap_texture_color_shader->setShadowMatrices(matrices);
                            _cubemap_texture_color_shader->setLightPosition(lightPos);
                            _cubemap_texture_color_shader->setFarPlane(farPlane);
                            _cubemap_texture_color_shader->setLightIndex(i);

                            if (_shadowCubeMap) {
                                _cubemap_color_shader->bindCubeMapTexture(*_shadowCubeMap);
                                _cubemap_texture_color_shader->bindCubeMapTexture(*_shadowCubeMap);
                            }
                        }

                        _color_shader->setFarPlane(farPlane);
                        _texture_shader->setFarPlane(farPlane);

                        // cameraMatrix = Magnum::Matrix4::lookAt(lightPos, lightPos + Magnum::Vector3::xAxis(), -Magnum::Vector3::yAxis()); // No effect
                    }

                    _shadowCameraObject->setTransformation(cameraMatrix);
                    Magnum::Matrix4 bias{{0.5f, 0.0f, 0.0f, 0.0f},
                        {0.0f, 0.5f, 0.0f, 0.0f},
                        {0.0f, 0.0f, 0.5f, 0.0f},
                        {0.5f, 0.5f, 0.5f, 1.0f}};
                    _lights[i].setShadowMatrix(bias * _shadowCamera->projectionMatrix() * cameraMatrix.invertedRigid());

                    Magnum::GL::Renderer::setDepthMask(true);
                    Magnum::GL::Renderer::enable(Magnum::GL::Renderer::Feature::DepthTest);
                    if (cullFront)
                        Magnum::GL::Renderer::setFaceCullingMode(Magnum::GL::Renderer::PolygonFacing::Front);
                    _shadowData[i].shadowFramebuffer.bind();
                    if (isPointLight) {
                        /* Clear layer-by-layer of the cube-map texture array */
                        for (size_t k = 0; k < 6; k++) {
                            _shadowData[i].shadowFramebuffer.attachTextureLayer(Magnum::GL::Framebuffer::BufferAttachment::Depth, *_shadowCubeMap, 0, i * 6 + k);
                            _shadowData[i].shadowFramebuffer.clear(Magnum::GL::FramebufferClear::Depth);
                        }
                        /* Attach again the full texture */
                        _shadowData[i].shadowFramebuffer.attachLayeredTexture(Magnum::GL::Framebuffer::BufferAttachment::Depth, *_shadowCubeMap, 0);
                    }
                    else
                        _shadowData[i].shadowFramebuffer.clear(Magnum::GL::FramebufferClear::Depth);

                    if (!isPointLight)
                        _shadowCamera->draw(_shadowed_drawables);
                    else
                        _shadowCamera->draw(_cubemap_drawables);
                    if (cullFront)
                        Magnum::GL::Renderer::setFaceCullingMode(Magnum::GL::Renderer::PolygonFacing::Back);

                    /* Transparent color shadow: main ideas taken from https://wickedengine.net/2018/01/18/easy-transparent-shadow-maps/ */
                    if (_drawTransparentShadows && _transparentSize > 0) {
                        Magnum::GL::Renderer::setDepthMask(false);
                        Magnum::GL::Renderer::setColorMask(true, true, true, true);
                        Magnum::GL::Renderer::enable(Magnum::GL::Renderer::Feature::DepthTest);
                        Magnum::GL::Renderer::setClearColor(Magnum::Color4{1.f, 1.f, 1.f, 0.f});
                        Magnum::GL::Renderer::enable(Magnum::GL::Renderer::Feature::Blending);
                        Magnum::GL::Renderer::setBlendFunction(Magnum::GL::Renderer::BlendFunction::DestinationColor, Magnum::GL::Renderer::BlendFunction::Zero);
                        Magnum::GL::Renderer::setBlendEquation(Magnum::GL::Renderer::BlendEquation::Add);

                        if (cullFront)
                            Magnum::GL::Renderer::setFaceCullingMode(Magnum::GL::Renderer::PolygonFacing::Front);
                        _shadowData[i].shadowColorFramebuffer.bind();
                        if (isPointLight) {
                            /* Clear layer-by-layer of the cube-map texture array */
                            for (size_t k = 0; k < 6; k++) {
                                _shadowData[i].shadowColorFramebuffer.attachTextureLayer(Magnum::GL::Framebuffer::ColorAttachment(0), *_shadowColorCubeMap, 0, i * 6 + k);
                                _shadowData[i].shadowColorFramebuffer.clear(Magnum::GL::FramebufferClear::Color);
                            }
                            /* Attach again the full texture */
                            _shadowData[i].shadowColorFramebuffer.attachLayeredTexture(Magnum::GL::Framebuffer::ColorAttachment(0), *_shadowColorCubeMap, 0);
                            // _shadowData[i].shadowColorFramebuffer.attachTextureLayer(Magnum::GL::Framebuffer::BufferAttachment::Depth, *_shadowCubeMap, 0, i * 6);
                        }
                        else
                            _shadowData[i].shadowColorFramebuffer.clear(Magnum::GL::FramebufferClear::Color);

                        /* Draw only transparent objects for transparent shadow color */
                        std::vector<std::pair<std::reference_wrapper<Magnum::SceneGraph::Drawable3D>, Magnum::Matrix4>>
                            drawableTransformations;
                        if (!isPointLight)
                            drawableTransformations = _shadowCamera->drawableTransformations(_shadowed_color_drawables);
                        else
                            drawableTransformations = _shadowCamera->drawableTransformations(_cubemap_color_drawables);

                        std::vector<std::pair<std::reference_wrapper<Magnum::SceneGraph::Drawable3D>, Magnum::Matrix4>> opaque, transparent;
                        for (size_t i = 0; i < drawableTransformations.size(); i++) {
                            auto& obj = static_cast<DrawableObject&>(drawableTransformations[i].first.get().object());
                            auto& mats = obj.materials();
                            bool any = false;
                            for (size_t j = 0; j < mats.size(); j++) {
                                // Assume textures are transparent objects so that everything gets drawn better
                                // TO-DO: Check if this is okay to do?
                                bool isTextured = mats[j].hasDiffuseTexture();
                                if (isTextured || mats[j].diffuseColor().a() != 1.f) {
                                    any = true;
                                    break;
                                }
                            }
                            if (!any)
                                opaque.push_back(drawableTransformations[i]);
                            else
                                transparent.push_back(drawableTransformations[i]);
                        }

                        if (transparent.size() > 0)
                            _shadowCamera->draw(transparent);

                        if (cullFront)
                            Magnum::GL::Renderer::setFaceCullingMode(Magnum::GL::Renderer::PolygonFacing::Back);
                        Magnum::GL::Renderer::setDepthMask(true);
                        Magnum::GL::Renderer::setColorMask(true, true, true, true);
                        Magnum::GL::Renderer::setClearColor(Magnum::Color4{0.f, 0.f, 0.f, 0.f});
                    }
                }
            }

            bool BaseApplication::attachCamera(gs::Camera& camera, const std::string& name)
            {
                for (Magnum::DartIntegration::Object& object : _dartWorld->objects()) {
                    if (object.bodyNode() && object.bodyNode()->getName() == name) {
                        camera.cameraObject().setParent(static_cast<Object3D*>(&object.object()));
                        return true;
                    }
                    if (object.shapeNode() && object.shapeNode()->getName() == name) {
                        camera.cameraObject().setParent(static_cast<Object3D*>(&object.object()));
                        return true;
                    }
                }

                return false;
            }

            void BaseApplication::enableShadows(bool enable, bool drawTransparentShadows)
            {
                _isShadowed = enable;
                _drawTransparentShadows = drawTransparentShadows;
#ifdef MAGNUM_MAC_OSX
                ROBOT_DART_WARNING(_isShadowed, "Shadows are not working properly on Mac! Disable them if you experience unexpected behavior..");
#endif
            }

            GrayscaleImage BaseApplication::depthImage()
            {
                auto& depth_image = _camera->depthImage();
                if (!depth_image)
                    return GrayscaleImage();
                auto pixels = depth_image->pixels<Magnum::Float>();
                auto sz = pixels.size();

                GrayscaleImage img;
                // TO-DO: Make this more performant
                size_t width = sz[1];
                size_t height = sz[0];
                img.resize(width);
                for (size_t w = 0; w < width; w++) {
                    img[w].resize(height);
                    for (size_t h = 0; h < height; h++) {
                        Magnum::Float depth = pixels[height - 1 - h][w];

                        /* Linearize depth for visualization */
                        Magnum::Float zNear = _camera->nearPlane();
                        Magnum::Float zFar = _camera->farPlane();
                        Magnum::Float val = (2.f * zNear) / (zFar + zNear - depth * (zFar - zNear));
                        img[w][h] = val * 255.f;
                    }
                }

                return img;
            }

            GrayscaleImage BaseApplication::rawDepthImage()
            {
                auto& depth_image = _camera->depthImage();
                if (!depth_image)
                    return GrayscaleImage();
                auto pixels = depth_image->pixels<Magnum::Float>();
                auto sz = pixels.size();

                GrayscaleImage img;
                // TO-DO: Make this more performant
                size_t width = sz[1];
                size_t height = sz[0];
                img.resize(width);
                for (size_t w = 0; w < width; w++) {
                    img[w].resize(height);
                    for (size_t h = 0; h < height; h++) {
                        Magnum::Float depth = pixels[height - 1 - h][w];
                        img[w][h] = depth * 255.f;
                    }
                }

                return img;
            }

            void BaseApplication::GLCleanUp()
            {
                /* Clean up GL because of destructor order */
                _color_shader.reset();
                _texture_shader.reset();
                _shadow_shader.reset();
                _shadow_texture_shader.reset();
                _shadow_color_shader.reset();
                _shadow_texture_color_shader.reset();
                _cubemap_shader.reset();
                _cubemap_texture_shader.reset();
                _cubemap_color_shader.reset();
                _cubemap_texture_color_shader.reset();
                _shadowTexture.reset();
                _shadowColorTexture.reset();
                _shadowCubeMap.reset();
                _shadowColorCubeMap.reset();

                _camera.reset();
                _shadowCamera.reset();

                _dartWorld.reset();
                for (auto& it : _drawableObjects)
                    delete it.second;
                _drawableObjects.clear();
                _dartObjs.clear();
                _lights.clear();
                _shadowData.clear();
            }

            void BaseApplication::prepareShadows()
            {
                /* Shadow Textures */
                if (!_shadowTexture) {
                    _shadowTexture.reset(new Magnum::GL::Texture2DArray{});
                    _shadowTexture->setStorage(1, Magnum::GL::TextureFormat::DepthComponent24, {_shadowMapSize, _shadowMapSize, _maxLights})
                        .setCompareFunction(Magnum::GL::SamplerCompareFunction::LessOrEqual)
                        .setCompareMode(Magnum::GL::SamplerCompareMode::CompareRefToTexture)
                        .setMinificationFilter(Magnum::GL::SamplerFilter::Linear, Magnum::GL::SamplerMipmap::Base)
                        .setMagnificationFilter(Magnum::GL::SamplerFilter::Linear);
                    // .setWrapping(Magnum::GL::SamplerWrapping::ClampToEdge);
                }

                if (_drawTransparentShadows && !_shadowColorTexture) {
                    _shadowColorTexture.reset(new Magnum::GL::Texture2DArray{});
                    _shadowColorTexture->setStorage(1, Magnum::GL::TextureFormat::RGBA32F, {_shadowMapSize, _shadowMapSize, _maxLights})
                        .setCompareFunction(Magnum::GL::SamplerCompareFunction::LessOrEqual)
                        .setCompareMode(Magnum::GL::SamplerCompareMode::CompareRefToTexture)
                        .setMinificationFilter(Magnum::GL::SamplerFilter::Linear, Magnum::GL::SamplerMipmap::Base)
                        .setMagnificationFilter(Magnum::GL::SamplerFilter::Linear)
                        .setDepthStencilMode(Magnum::GL::SamplerDepthStencilMode::DepthComponent);
                }

                if (!_shadowCubeMap) {
                    _shadowCubeMap.reset(new Magnum::GL::CubeMapTextureArray{});
                    _shadowCubeMap->setStorage(1, Magnum::GL::TextureFormat::DepthComponent24, {_shadowMapSize, _shadowMapSize, _maxLights * 6})
                        .setCompareFunction(Magnum::GL::SamplerCompareFunction::LessOrEqual)
                        .setCompareMode(Magnum::GL::SamplerCompareMode::CompareRefToTexture)
                        .setMinificationFilter(Magnum::GL::SamplerFilter::Linear, Magnum::GL::SamplerMipmap::Base)
                        .setMagnificationFilter(Magnum::GL::SamplerFilter::Linear)
                        .setWrapping(Magnum::GL::SamplerWrapping::ClampToEdge)
                        .setDepthStencilMode(Magnum::GL::SamplerDepthStencilMode::DepthComponent);
                }

                if (_drawTransparentShadows && !_shadowColorCubeMap) {
                    _shadowColorCubeMap.reset(new Magnum::GL::CubeMapTextureArray{});
                    _shadowColorCubeMap->setStorage(1, Magnum::GL::TextureFormat::RGBA32F, {_shadowMapSize, _shadowMapSize, _maxLights * 6})
                        .setCompareFunction(Magnum::GL::SamplerCompareFunction::LessOrEqual)
                        .setCompareMode(Magnum::GL::SamplerCompareMode::CompareRefToTexture)
                        .setMinificationFilter(Magnum::GL::SamplerFilter::Linear, Magnum::GL::SamplerMipmap::Base)
                        .setMagnificationFilter(Magnum::GL::SamplerFilter::Linear)
                        .setWrapping(Magnum::GL::SamplerWrapping::ClampToEdge);
                    // .setDepthStencilMode(Magnum::GL::SamplerDepthStencilMode::DepthComponent);
                }

                /* For each light */
                for (size_t i = 0; i < _lights.size(); i++) {
                    /* There's no shadow texture/framebuffer for this light */
                    if (_shadowData.size() <= i) {
                        bool isPointLight = (_lights[i].position().w() > 0.f) && (_lights[i].spotCutOff() >= M_PI / 2.0);

                        _shadowData.push_back({});

                        _shadowData[i].shadowFramebuffer = Magnum::GL::Framebuffer({{}, {_shadowMapSize, _shadowMapSize}});
                        if (_drawTransparentShadows)
                            _shadowData[i].shadowColorFramebuffer = Magnum::GL::Framebuffer({{}, {_shadowMapSize, _shadowMapSize}});

                        if (!isPointLight) {
                            (_shadowData[i].shadowFramebuffer)
                                .attachTextureLayer(Magnum::GL::Framebuffer::BufferAttachment::Depth, *_shadowTexture, 0, i)
                                .mapForDraw(Magnum::GL::Framebuffer::DrawAttachment::None)
                                .bind();
                            if (_drawTransparentShadows)
                                (_shadowData[i].shadowColorFramebuffer)
                                    .attachTextureLayer(Magnum::GL::Framebuffer::ColorAttachment(0), *_shadowColorTexture, 0, i)
                                    .attachTextureLayer(Magnum::GL::Framebuffer::BufferAttachment::Depth, *_shadowTexture, 0, i)
                                    .bind();
                        }
                        else {
                            (_shadowData[i].shadowFramebuffer)
                                .mapForDraw(Magnum::GL::Framebuffer::DrawAttachment::None)
                                .attachLayeredTexture(Magnum::GL::Framebuffer::BufferAttachment::Depth, *_shadowCubeMap, 0)
                                .bind();
                            if (_drawTransparentShadows)
                                (_shadowData[i].shadowColorFramebuffer)
                                    .attachLayeredTexture(Magnum::GL::Framebuffer::ColorAttachment(0), *_shadowColorCubeMap, 0)
                                    // .attachLayeredTexture(Magnum::GL::Framebuffer::BufferAttachment::Depth, *_shadowCubeMap, 0)
                                    .bind();
                        }

                        /* Check if the creation of the framebuffer was successful */
                        if (!(_shadowData[i].shadowFramebuffer.checkStatus(Magnum::GL::FramebufferTarget::Draw) == Magnum::GL::Framebuffer::Status::Complete)
                            || (_drawTransparentShadows && !(_shadowData[i].shadowColorFramebuffer.checkStatus(Magnum::GL::FramebufferTarget::Draw) == Magnum::GL::Framebuffer::Status::Complete))) {
                            _isShadowed = false;
                            break;
                        }
                    }
                }

                ROBOT_DART_WARNING(!_isShadowed, "Something bad happened when configuring shadows! Disabling them!");
            }
        } // namespace magnum
    } // namespace gui
} // namespace robot_dart