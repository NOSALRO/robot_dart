#ifndef ROBOT_DART_GUI_MAGNUM_DART_APPLICATION_HPP
#define ROBOT_DART_GUI_MAGNUM_DART_APPLICATION_HPP

#include <dart/dynamics/SoftBodyNode.hpp>
#include <dart/dynamics/SoftMeshShape.hpp>

#include <Corrade/Utility/Directory.h>

#include <Magnum/GL/Buffer.h>
#include <Magnum/GL/DefaultFramebuffer.h>
#include <Magnum/GL/Mesh.h>
#include <Magnum/GL/OpenGLTester.h>
#include <Magnum/GL/Texture.h>
#include <Magnum/Platform/Sdl2Application.h>
#include <Magnum/ResourceManager.h>
#include <Magnum/SceneGraph/Camera.h>
#include <Magnum/SceneGraph/Drawable.h>
#include <Magnum/SceneGraph/MatrixTransformation3D.h>
#include <Magnum/SceneGraph/Object.hpp>
#include <Magnum/SceneGraph/SceneGraph.h>
#include <Magnum/Shaders/Phong.h>
#include <Magnum/Timeline.h>
#include <Magnum/Trade/PhongMaterialData.h>

#include <Magnum/DartIntegration/ConvertShapeNode.h>
#include <Magnum/DartIntegration/World.h>

namespace Magnum {
    using ViewerResourceManager = ResourceManager<GL::Buffer, GL::Mesh, Shaders::Phong>;
    using Object3D = SceneGraph::Object<SceneGraph::MatrixTransformation3D>;
    using Scene3D = SceneGraph::Scene<SceneGraph::MatrixTransformation3D>;

    struct MaterialData {
        Vector3 _ambientColor,
            _diffuseColor,
            _specularColor;
        Float _shininess;
        Vector3 _scaling;
    };

    class DrawableObject : public Object3D, SceneGraph::Drawable3D {
    public:
        explicit DrawableObject(const std::vector<std::reference_wrapper<GL::Mesh>>& meshes, const std::vector<MaterialData>& materials, Object3D* parent, SceneGraph::DrawableGroup3D* group)
            : Object3D{parent}, SceneGraph::Drawable3D{*this, group}, _meshes{meshes}, _color_shader{ViewerResourceManager::instance().get<Shaders::Phong>("color")}, _texture_shader{ViewerResourceManager::instance().get<Shaders::Phong>("texture")}, _materials(materials)
        {
            assert(_materials.size() >= meshes.size());
            _isSoftBody.resize(_meshes.size(), false);
            _textures.resize(_meshes.size());

            _light0Position = Vector3{0.f, 2.f, 3.f};
            _light1Position = Vector3{0.f, -2.f, 3.f};
        }

        DrawableObject& setMesh(size_t i, GL::Mesh& mesh)
        {
            assert(i < _meshes.size());
            _meshes[i] = mesh;
            return *this;
        }

        DrawableObject& setMeshes(const std::vector<std::reference_wrapper<GL::Mesh>>& meshes)
        {
            _meshes = meshes;
            return *this;
        }

        DrawableObject& setMaterial(size_t i, const MaterialData& material)
        {
            assert(i < _materials.size());
            _materials[i] = material;
            return *this;
        }

        DrawableObject& setMaterials(const std::vector<MaterialData>& materials)
        {
            _materials = materials;
            return *this;
        }

        DrawableObject& setSoftBody(size_t i, bool softBody = true)
        {
            assert(i < _isSoftBody.size());
            _isSoftBody[i] = softBody;
            return *this;
        }

        DrawableObject& setSoftBodies(const std::vector<bool>& softBody)
        {
            _isSoftBody = softBody;
            return *this;
        }

        DrawableObject& setTextures(std::vector<Containers::Optional<GL::Texture2D>>& textures)
        {
            _textures = std::move(textures);
            return *this;
        }

        DrawableObject& setTexture(size_t i, Containers::Optional<GL::Texture2D>& texture)
        {
            _textures[i] = std::move(texture);
            return *this;
        }

        DrawableObject& setLight0Position(const Vector3& position)
        {
            _light0Position = position;
            return *this;
        }

        DrawableObject& setLight1Position(const Vector3& position)
        {
            _light1Position = position;
            return *this;
        }

