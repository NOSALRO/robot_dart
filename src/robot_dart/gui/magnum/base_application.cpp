#include "base_application.hpp"

#include <robot_dart/utils.hpp>

#include <dart/dynamics/SoftBodyNode.hpp>
#include <dart/dynamics/SoftMeshShape.hpp>

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
                        _color_shader.get()
                            .setMaterial(_materials[i])
                            .setTransformationMatrix(absoluteTransformationMatrix() * scalingMatrix)
                            .setNormalMatrix((transformationMatrix * scalingMatrix).rotationScaling())
                            .setCameraMatrix(camera.cameraMatrix())
                            .setProjectionMatrix(camera.projectionMatrix());
                    }
                    else {
                        _texture_shader.get()
                            .setMaterial(_materials[i])
                            .setTransformationMatrix(absoluteTransformationMatrix() * scalingMatrix)
                            .setNormalMatrix((transformationMatrix * scalingMatrix).rotationScaling())
                            .setCameraMatrix(camera.cameraMatrix())
                            .setProjectionMatrix(camera.projectionMatrix());
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

            // ShadowedObject
            ShadowedObject::ShadowedObject(
                const std::vector<std::reference_wrapper<Magnum::GL::Mesh>>& meshes,
                std::reference_wrapper<gs::ShadowMap> shader,
                Object3D* parent,
                Magnum::SceneGraph::DrawableGroup3D* group)
                : Object3D{parent},
                  Magnum::SceneGraph::Drawable3D{*this, group},
                  _meshes{meshes},
                  _shader{shader} {}

            ShadowedObject& ShadowedObject::setMeshes(const std::vector<std::reference_wrapper<Magnum::GL::Mesh>>& meshes)
            {
                _meshes = meshes;
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
                    _shader.get().setTransformationMatrix(transformationMatrix * scalingMatrix).setProjectionMatrix(camera.projectionMatrix());
                    mesh.draw(_shader);
                }
            }

            // CubeMapShadowedObject
            CubeMapShadowedObject::CubeMapShadowedObject(
                const std::vector<std::reference_wrapper<Magnum::GL::Mesh>>& meshes,
                std::reference_wrapper<gs::CubeMap> shader,
                Object3D* parent,
                Magnum::SceneGraph::DrawableGroup3D* group)
                : Object3D{parent},
                  Magnum::SceneGraph::Drawable3D{*this, group},
                  _meshes{meshes},
                  _shader{shader} {}

            CubeMapShadowedObject& CubeMapShadowedObject::setMeshes(const std::vector<std::reference_wrapper<Magnum::GL::Mesh>>& meshes)
            {
                _meshes = meshes;
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
                    _shader.get().setTransformationMatrix(absoluteTransformation() * scalingMatrix);
                    mesh.draw(_shader);
                }
            }

            // BaseApplication
            void BaseApplication::init(const dart::simulation::WorldPtr& world, size_t width, size_t height)
            {
                /* Camera setup */
                _camera.reset(
                    new gs::Camera(_scene, static_cast<int>(width), static_cast<int>(height)));

                /* Shadow camera */
                _shadowCameraObject = new Object3D{&_scene};
                _shadowCamera.reset(new Camera3D{*_shadowCameraObject});

                /* Shadow Textures */
                _shadowTexture.reset(new Magnum::GL::Texture2DArray{});
                _shadowTexture->setImage(0, Magnum::GL::TextureFormat::DepthComponent, Magnum::ImageView3D{Magnum::GL::PixelFormat::DepthComponent, Magnum::GL::PixelType::Float, {_shadowMapSize, _shadowMapSize, _maxLights}})
                    .setMaxLevel(0)
                    .setCompareFunction(Magnum::GL::SamplerCompareFunction::LessOrEqual)
                    .setCompareMode(Magnum::GL::SamplerCompareMode::CompareRefToTexture)
                    .setMinificationFilter(Magnum::GL::SamplerFilter::Linear, Magnum::GL::SamplerMipmap::Base)
                    .setMagnificationFilter(Magnum::GL::SamplerFilter::Linear);
                // .setWrapping(Magnum::GL::SamplerWrapping::ClampToBorder);

                _shadowCubeMap.reset(new Magnum::GL::CubeMapTextureArray{});
                _shadowCubeMap->setImage(0, Magnum::GL::TextureFormat::DepthComponent, Magnum::ImageView3D{Magnum::GL::PixelFormat::DepthComponent, Magnum::GL::PixelType::Float, {_shadowMapSize, _shadowMapSize, _maxLights * 6}})
                    .setMaxLevel(0)
                    .setCompareFunction(Magnum::GL::SamplerCompareFunction::LessOrEqual)
                    .setCompareMode(Magnum::GL::SamplerCompareMode::CompareRefToTexture)
                    .setMinificationFilter(Magnum::GL::SamplerFilter::Nearest, Magnum::GL::SamplerMipmap::Base)
                    .setMagnificationFilter(Magnum::GL::SamplerFilter::Nearest)
                    .setWrapping(Magnum::GL::SamplerWrapping::ClampToEdge)
                    .setDepthStencilMode(Magnum::GL::SamplerDepthStencilMode::DepthComponent);

                /* Create our DARTIntegration object/world */
                auto dartObj = new Object3D{&_scene};
                _dartWorld.reset(new Magnum::DartIntegration::World(_importer_manager, *dartObj, *world)); /* Plugin manager is now thread-safe */

                /* Phong shaders */
                _color_shader.reset(new gs::PhongMultiLight{{}, _maxLights});
                _texture_shader.reset(new gs::PhongMultiLight{{gs::PhongMultiLight::Flag::DiffuseTexture}, _maxLights});

                /* Shadow shaders */
                _shadow_shader.reset(new gs::ShadowMap());
                _cubemap_shader.reset(new gs::CubeMap());

                /* Add default lights (2 directional lights) */
                gs::Material mat;
                mat.diffuseColor() = {1.f, 1.f, 1.f, 1.f};
                mat.specularColor() = {1.f, 1.f, 1.f, 1.f};
                // gs::Light light = gs::createPointLight({0.f, 0.f, 2.f}, mat, 2.f, {0.f, 0.f, 1.f});
                // gs::Light light = gs::createSpotLight(
                //     {0.f, 0.f, 3.f}, mat, {0.f, 0.f, -1.f}, 1.f, Magnum::Math::Constants<Magnum::Float>::piHalf() / 5.f, 2.f, {0.f, 0.f, 1.f});
                Magnum::Vector3 dir = {-0.5f, -0.5f, -0.5f};
                gs::Light light = gs::createDirectionalLight(dir, mat);
                // _lights.push_back(light);
                dir = {0.5f, 0.5f, -0.5f};
                light = gs::createDirectionalLight(dir, mat);
                // _lights.push_back(light);
                Magnum::Vector3 lpos = {0.f, 0.5f, 1.f};
                Magnum::Vector3 ldir = {0.f, 0.f, -1.f};
                Magnum::Float lexp = 1.f;
                Magnum::Float lspot = M_PI / 3.;
                Magnum::Float lint = 1.f;
                Magnum::Vector3 latt = {0.f, 0.f, 1.f};
                light = gs::createSpotLight(lpos, mat, ldir, lexp, lspot, lint, latt);
                // _lights.push_back(light);
                lpos = {0.5f, -0.5f, 0.6f};
                light = gs::createPointLight(lpos, mat, lint, latt);
                // _lights.push_back(light);
                lpos = {0.5f, 0.5f, 0.6f};
                light = gs::createPointLight(lpos, mat, lint, latt);
                // _lights.push_back(light);

                lpos = {0.5f, 1.5f, 2.f};
                latt = {1.f, 0.2f, 0.f};
                light = gs::createPointLight(lpos, mat, lint, latt);
                _lights.push_back(light);
                lpos = {2.f, -1.f, 2.f};
                light = gs::createPointLight(lpos, mat, lint, latt);
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

                _color_shader->bindShadowTexture(*_shadowTexture);
                _texture_shader->bindShadowTexture(*_shadowTexture);
                _color_shader->bindCubeMapTexture(*_shadowCubeMap);
                _texture_shader->bindCubeMapTexture(*_shadowCubeMap);
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
                        auto shadowedObject = new ShadowedObject(meshes, *_shadow_shader, static_cast<Object3D*>(&(object.object())), &_shadowed_drawables);
                        shadowedObject->setScalings(scalings);
                        auto cubeMapObject = new CubeMapShadowedObject(meshes, *_cubemap_shader, static_cast<Object3D*>(&(object.object())), &_cubemap_drawables);
                        cubeMapObject->setScalings(scalings);

                        auto obj = new ObjectStruct{};
                        obj->drawable = drawableObject;
                        obj->shadowed = shadowedObject;
                        obj->cubemapped = cubeMapObject;
                        it.first->second = obj;
                    }
                    else {
                        /* Otherwise, update the mesh and the material data */
                        auto obj = it.first->second;

                        // TO-DO: Do I need to re-set the shaders?!
                        obj->drawable->setMeshes(meshes).setMaterials(materials).setSoftBodies(isSoftBody).setScalings(scalings).setColorShader(*_color_shader).setTextureShader(*_texture_shader);
                        obj->shadowed->setMeshes(meshes).setScalings(scalings);
                        obj->cubemapped->setMeshes(meshes).setScalings(scalings);
                    }
                }

                _dartWorld->clearUpdatedShapeObjects();

