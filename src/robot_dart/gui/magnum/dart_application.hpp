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

#include <robot_dart/gui/magnum/gs/phong_multi_light.hpp>

namespace Magnum {
    using namespace robot_dart::gui;

    using ViewerResourceManager = ResourceManager<GL::Buffer, GL::Mesh, Shaders::Phong, magnum::gs::PhongMultiLight>;
    using Object3D = SceneGraph::Object<SceneGraph::MatrixTransformation3D>;
    using Scene3D = SceneGraph::Scene<SceneGraph::MatrixTransformation3D>;

    class DrawableObject : public Object3D, SceneGraph::Drawable3D {
    public:
        explicit DrawableObject(const std::vector<std::reference_wrapper<GL::Mesh>>& meshes, const std::vector<magnum::gs::Material>& materials, Object3D* parent, SceneGraph::DrawableGroup3D* group)
            : Object3D{parent},
              SceneGraph::Drawable3D{*this, group},
              _meshes{meshes},
              _color_shader{ViewerResourceManager::instance().get<magnum::gs::PhongMultiLight>("multi-color")},
              _texture_shader{ViewerResourceManager::instance().get<magnum::gs::PhongMultiLight>("multi-texture")},
              _materials(materials)
        {
            assert(_materials.size() >= meshes.size());
            _isSoftBody.resize(_meshes.size(), false);
        }

        DrawableObject& setMeshes(const std::vector<std::reference_wrapper<GL::Mesh>>& meshes)
        {
            _meshes = meshes;
            return *this;
        }

        DrawableObject& setMaterials(const std::vector<magnum::gs::Material>& materials)
        {
            _materials = materials;
            return *this;
        }

        DrawableObject& setSoftBodies(const std::vector<bool>& softBody)
        {
            _isSoftBody = softBody;
            return *this;
        }

        DrawableObject& setScalings(const std::vector<Vector3>& scalings)
        {
            _scalings = scalings;
            return *this;
        }