    private:
        void draw(const Matrix4& transformationMatrix, SceneGraph::Camera3D& camera) override
        {
            for (size_t i = 0; i < _meshes.size(); i++) {
                GL::Mesh& mesh = _meshes[i];
                Matrix4 scalingMatrix = Matrix4::scaling(_materials[i]._scaling);
                bool isColor = !_textures[i];
                if (isColor) {
                    _color_shader->setAmbientColor(_materials[i]._ambientColor)
                        .setDiffuseColor(_materials[i]._diffuseColor)
                        .setSpecularColor(_materials[i]._specularColor)
                        .setShininess(_materials[i]._shininess)
                        .setLightPosition(0, camera.cameraMatrix().transformPoint(_light0Position))
                        .setLightPosition(1, camera.cameraMatrix().transformPoint(_light1Position))
                        .setTransformationMatrix(transformationMatrix * scalingMatrix)
                        .setNormalMatrix((transformationMatrix * scalingMatrix).rotation())
                        .setProjectionMatrix(camera.projectionMatrix());
                }
                else {
                    _texture_shader->setAmbientColor(_materials[i]._ambientColor)
                        .bindDiffuseTexture(*_textures[i])
                        .setSpecularColor(_materials[i]._specularColor)
                        .setShininess(_materials[i]._shininess)
                        .setLightPosition(0, camera.cameraMatrix().transformPoint(_light0Position))
                        .setLightPosition(1, camera.cameraMatrix().transformPoint(_light1Position))
                        .setTransformationMatrix(transformationMatrix * scalingMatrix)
                        .setNormalMatrix((transformationMatrix * scalingMatrix).rotation())
                        .setProjectionMatrix(camera.projectionMatrix());
                }

                if (_isSoftBody[i])
                    GL::Renderer::disable(GL::Renderer::Feature::FaceCulling);
                if (isColor)
                    mesh.draw(*_color_shader);
                else
                    mesh.draw(*_texture_shader);
                if (_isSoftBody[i])
                    GL::Renderer::enable(GL::Renderer::Feature::FaceCulling);
            }
        }

        std::vector<std::reference_wrapper<GL::Mesh>> _meshes;
        Resource<Shaders::Phong> _color_shader;
        Resource<Shaders::Phong> _texture_shader;
        std::vector<MaterialData> _materials;
        std::vector<bool> _isSoftBody;
        std::vector<Containers::Optional<GL::Texture2D>> _textures;
        Vector3 _light0Position, _light1Position;
    };

    class DartApplication : public Platform::Application {
    public:
        explicit DartApplication(const Arguments& arguments, const dart::simulation::WorldPtr& world, size_t width, size_t height) : Platform::Application(arguments, NoCreate)
        {
            /* Try 16x MSAA */
            Configuration conf;
            GLConfiguration glConf;
            conf.setTitle("Magnum Dart Integration Example");
            conf.setSize({static_cast<int>(width), static_cast<int>(height)});
            glConf.setSampleCount(8);
            if (!tryCreate(conf, glConf))
                create(conf, glConf.setSampleCount(0));

            GL::Renderer::enable(GL::Renderer::Feature::DepthTest);
            GL::Renderer::enable(GL::Renderer::Feature::FaceCulling);

            /* Camera setup */
            (_cameraRig = new Object3D(&_scene));
            (_cameraObject = new Object3D(_cameraRig));
            (_camera = new SceneGraph::Camera3D(*_cameraObject))
                ->setAspectRatioPolicy(SceneGraph::AspectRatioPolicy::Extend)
                .setProjectionMatrix(Matrix4::perspectiveProjection(Deg(35.0f), 1.0f, 0.001f, 100.0f))
                .setViewport(GL::defaultFramebuffer.viewport().size());
            /* DART has +Z-axis as up direction*/
            /* Default look at */
            _cameraObject->setTransformation(Magnum::Matrix4::lookAt(Vector3{0.f, 3.f, 1.5f}, Vector3{0.f, 0.f, 0.5f}, Vector3{0.f, 0.f, 1.f}));

            /* Create our DARTIntegration object/world */
            auto dartObj = new Object3D{&_scene};
            _dartWorld.reset(new DartIntegration::World(*dartObj, *world));

            /* Phong shader instance */
            _resourceManager.set("color", new Shaders::Phong({}, 2));
            _resourceManager.set("texture", new Shaders::Phong{Shaders::Phong::Flag::DiffuseTexture, 2});

            /* Loop at 60 Hz max */
            setSwapInterval(1);
            setMinimalLoopPeriod(16);

            redraw();
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
                Magnum::Matrix4::lookAt(Vector3{cx, cy, cz},
                    Vector3{lx, ly, lz},
                    Vector3{ux, uy, uz}));
        }

