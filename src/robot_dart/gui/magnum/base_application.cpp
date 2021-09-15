#include "base_application.hpp"

#include <robot_dart/gui/magnum/gs/helper.hpp>
#include <robot_dart/robot_dart_simu.hpp>
#include <robot_dart/utils.hpp>

#include <dart/dynamics/SoftBodyNode.hpp>
#include <dart/dynamics/SoftMeshShape.hpp>

#include <Corrade/Containers/StridedArrayView.h>
#include <Corrade/Utility/Resource.h>

#include <Magnum/GL/CubeMapTexture.h>
#include <Magnum/GL/DefaultFramebuffer.h>
#include <Magnum/GL/Renderer.h>
#include <Magnum/GL/Texture.h>
#include <Magnum/GL/TextureFormat.h>
#include <Magnum/MeshTools/Compile.h>
#include <Magnum/MeshTools/CompressIndices.h>
#include <Magnum/MeshTools/Interleave.h>
#include <Magnum/Primitives/Axis.h>
#include <Magnum/Primitives/Square.h>

#include <Magnum/Trade/MeshData.h>
#include <Magnum/Trade/PhongMaterialData.h>

namespace robot_dart {
    namespace gui {
        namespace magnum {
            // GlobalData
            Magnum::Platform::WindowlessGLContext* GlobalData::gl_context()
            {
#ifdef MAGNUM_MAC_OSX
                ROBOT_DART_EXCEPTION_ASSERT(false, "Windowless GLContext unsupported in Mac!");
#endif
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
#ifdef MAGNUM_MAC_OSX
                ROBOT_DART_EXCEPTION_ASSERT(false, "Windowless GLContext unsupported in Mac!");
#endif
                std::lock_guard<std::mutex> lg(_context_mutex);
                for (size_t i = 0; i < _gl_contexts.size(); i++) {
                    if (&_gl_contexts[i] == context) {
                        while (!_gl_contexts[i].release()) {} // release the context
                        _used[i] = false;
                        break;
                    }
                }
            }