#ifdef MAGNUM_MAC_OSX
                ROBOT_DART_WARNING(_isShadowed, "Shadows are not supported in Mac! Disabling them!");
                _isShadowed = false;
#endif

                _color_shader->setIsShadowed(_isShadowed);
                _texture_shader->setIsShadowed(_isShadowed);

                if (_isShadowed)
                    renderShadows();
            }

            void BaseApplication::renderShadows()
            {
                /* For each light */
                for (size_t i = 0; i < _lights.size(); i++) {
                    bool isPointLight = (_lights[i].position().w() > 0.f) && (_lights[i].spotCutOff() >= M_PI / 2.0);
                    /* There's no shadow texture/framebuffer for this light */
                    if (_shadowData.size() <= i) {
                        _shadowData.push_back({});

                        _shadowData[i].shadowFramebuffer = Magnum::GL::Framebuffer({{}, {_shadowMapSize, _shadowMapSize}});

                        if (!isPointLight) {
                            (_shadowData[i].shadowFramebuffer)
                                .attachTextureLayer(Magnum::GL::Framebuffer::BufferAttachment::Depth, *_shadowTexture, 0, i)
                                .mapForDraw(Magnum::GL::Framebuffer::DrawAttachment::None);
                            // .bind();
                            // CORRADE_INTERNAL_ASSERT(shadowFramebuffer.checkStatus(GL::FramebufferTarget::Draw) == GL::Framebuffer::Status::Complete);
                        }
                        else {
                            (_shadowData[i].shadowFramebuffer)
                                // .attachTextureLayer(Magnum::GL::Framebuffer::BufferAttachment::Depth, *_shadowCubeMap, 0, i * 6)
                                .mapForDraw(Magnum::GL::Framebuffer::DrawAttachment::None);
                            // TO-DO: Missing API of Magnum
                            // glNamedFramebufferTexture(_shadowData[i].shadowFramebuffer.id(), GL_DEPTH_ATTACHMENT, _shadowCubeMap->id(), 0); // we choose the layer inside the shader
                        }
                    }

                    bool cullFront = false;
                    Magnum::Matrix4 cameraMatrix;
                    Magnum::Float farPlane = 20.f, nearPlane = 0.001f;
                    if (!isPointLight) {
                        /* Directional lights */
                        if (_lights[i].position().w() == 0.f) {
                            cameraMatrix = Magnum::Matrix4::lookAt(-_lights[i].position().xyz().normalized(), {}, Magnum::Vector3::yAxis()); //_camera->cameraObject().transformation()[2].xyz()); //.invertedRigid();

                            (*_shadowCamera)
                                .setAspectRatioPolicy(Magnum::SceneGraph::AspectRatioPolicy::NotPreserved)
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
                        for (size_t k = 0; k < 6; k++) {
                            _shadowData[i].shadowFramebuffer.attachTextureLayer(Magnum::GL::Framebuffer::BufferAttachment::Depth, *_shadowCubeMap, 0, i * 6 + k);
                            _shadowData[i].shadowFramebuffer.clear(Magnum::GL::FramebufferClear::Depth);
                        }
                    }
                    else
                        _shadowData[i].shadowFramebuffer.clear(Magnum::GL::FramebufferClear::Depth);

                    // TO-DO: Missing API of Magnum
                    if (isPointLight)
                        glNamedFramebufferTexture(_shadowData[i].shadowFramebuffer.id(), GL_DEPTH_ATTACHMENT, _shadowCubeMap->id(), 0); // we choose the layer inside the shader
                    if (!isPointLight)
                        _shadowCamera->draw(_shadowed_drawables);
                    else
                        _shadowCamera->draw(_cubemap_drawables);
                    if (cullFront)
                        Magnum::GL::Renderer::setFaceCullingMode(Magnum::GL::Renderer::PolygonFacing::Back);
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
                _cubemap_shader.reset();
                _shadowTexture.reset();
                _shadowCubeMap.reset();

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
        } // namespace magnum
    } // namespace gui
} // namespace robot_dart