        void setLightPosition(size_t index, const Eigen::Vector3d& position)
        {
            Vector3 pos = {static_cast<float>(position[0]), static_cast<float>(position[1]), static_cast<float>(position[2])};
            for (auto& n : _drawableObjects) {
                if (index == 0)
                    n.second->setLight0Position(pos);
                else
                    n.second->setLight1Position(pos);
            }
        }

    private:
        void viewportEvent(const Vector2i& size) override
        {
            GL::defaultFramebuffer.setViewport({{}, size});

            _camera->setViewport(size);
        }

        void drawEvent() override
        {
            GL::defaultFramebuffer.clear(
                GL::FramebufferClear::Color | GL::FramebufferClear::Depth);

            /* Update graphic meshes/materials and render */
            updateGraphics();
            _camera->draw(_drawables);

            swapBuffers();
            redraw();
        }

        void keyPressEvent(KeyEvent& event) override
        {
            if (event.key() == KeyEvent::Key::Down)
                _cameraObject->rotateX(Deg(5.0f));
            else if (event.key() == KeyEvent::Key::Up)
                _cameraObject->rotateX(Deg(-5.0f));
            else if (event.key() == KeyEvent::Key::Left)
                _cameraRig->rotateY(Deg(-5.0f));
            else if (event.key() == KeyEvent::Key::Right)
                _cameraRig->rotateY(Deg(5.0f));
            else
                return;

            event.setAccepted();
        }

        void exitEvent(ExitEvent& event) override
        {
            _done = true;
            event.setAccepted();
        }

        void updateGraphics()
        {
            /* We refresh the graphical models at 60Hz */
            _dartWorld->refresh();

            /* For each update object */
            for (DartIntegration::Object& object : _dartWorld->updatedShapeObjects()) {
                /* Get material information */
                std::vector<MaterialData> materials;
                std::vector<std::reference_wrapper<GL::Mesh>> meshes;
                std::vector<bool> isSoftBody;
                std::vector<Containers::Optional<GL::Texture2D>> textures;

                for (size_t i = 0; i < object.drawData().meshes.size(); i++) {
                    bool isColor = true;

                    if (object.drawData().materials[i].flags() & Trade::PhongMaterialData::Flag::DiffuseTexture) {
                        textures.push_back(std::move(object.drawData().textures[object.drawData().materials[i].diffuseTexture()]));
                        isColor = false;
                    }
                    else
                        textures.push_back({});

                    MaterialData mat;
                    mat._ambientColor = object.drawData().materials[i].ambientColor().rgb();
                    if (isColor)
                        mat._diffuseColor = object.drawData().materials[i].diffuseColor().rgb();
                    mat._specularColor = object.drawData().materials[i].specularColor().rgb();
                    mat._shininess = object.drawData().materials[i].shininess();
                    mat._scaling = object.drawData().scaling;

                    /* Get the modified mesh */
                    GL::Mesh& mesh = object.drawData().meshes[i];

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
                    auto drawableObject = new DrawableObject(meshes, materials, static_cast<Object3D*>(&(object.object())), &_drawables);
                    drawableObject->setSoftBodies(isSoftBody);
                    drawableObject->setTextures(textures);
                    it.first->second = drawableObject;
                }
                else {
                    /* Otherwise, update the mesh and the material data */
                    it.first->second->setMeshes(meshes).setMaterials(materials).setSoftBodies(isSoftBody).setTextures(textures);
                }
            }

            _dartWorld->clearUpdatedShapeObjects();
        }

        ViewerResourceManager _resourceManager;

        Scene3D _scene;
        SceneGraph::DrawableGroup3D _drawables;
        SceneGraph::Camera3D* _camera;

        Object3D *_cameraRig, *_cameraObject;

        bool _done = false;

        /* DART */
        std::unique_ptr<Magnum::DartIntegration::World> _dartWorld;
        std::unordered_map<DartIntegration::Object*, DrawableObject*> _drawableObjects;
        std::vector<Object3D*> _dartObjs;
    };
} // namespace Magnum

#endif