            void GlobalData::set_max_contexts(size_t N)
            {
#ifdef MAGNUM_MAC_OSX
                ROBOT_DART_EXCEPTION_ASSERT(false, "Windowless GLContext unsupported in Mac!");
#endif
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

            // BaseApplication
            BaseApplication::BaseApplication(const GraphicsConfiguration& configuration) : _configuration(configuration), _max_lights(configuration.max_lights), _shadow_map_size(configuration.shadow_map_size)
            {
                enable_shadows(configuration.shadowed, configuration.transparent_shadows);
            }

            void BaseApplication::init(RobotDARTSimu* simu, const GraphicsConfiguration& configuration)
            {
                _configuration = configuration;
                _simu = simu;
                /* Camera setup */
                _camera.reset(
                    new gs::Camera(_scene, static_cast<int>(configuration.width), static_cast<int>(configuration.height)));

                /* Shadow camera */
                _shadow_camera_object = new Object3D{&_scene};
                _shadow_camera.reset(new Camera3D{*_shadow_camera_object});

                /* Create our DARTIntegration object/world */
                auto dartObj = new Object3D{&_scene};
                _dart_world.reset(new Magnum::DartIntegration::World(_importer_manager, *dartObj, *simu->world())); /* Plugin manager is now thread-safe */

                /* Phong shaders */
                _color_shader.reset(new gs::PhongMultiLight{{}, _max_lights});
                _texture_shader.reset(new gs::PhongMultiLight{{gs::PhongMultiLight::Flag::DiffuseTexture}, _max_lights});

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
                mat.diffuse_color() = {1.f, 1.f, 1.f, 1.f};
                mat.specular_color() = {1.f, 1.f, 1.f, 1.f};
                // gs::Light light = gs::create_point_light({0.f, 0.f, 2.f}, mat, 2.f, {0.f, 0.f, 1.f});
                // gs::Light light = gs::create_spot_light(
                //     {0.f, 0.f, 3.f}, mat, {0.f, 0.f, -1.f}, 1.f, Magnum::Math::Constants<Magnum::Float>::piHalf() / 5.f, 2.f, {0.f, 0.f, 1.f});
                Magnum::Vector3 dir = {-0.5f, -0.5f, -0.5f};
                gs::Light light = gs::create_directional_light(dir, mat);
                _lights.push_back(light);
                dir = {0.5f, 0.5f, -0.5f};
                light = gs::create_directional_light(dir, mat);
                light.set_casts_shadows(false);
                _lights.push_back(light);
                // Magnum::Vector3 lpos = {0.f, 0.5f, 1.f};
                // Magnum::Vector3 ldir = {0.f, 0.f, -1.f};
                // Magnum::Float lexp = 1.f;
                // Magnum::Float lspot = M_PI / 3.;
                // Magnum::Float lint = 1.f;
                // Magnum::Vector3 latt = {0.f, 0.f, 1.f};
                // light = gs::create_spot_light(lpos, mat, ldir, lexp, lspot, lint, latt);
                // // _lights.push_back(light);
                // lpos = {0.5f, -0.5f, 0.6f};
                // light = gs::create_point_light(lpos, mat, lint, latt);
                // // _lights.push_back(light);
                // lpos = {0.5f, 0.5f, 0.6f};
                // light = gs::create_point_light(lpos, mat, lint, latt);
                // // _lights.push_back(light);

                // lpos = {0.5f, 1.5f, 2.f};
                // latt = {1.f, 0.2f, 0.f};
                // light = gs::create_point_light(lpos, mat, lint, latt);
                // _lights.push_back(light);
                // lpos = {-2.f, -1.f, 2.f};
                // light = gs::create_point_light(lpos, mat, lint, latt);
                // _lights.push_back(light);

                /* Initialize 3D axis visualization mesh */
                _3D_axis_shader.reset(new Magnum::Shaders::VertexColorGL3D);
                _3D_axis_mesh.reset(new Magnum::GL::Mesh);

                _background_shader.reset(new Magnum::Shaders::FlatGL2D);
                _background_mesh.reset(new Magnum::GL::Mesh{Magnum::MeshTools::compile(Magnum::Primitives::squareSolid())});

                Magnum::Trade::MeshData axis_data = Magnum::Primitives::axis3D();

                Magnum::GL::Buffer axis_vertices;
                axis_vertices.setData(Magnum::MeshTools::interleave(axis_data.positions3DAsArray(), axis_data.colorsAsArray()));

                std::pair<Corrade::Containers::Array<char>, Magnum::MeshIndexType> compressed = Magnum::MeshTools::compressIndices(axis_data.indicesAsArray());
                Magnum::GL::Buffer axis_indices;
                axis_indices.setData(compressed.first);

                _3D_axis_mesh->setPrimitive(Magnum::GL::MeshPrimitive::Lines)
                    .setCount(axis_data.indexCount())
                    .addVertexBuffer(std::move(axis_vertices), 0, Magnum::Shaders::VertexColorGL3D::Position{}, Magnum::Shaders::VertexColorGL3D::Color4{})
                    .setIndexBuffer(std::move(axis_indices), 0, compressed.second);

                /* Initialize text visualization */
                if (_configuration.draw_debug && _configuration.draw_text) { // only if we ask for it
                    _font = _font_manager.loadAndInstantiate("TrueTypeFont");
                    if (_font) {
                        Corrade::Utility::Resource rs("RobotDARTShaders");
                        _font->openData(rs.getRaw("SourceSansPro-Regular.ttf"), 180.0f);

                        /* Glyphs we need to render everything */
                        /* Latin characters for now only */
                        _glyph_cache.reset(new Magnum::Text::DistanceFieldGlyphCache{Magnum::Vector2i{2048}, Magnum::Vector2i{512}, 22});
                        _font->fillGlyphCache(*_glyph_cache,
                            "abcdefghijklmnopqrstuvwxyz"
                            "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                            "0123456789:-+*,.!° /|[]()_");

                        /* Initialize buffers for text */
                        _text_vertices.reset(new Magnum::GL::Buffer);
                        _text_indices.reset(new Magnum::GL::Buffer);

                        /* Initialize text shader */
                        _text_shader.reset(new Magnum::Shaders::DistanceFieldVectorGL2D);
                        _text_shader->bindVectorTexture(_glyph_cache->texture());
                    }
                }
            }

            void BaseApplication::clear_lights()
            {
                _lights.clear();
                /* Reset lights in shaders */
                gs::Light light;
                for (int i = 0; i < _color_shader->max_lights(); i++)
                    _color_shader->set_light(i, light);
                for (int i = 0; i < _texture_shader->max_lights(); i++)
                    _texture_shader->set_light(i, light);
            }

            void BaseApplication::add_light(const gs::Light& light)
            {
                ROBOT_DART_ASSERT(static_cast<int>(_lights.size()) < _max_lights, "You cannot add more lights!", );
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

            size_t BaseApplication::num_lights() const
            {
                return _lights.size();
            }

            bool BaseApplication::done() const
            {
                return _done;
            }

            void BaseApplication::look_at(const Eigen::Vector3d& camera_pos,
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

                _camera->look_at(Magnum::Vector3{cx, cy, cz},
                    Magnum::Vector3{lx, ly, lz},
                    Magnum::Vector3{ux, uy, uz});
            }

            void BaseApplication::update_lights(const gs::Camera& camera)
            {
                /* Update lights transformations */
                camera.transform_lights(_lights);

                if (_shadowed)
                    _prepare_shadows();

                if (_shadowed)
                    render_shadows();

                /* Set the shader information */
                for (size_t i = 0; i < _lights.size(); i++) {
                    _color_shader->set_light(i, _lights[i]);
                    _texture_shader->set_light(i, _lights[i]);
                }

                if (_shadow_texture) {
                    _color_shader->bind_shadow_texture(*_shadow_texture);
                    _texture_shader->bind_shadow_texture(*_shadow_texture);
                }

                if (_shadow_color_texture) {
                    _color_shader->bind_shadow_color_texture(*_shadow_color_texture);
                    _texture_shader->bind_shadow_color_texture(*_shadow_color_texture);
                }

                if (_shadow_cube_map) {
                    _color_shader->bind_cube_map_texture(*_shadow_cube_map);
                    _texture_shader->bind_cube_map_texture(*_shadow_cube_map);
                }

                if (_shadow_color_cube_map) {
                    _color_shader->bind_cube_map_color_texture(*_shadow_color_cube_map);
                    _texture_shader->bind_cube_map_color_texture(*_shadow_color_cube_map);
                }

                _color_shader->set_is_shadowed(_shadowed);
                _texture_shader->set_is_shadowed(_shadowed);
                _color_shader->set_transparent_shadows(_transparent_shadows && _transparentSize > 0);
                _texture_shader->set_transparent_shadows(_transparent_shadows && _transparentSize > 0);

                _color_shader->set_specular_strength(_configuration.specular_strength);
                _texture_shader->set_specular_strength(_configuration.specular_strength);
            }

            void BaseApplication::update_graphics()
            {
                /* Refresh the graphical models */
                _dart_world->refresh();

                /* Remove unused/deleted objects */
                auto& unused = _dart_world->unusedObjects();
                for (auto& p : unused) {
                    auto obj = &p->object();
                    auto it = _drawable_objects.begin();
                    while (it != _drawable_objects.end()) {
                        auto obj2 = (it->second->drawable->object().parent());
                        if (obj == obj2) {
                            // Update variables
                            if (_transparent_shadows) {
                                /* Check if it was transparent */
                                auto& mats = it->second->drawable->materials();
                                bool any = false;
                                for (size_t j = 0; j < mats.size(); j++) {
                                    // Assume textures are transparent objects so that everything gets drawn better
                                    // TO-DO: Check if this is okay to do?
                                    bool isTextured = mats[j].has_diffuse_texture();
                                    if (isTextured || mats[j].diffuse_color().a() != 1.f) {
                                        any = true;
                                        break;
                                    }
                                }

                                if (any)
                                    _transparentSize--;
                            }
                            // Remove it from the drawable lists
                            _drawables.remove(*it->second->drawable);
                            _shadowed_drawables.remove(*it->second->shadowed);
                            _shadowed_color_drawables.remove(*it->second->shadowed_color);
                            _cubemap_drawables.remove(*it->second->cubemapped);
                            _cubemap_color_drawables.remove(*it->second->cubemapped_color);
                            // Delete it completely
                            delete it->second;
                            _drawable_objects.erase(it);
                            break;
                        }
                        it++;
                    }
                }

                /* For each update object */
                for (Magnum::DartIntegration::Object& object : _dart_world->updatedShapeObjects()) {
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

                        if (object.drawData().materials[i].hasAttribute(Magnum::Trade::MaterialAttribute::DiffuseTexture)) {
                            mat.set_diffuse_texture(&(*object.drawData().textures[object.drawData().materials[i].diffuseTexture()]));
                            isColor = false;
                        }
                        mat.ambient_color() = object.drawData().materials[i].ambientColor();
                        if (isColor)
                            mat.diffuse_color() = object.drawData().materials[i].diffuseColor();
                        if (!isColor || mat.diffuse_color().a() != 1.f)
                            transparent = true;
                        mat.specular_color() = object.drawData().materials[i].specularColor();
                        mat.shininess() = object.drawData().materials[i].shininess();
                        if (mat.shininess() < 1.f)
                            mat.shininess() = 2000.f;

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
                    auto it = _drawable_objects.insert(std::make_pair(&object, nullptr));
                    if (it.second) {
                        /* If not, create a new object and add it to our drawables list */
                        auto drawableObject = new DrawableObject(_simu, object.shapeNode(), meshes, materials, *_color_shader, *_texture_shader, static_cast<Object3D*>(&(object.object())), &_drawables);
                        drawableObject->set_soft_bodies(isSoftBody);
                        drawableObject->set_scalings(scalings);
                        drawableObject->set_transparent(transparent);
                        auto shadowedObject = new ShadowedObject(_simu, object.shapeNode(), meshes, *_shadow_shader, *_shadow_texture_shader, static_cast<Object3D*>(&(object.object())), &_shadowed_drawables);
                        shadowedObject->set_scalings(scalings);
                        shadowedObject->set_materials(materials);
                        auto cubeMapObject = new CubeMapShadowedObject(_simu, object.shapeNode(), meshes, *_cubemap_shader, *_cubemap_texture_shader, static_cast<Object3D*>(&(object.object())), &_cubemap_drawables);
                        cubeMapObject->set_scalings(scalings);
                        cubeMapObject->set_materials(materials);
                        auto shadowedColorObject = new ShadowedColorObject(_simu, object.shapeNode(), meshes, *_shadow_color_shader, *_shadow_texture_color_shader, static_cast<Object3D*>(&(object.object())), &_shadowed_color_drawables);
                        shadowedColorObject->set_scalings(scalings);
                        shadowedColorObject->set_materials(materials);
                        auto cubeMapColorObject = new CubeMapShadowedColorObject(_simu, object.shapeNode(), meshes, *_cubemap_color_shader, *_cubemap_texture_color_shader, static_cast<Object3D*>(&(object.object())), &_cubemap_color_drawables);
                        cubeMapColorObject->set_scalings(scalings);
                        cubeMapColorObject->set_materials(materials);

                        auto obj = new ObjectStruct{};
                        obj->drawable = drawableObject;
                        obj->shadowed = shadowedObject;
                        obj->cubemapped = cubeMapObject;
                        obj->shadowed_color = shadowedColorObject;
                        obj->cubemapped_color = cubeMapColorObject;
                        it.first->second = obj;
                        if (transparent)
                            _transparentSize++;
                    }
                    else {
                        /* Otherwise, update the mesh and the material data */
                        auto obj = it.first->second;

                        if (_transparent_shadows) {
                            /* Check if it was transparent before */
                            auto& mats = obj->drawable->materials();
                            bool any = false;
                            for (size_t j = 0; j < mats.size(); j++) {
                                // Assume textures are transparent objects so that everything gets drawn better
                                // TO-DO: Check if this is okay to do?
                                bool isTextured = mats[j].has_diffuse_texture();
                                if (isTextured || mats[j].diffuse_color().a() != 1.f) {
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

                        obj->drawable->set_meshes(meshes).set_materials(materials).set_soft_bodies(isSoftBody).set_scalings(scalings).set_transparent(transparent).set_color_shader(*_color_shader).set_texture_shader(*_texture_shader);
                        obj->shadowed->set_meshes(meshes).set_materials(materials).set_scalings(scalings);
                        obj->cubemapped->set_meshes(meshes).set_materials(materials).set_scalings(scalings);
                        obj->shadowed_color->set_meshes(meshes).set_materials(materials).set_scalings(scalings);
                        obj->cubemapped_color->set_meshes(meshes).set_materials(materials).set_scalings(scalings);
                    }
                }

                _dart_world->clearUpdatedShapeObjects();
            }

            void BaseApplication::render_shadows()
            {
                /* For each light */
                for (size_t i = 0; i < _lights.size(); i++) {
                    if (!_lights[i].casts_shadows())
                        continue;
                    bool isPointLight = (_lights[i].position().w() > 0.f) && (_lights[i].spot_cut_off() >= M_PI / 2.0);
                    bool cullFront = false;
                    Magnum::Matrix4 cameraMatrix;
                    Magnum::Float far_plane = 20.f, near_plane = 0.001f;
                    Magnum::Float proj_size = (far_plane - near_plane) / 2.f;
                    if (!isPointLight) {
                        /* Directional lights */
                        if (_lights[i].position().w() == 0.f) {
                            cameraMatrix = Magnum::Matrix4::lookAt(-_lights[i].position().xyz().normalized() * proj_size, {}, Magnum::Vector3::yAxis()); //_camera->camera_object().transformation()[2].xyz()); //.invertedRigid();

                            (*_shadow_camera)
                                .setAspectRatioPolicy(Magnum::SceneGraph::AspectRatioPolicy::Extend)
                                .setProjectionMatrix(Magnum::Matrix4::orthographicProjection({proj_size, proj_size}, near_plane, far_plane))
                                .setViewport({_shadow_map_size, _shadow_map_size});
                            cullFront = false; // if false, peter panning will be quite a bit, but has better acne
                        }
                        /* Spotlights */
                        else if (_lights[i].spot_cut_off() < M_PI / 2.0) {
                            Magnum::Vector3 position = _lights[i].position().xyz();
                            cameraMatrix = Magnum::Matrix4::lookAt(position, position + _lights[i].spot_direction().normalized(), Magnum::Vector3::yAxis());

                            (*_shadow_camera)
                                .setAspectRatioPolicy(Magnum::SceneGraph::AspectRatioPolicy::Extend)
                                .setProjectionMatrix(Magnum::Matrix4::perspectiveProjection(Magnum::Rad(_lights[i].spot_cut_off() * 2.f), 1.f, near_plane, far_plane))
                                .setViewport({_shadow_map_size, _shadow_map_size});
                        }
                    }
                    /* Point lights */
                    else {
                        (*_shadow_camera)
                            .setAspectRatioPolicy(Magnum::SceneGraph::AspectRatioPolicy::Extend)
                            .setProjectionMatrix(Magnum::Matrix4::perspectiveProjection(Magnum::Rad(Magnum::Math::Constants<Magnum::Float>::piHalf()), 1.f, near_plane, far_plane))
                            .setViewport({_shadow_map_size, _shadow_map_size});

                        Magnum::Vector3 lightPos = _lights[i].position().xyz();
                        Magnum::Matrix4 matrices[6];
                        matrices[0] = _shadow_camera->projectionMatrix() * Magnum::Matrix4::lookAt(lightPos, lightPos + Magnum::Vector3::xAxis(), -Magnum::Vector3::yAxis()).invertedRigid();
                        matrices[1] = _shadow_camera->projectionMatrix() * Magnum::Matrix4::lookAt(lightPos, lightPos - Magnum::Vector3::xAxis(), -Magnum::Vector3::yAxis()).invertedRigid();
                        matrices[2] = _shadow_camera->projectionMatrix() * Magnum::Matrix4::lookAt(lightPos, lightPos + Magnum::Vector3::yAxis(), Magnum::Vector3::zAxis()).invertedRigid();
                        matrices[3] = _shadow_camera->projectionMatrix() * Magnum::Matrix4::lookAt(lightPos, lightPos - Magnum::Vector3::yAxis(), -Magnum::Vector3::zAxis()).invertedRigid();
                        matrices[4] = _shadow_camera->projectionMatrix() * Magnum::Matrix4::lookAt(lightPos, lightPos + Magnum::Vector3::zAxis(), -Magnum::Vector3::yAxis()).invertedRigid();
                        matrices[5] = _shadow_camera->projectionMatrix() * Magnum::Matrix4::lookAt(lightPos, lightPos - Magnum::Vector3::zAxis(), -Magnum::Vector3::yAxis()).invertedRigid();

                        _cubemap_shader->set_shadow_matrices(matrices);
                        _cubemap_shader->set_light_position(lightPos);
                        _cubemap_shader->set_far_plane(far_plane);
                        _cubemap_shader->set_light_index(i);

                        _cubemap_texture_shader->set_shadow_matrices(matrices);
                        _cubemap_texture_shader->set_light_position(lightPos);
                        _cubemap_texture_shader->set_far_plane(far_plane);
                        _cubemap_texture_shader->set_light_index(i);

                        if (_transparent_shadows) {
                            _cubemap_color_shader->set_shadow_matrices(matrices);
                            _cubemap_color_shader->set_light_position(lightPos);
                            _cubemap_color_shader->set_far_plane(far_plane);
                            _cubemap_color_shader->set_light_index(i);

                            _cubemap_texture_color_shader->set_shadow_matrices(matrices);
                            _cubemap_texture_color_shader->set_light_position(lightPos);
                            _cubemap_texture_color_shader->set_far_plane(far_plane);
                            _cubemap_texture_color_shader->set_light_index(i);

                            if (_shadow_cube_map) {
                                _cubemap_color_shader->bind_cube_map_texture(*_shadow_cube_map);
                                _cubemap_texture_color_shader->bind_cube_map_texture(*_shadow_cube_map);
                            }
                        }

                        _color_shader->set_far_plane(far_plane);
                        _texture_shader->set_far_plane(far_plane);

                        // cameraMatrix = Magnum::Matrix4::lookAt(lightPos, lightPos + Magnum::Vector3::xAxis(), -Magnum::Vector3::yAxis()); // No effect
                    }

                    _shadow_camera_object->setTransformation(cameraMatrix);
                    Magnum::Matrix4 bias{{0.5f, 0.0f, 0.0f, 0.0f},
                        {0.0f, 0.5f, 0.0f, 0.0f},
                        {0.0f, 0.0f, 0.5f, 0.0f},
                        {0.5f, 0.5f, 0.5f, 1.0f}};
                    _lights[i].set_shadow_matrix(bias * _shadow_camera->projectionMatrix() * cameraMatrix.invertedRigid());

                    Magnum::GL::Renderer::setDepthMask(true);
                    Magnum::GL::Renderer::enable(Magnum::GL::Renderer::Feature::DepthTest);
                    if (cullFront)
                        Magnum::GL::Renderer::setFaceCullingMode(Magnum::GL::Renderer::PolygonFacing::Front);
                    _shadow_data[i].shadow_framebuffer.bind();
                    if (isPointLight) {
                        /* Clear layer-by-layer of the cube-map texture array */
                        for (size_t k = 0; k < 6; k++) {
                            _shadow_data[i].shadow_framebuffer.attachTextureLayer(Magnum::GL::Framebuffer::BufferAttachment::Depth, *_shadow_cube_map, 0, i * 6 + k);
                            _shadow_data[i].shadow_framebuffer.clear(Magnum::GL::FramebufferClear::Depth);
                        }
                        /* Attach again the full texture */
                        _shadow_data[i].shadow_framebuffer.attachLayeredTexture(Magnum::GL::Framebuffer::BufferAttachment::Depth, *_shadow_cube_map, 0);
                    }
                    else
                        _shadow_data[i].shadow_framebuffer.clear(Magnum::GL::FramebufferClear::Depth);

                    if (!isPointLight)
                        _shadow_camera->draw(_shadowed_drawables);
                    else
                        _shadow_camera->draw(_cubemap_drawables);
                    if (cullFront)
                        Magnum::GL::Renderer::setFaceCullingMode(Magnum::GL::Renderer::PolygonFacing::Back);

                    /* Transparent color shadow: main ideas taken from https://wickedengine.net/2018/01/18/easy-transparent-shadow-maps/ */
                    if (_transparent_shadows && _transparentSize > 0) {
                        Magnum::GL::Renderer::setDepthMask(false);
                        Magnum::GL::Renderer::setColorMask(true, true, true, true);
                        Magnum::GL::Renderer::enable(Magnum::GL::Renderer::Feature::DepthTest);
                        Magnum::GL::Renderer::setClearColor(Magnum::Color4{1.f, 1.f, 1.f, 0.f});
                        Magnum::GL::Renderer::enable(Magnum::GL::Renderer::Feature::Blending);
                        Magnum::GL::Renderer::setBlendFunction(Magnum::GL::Renderer::BlendFunction::DestinationColor, Magnum::GL::Renderer::BlendFunction::Zero);
                        Magnum::GL::Renderer::setBlendEquation(Magnum::GL::Renderer::BlendEquation::Add);

                        if (cullFront)
                            Magnum::GL::Renderer::setFaceCullingMode(Magnum::GL::Renderer::PolygonFacing::Front);
                        _shadow_data[i].shadow_color_framebuffer.bind();
                        if (isPointLight) {
                            /* Clear layer-by-layer of the cube-map texture array */
                            for (size_t k = 0; k < 6; k++) {
                                _shadow_data[i].shadow_color_framebuffer.attachTextureLayer(Magnum::GL::Framebuffer::ColorAttachment(0), *_shadow_color_cube_map, 0, i * 6 + k);
                                _shadow_data[i].shadow_color_framebuffer.clear(Magnum::GL::FramebufferClear::Color);
                            }
                            /* Attach again the full texture */
                            _shadow_data[i].shadow_color_framebuffer.attachLayeredTexture(Magnum::GL::Framebuffer::ColorAttachment(0), *_shadow_color_cube_map, 0);
                            // _shadow_data[i].shadow_color_framebuffer.attachTextureLayer(Magnum::GL::Framebuffer::BufferAttachment::Depth, *_shadow_cube_map, 0, i * 6);
                        }
                        else
                            _shadow_data[i].shadow_color_framebuffer.clear(Magnum::GL::FramebufferClear::Color);

                        /* Draw only transparent objects for transparent shadow color */
                        std::vector<std::pair<std::reference_wrapper<Magnum::SceneGraph::Drawable3D>, Magnum::Matrix4>>
                            drawableTransformations;
                        if (!isPointLight)
                            drawableTransformations = _shadow_camera->drawableTransformations(_shadowed_color_drawables);
                        else
                            drawableTransformations = _shadow_camera->drawableTransformations(_cubemap_color_drawables);

                        std::vector<std::pair<std::reference_wrapper<Magnum::SceneGraph::Drawable3D>, Magnum::Matrix4>> opaque, transparent;
                        for (size_t i = 0; i < drawableTransformations.size(); i++) {
                            auto& obj = static_cast<DrawableObject&>(drawableTransformations[i].first.get().object());
                            auto& mats = obj.materials();
                            bool any = false;
                            for (size_t j = 0; j < mats.size(); j++) {
                                // Assume textures are transparent objects so that everything gets drawn better
                                // TO-DO: Check if this is okay to do?
                                bool isTextured = mats[j].has_diffuse_texture();
                                if (isTextured || mats[j].diffuse_color().a() != 1.f) {
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
                            _shadow_camera->draw(transparent);

                        if (cullFront)
                            Magnum::GL::Renderer::setFaceCullingMode(Magnum::GL::Renderer::PolygonFacing::Back);
                        Magnum::GL::Renderer::setDepthMask(true);
                        Magnum::GL::Renderer::setColorMask(true, true, true, true);
                        Magnum::GL::Renderer::setClearColor(Magnum::Color4{0.f, 0.f, 0.f, 0.f});
                    }
                }
            }

            bool BaseApplication::attach_camera(gs::Camera& camera, dart::dynamics::BodyNode* body)
            {
                for (Magnum::DartIntegration::Object& object : _dart_world->objects()) {
                    if (object.bodyNode() && object.bodyNode() == body) {
                        camera.root_object().setParent(static_cast<Object3D*>(&object.object()));
                        return true;
                    }
                    // if (object.shapeNode() && object.shapeNode()->getName() == name) {
                    //     camera.root_object().setParent(static_cast<Object3D*>(&object.object()));
                    //     return true;
                    // }
                }

                return false;
            }

            void BaseApplication::enable_shadows(bool enable, bool drawTransparentShadows)
            {
                _shadowed = enable;
                _transparent_shadows = drawTransparentShadows;
#ifdef MAGNUM_MAC_OSX
                ROBOT_DART_WARNING(_shadowed, "Shadows are not working properly on Mac! Disable them if you experience unexpected behavior..");
#endif
            }

            GrayscaleImage BaseApplication::depth_image()
            {
                auto& depth_image = _camera->depth_image();
                if (!depth_image)
                    return GrayscaleImage();

                return gs::depth_from_image(&*depth_image, true, _camera->near_plane(), _camera->far_plane());
            }

            GrayscaleImage BaseApplication::raw_depth_image()
            {
                auto& depth_image = _camera->depth_image();
                if (!depth_image)
                    return GrayscaleImage();

                return gs::depth_from_image(&*depth_image);
            }

            DepthImage BaseApplication::depth_array()
            {
                auto& depth_image = _camera->depth_image();
                if (!depth_image)
                    return DepthImage();

                return gs::depth_array_from_image(&*depth_image, _camera->near_plane(), _camera->far_plane());
            }

            void BaseApplication::_gl_clean_up()
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
                _shadow_texture.reset();
                _shadow_color_texture.reset();
                _shadow_cube_map.reset();
                _shadow_color_cube_map.reset();
                _3D_axis_shader.reset();
                _3D_axis_mesh.reset();
                _background_mesh.reset();
                _background_shader.reset();
                _text_shader.reset();
                _glyph_cache.reset();
                _font.reset();
                _text_vertices.reset();
                _text_indices.reset();

                _camera.reset();
                _shadow_camera.reset();

                _dart_world.reset();
                for (auto& it : _drawable_objects)
                    delete it.second;
                _drawable_objects.clear();
                _lights.clear();
                _shadow_data.clear();
            }

            void BaseApplication::_prepare_shadows()
            {
                /* Shadow Textures */
                if (!_shadow_texture) {
                    _shadow_texture.reset(new Magnum::GL::Texture2DArray{});
                    _shadow_texture->setStorage(1, Magnum::GL::TextureFormat::DepthComponent24, {_shadow_map_size, _shadow_map_size, _max_lights})
                        .setCompareFunction(Magnum::GL::SamplerCompareFunction::LessOrEqual)
                        .setCompareMode(Magnum::GL::SamplerCompareMode::CompareRefToTexture)
                        .setMinificationFilter(Magnum::GL::SamplerFilter::Linear, Magnum::GL::SamplerMipmap::Base)
                        .setMagnificationFilter(Magnum::GL::SamplerFilter::Linear);
                    // .setWrapping(Magnum::GL::SamplerWrapping::ClampToEdge);
                }

                if (_transparent_shadows && !_shadow_color_texture) {
                    _shadow_color_texture.reset(new Magnum::GL::Texture2DArray{});
                    _shadow_color_texture->setStorage(1, Magnum::GL::TextureFormat::RGBA32F, {_shadow_map_size, _shadow_map_size, _max_lights})
                        .setCompareFunction(Magnum::GL::SamplerCompareFunction::LessOrEqual)
                        .setCompareMode(Magnum::GL::SamplerCompareMode::CompareRefToTexture)
                        .setMinificationFilter(Magnum::GL::SamplerFilter::Linear, Magnum::GL::SamplerMipmap::Base)
                        .setMagnificationFilter(Magnum::GL::SamplerFilter::Linear)
                        .setDepthStencilMode(Magnum::GL::SamplerDepthStencilMode::DepthComponent);
                }

                if (!_shadow_cube_map) {
                    _shadow_cube_map.reset(new Magnum::GL::CubeMapTextureArray{});
                    _shadow_cube_map->setStorage(1, Magnum::GL::TextureFormat::DepthComponent24, {_shadow_map_size, _shadow_map_size, _max_lights * 6})
                        .setCompareFunction(Magnum::GL::SamplerCompareFunction::LessOrEqual)
                        .setCompareMode(Magnum::GL::SamplerCompareMode::CompareRefToTexture)
                        .setMinificationFilter(Magnum::GL::SamplerFilter::Linear, Magnum::GL::SamplerMipmap::Base)
                        .setMagnificationFilter(Magnum::GL::SamplerFilter::Linear)
                        .setWrapping(Magnum::GL::SamplerWrapping::ClampToEdge)
                        .setDepthStencilMode(Magnum::GL::SamplerDepthStencilMode::DepthComponent);
                }

                if (_transparent_shadows && !_shadow_color_cube_map) {
                    _shadow_color_cube_map.reset(new Magnum::GL::CubeMapTextureArray{});
                    _shadow_color_cube_map->setStorage(1, Magnum::GL::TextureFormat::RGBA32F, {_shadow_map_size, _shadow_map_size, _max_lights * 6})
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
                    if (_shadow_data.size() <= i) {
                        bool isPointLight = (_lights[i].position().w() > 0.f) && (_lights[i].spot_cut_off() >= M_PI / 2.0);

                        _shadow_data.push_back({});

                        _shadow_data[i].shadow_framebuffer = Magnum::GL::Framebuffer({{}, {_shadow_map_size, _shadow_map_size}});
                        if (_transparent_shadows)
                            _shadow_data[i].shadow_color_framebuffer = Magnum::GL::Framebuffer({{}, {_shadow_map_size, _shadow_map_size}});

                        if (!isPointLight) {
                            (_shadow_data[i].shadow_framebuffer)
                                .attachTextureLayer(Magnum::GL::Framebuffer::BufferAttachment::Depth, *_shadow_texture, 0, i)
                                .mapForDraw(Magnum::GL::Framebuffer::DrawAttachment::None)
                                .bind();
                            if (_transparent_shadows)
                                (_shadow_data[i].shadow_color_framebuffer)
                                    .attachTextureLayer(Magnum::GL::Framebuffer::ColorAttachment(0), *_shadow_color_texture, 0, i)
                                    .attachTextureLayer(Magnum::GL::Framebuffer::BufferAttachment::Depth, *_shadow_texture, 0, i)
                                    .bind();
                        }
                        else {
                            (_shadow_data[i].shadow_framebuffer)
                                .mapForDraw(Magnum::GL::Framebuffer::DrawAttachment::None)
                                .attachLayeredTexture(Magnum::GL::Framebuffer::BufferAttachment::Depth, *_shadow_cube_map, 0)
                                .bind();
                            if (_transparent_shadows)
                                (_shadow_data[i].shadow_color_framebuffer)
                                    .attachLayeredTexture(Magnum::GL::Framebuffer::ColorAttachment(0), *_shadow_color_cube_map, 0)
                                    // .attachLayeredTexture(Magnum::GL::Framebuffer::BufferAttachment::Depth, *_shadow_cube_map, 0)
                                    .bind();
                        }

                        /* Check if the creation of the framebuffer was successful */
                        if (!(_shadow_data[i].shadow_framebuffer.checkStatus(Magnum::GL::FramebufferTarget::Draw) == Magnum::GL::Framebuffer::Status::Complete)
                            || (_transparent_shadows && !(_shadow_data[i].shadow_color_framebuffer.checkStatus(Magnum::GL::FramebufferTarget::Draw) == Magnum::GL::Framebuffer::Status::Complete))) {
                            _shadowed = false;
                            break;
                        }
                    }
                }

                ROBOT_DART_WARNING(!_shadowed, "Something bad happened when configuring shadows! Disabling them!");
            }
        } // namespace magnum
    } // namespace gui
} // namespace robot_dart