    private:
        void draw(const Matrix4& transformationMatrix, SceneGraph::Camera3D& camera) override
        {
            for (size_t i = 0; i < _meshes.size(); i++) {
                GL::Mesh& mesh = _meshes[i];
                Matrix4 scalingMatrix = Matrix4::scaling(_scalings[i]);
                bool isColor = !_materials[i].hasDiffuseTexture();
                if (isColor) {
                    _color_shader->setMaterial(_materials[i])
                        .setTransformationMatrix(transformationMatrix * scalingMatrix)
                        .setNormalMatrix((transformationMatrix * scalingMatrix).rotation())
                        .setProjectionMatrix(camera.projectionMatrix());
                }
                else {
                    _texture_shader->setMaterial(_materials[i])
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
        Resource<magnum::gs::PhongMultiLight> _color_shader;
        Resource<magnum::gs::PhongMultiLight> _texture_shader;
        std::vector<magnum::gs::Material> _materials;
        std::vector<Vector3> _scalings;
        std::vector<bool> _isSoftBody;
    };

    class DartApplication : public Platform::Application {
    public:
        explicit DartApplication(
            const Arguments& arguments, const dart::simulation::WorldPtr& world, size_t width, size_t height, const std::string& title = "DART")
            : Platform::Application(arguments, NoCreate)
        {
            /* Try 16x MSAA */
            Configuration conf;
            GLConfiguration glConf;
            conf.setTitle(title);
            conf.setSize({static_cast<int>(width), static_cast<int>(height)});
            conf.setWindowFlags(Configuration::WindowFlag::Resizable);
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
            _cameraObject->setTransformation(Magnum::Matrix4::lookAt(Vector3{0.f, 2.f, 1.5f}, Vector3{0.f, 0.f, 0.5f}, Vector3{0.f, 0.f, 1.f}));

            /* Create our DARTIntegration object/world */
            auto dartObj = new Object3D{&_scene};
            _dartWorld.reset(new DartIntegration::World(*dartObj, *world));

            /* Phong shader instance */
            _resourceManager.set("multi-color", new magnum::gs::PhongMultiLight{{}, 10});
            _resourceManager.set("multi-texture", new magnum::gs::PhongMultiLight{{magnum::gs::PhongMultiLight::Flag::DiffuseTexture}, 10});

            /* Add default lights (2 directional lights) */
            magnum::gs::Material mat;
            mat.diffuseColor() = {1.f, 1.f, 1.f, 1.f};
            mat.specularColor() = {1.f, 1.f, 1.f, 1.f};
            // magnum::gs::Light light = magnum::gs::createPointLight({0.f, 0.f, 2.f}, mat, 2.f, {0.f, 0.f, 1.f});
            // magnum::gs::Light light = magnum::gs::createSpotLight(
            //     {0.f, 0.f, 3.f}, mat, {0.f, 0.f, -1.f}, 1.f, Magnum::Math::Constants<Magnum::Float>::piHalf() / 5.f, 2.f, {0.f, 0.f, 1.f});
            Vector3 dir = {-1.f, -1.f, -1.f};
            magnum::gs::Light light = magnum::gs::createDirectionalLight(dir, mat);
            _lights.push_back(light);
            dir = {1.f, 1.f, -1.f};
            light = magnum::gs::createDirectionalLight(dir, mat);
            _lights.push_back(light);

            /* Change default clear color to black */
            GL::Renderer::setClearColor(Vector4{0.f, 0.f, 0.f, 1.f});

            /* Loop at 60 Hz max */
            setSwapInterval(1);
            setMinimalLoopPeriod(16);

            redraw();
        }

        void clearLights()
        {
            _lights.clear();
            /* Reset lights in shaders */
            magnum::gs::Light light;
            for (int i = 0; i < ViewerResourceManager::instance().get<magnum::gs::PhongMultiLight>("multi-color")->maxLights(); i++)
                ViewerResourceManager::instance().get<magnum::gs::PhongMultiLight>("multi-color")->setLight(i, light);
            for (int i = 0; i < ViewerResourceManager::instance().get<magnum::gs::PhongMultiLight>("multi-texture")->maxLights(); i++)
                ViewerResourceManager::instance().get<magnum::gs::PhongMultiLight>("multi-texture")->setLight(i, light);
        }

        void addLight(const magnum::gs::Light& light)
        {
            _lights.push_back(light);
        }

        magnum::gs::Light& light(size_t i)
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
                Magnum::Matrix4::lookAt(Vector3{cx, cy, cz},
                    Vector3{lx, ly, lz},
                    Vector3{ux, uy, uz}));
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

                ViewerResourceManager::instance().get<magnum::gs::PhongMultiLight>("multi-color")->setLight(i, _lights[i]);
                ViewerResourceManager::instance().get<magnum::gs::PhongMultiLight>("multi-texture")->setLight(i, _lights[i]);
            }
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
                std::vector<magnum::gs::Material> materials;
                std::vector<std::reference_wrapper<GL::Mesh>> meshes;
                std::vector<bool> isSoftBody;
                // std::vector<Containers::Optional<GL::Texture2D>> textures;
                std::vector<Vector3> scalings;

                for (size_t i = 0; i < object.drawData().meshes.size(); i++) {
                    bool isColor = true;
                    magnum::gs::Material mat;

                    if (object.drawData().materials[i].flags() & Trade::PhongMaterialData::Flag::DiffuseTexture) {
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
                    drawableObject->setScalings(scalings);
                    it.first->second = drawableObject;
                }
                else {
                    /* Otherwise, update the mesh and the material data */
                    it.first->second->setMeshes(meshes).setMaterials(materials).setSoftBodies(isSoftBody).setScalings(scalings);
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
        std::vector<magnum::gs::Light> _lights;
    };
} // namespace Magnum

#endif