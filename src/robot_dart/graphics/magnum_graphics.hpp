#ifndef ROBOT_DART_GRAPHICS_MAGNUM_GRAPHICS_HPP
#define ROBOT_DART_GRAPHICS_MAGNUM_GRAPHICS_HPP

#include <robot_dart/graphics/base_graphics.hpp>

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

    class ColoredObject : public Object3D, SceneGraph::Drawable3D {
    public:
        explicit ColoredObject(GL::Mesh* mesh, const MaterialData& material, Object3D* parent, SceneGraph::DrawableGroup3D* group)
            : Object3D{parent}, SceneGraph::Drawable3D{*this, group}, _mesh{mesh}, _shader{ViewerResourceManager::instance().get<Shaders::Phong>("color")}, _material(material)
        {
            _light0Position = Vector3{0.f, 2.f, 3.f};
            _light1Position = Vector3{0.f, -2.f, 3.f};
        }

        ColoredObject& setMesh(GL::Mesh* mesh)
        {
            _mesh = mesh;
            return *this;
        }

        ColoredObject& setMaterial(const MaterialData& material)
        {
            _material = material;
            return *this;
        }

        ColoredObject& setSoftBody(bool softBody = true)
        {
            _isSoftBody = softBody;
            return *this;
        }

        ColoredObject& setLight0Position(const Vector3& position)
        {
            _light0Position = position;
            return *this;
        }

        ColoredObject& setLight1Position(const Vector3& position)
        {
            _light1Position = position;
            return *this;
        }

    private:
        void draw(const Matrix4& transformationMatrix, SceneGraph::Camera3D& camera) override
        {
            Matrix4 scalingMatrix = Matrix4::scaling(_material._scaling);
            _shader->setAmbientColor(_material._ambientColor)
                .setDiffuseColor(_material._diffuseColor)
                .setSpecularColor(_material._specularColor)
                .setShininess(_material._shininess)
                .setLightPosition(0, camera.cameraMatrix().transformPoint(_light0Position))
                .setLightPosition(1, camera.cameraMatrix().transformPoint(_light1Position))
                .setTransformationMatrix(transformationMatrix * scalingMatrix)
                .setNormalMatrix((transformationMatrix * scalingMatrix).rotation())
                .setProjectionMatrix(camera.projectionMatrix());

            if (_isSoftBody)
                GL::Renderer::disable(GL::Renderer::Feature::FaceCulling);
            _mesh->draw(*_shader);
            if (_isSoftBody)
                GL::Renderer::enable(GL::Renderer::Feature::FaceCulling);
        }

        GL::Mesh* _mesh;
        Resource<Shaders::Phong> _shader;
        MaterialData _material;
        bool _isSoftBody = false;
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
            for (auto& n : _coloredObjects) {
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
            for (auto& object : _dartWorld->updatedShapeObjects()) {
                /* Get material information */
                MaterialData mat;
                mat._ambientColor = object.get().drawData().materials[0].ambientColor().rgb();
                mat._diffuseColor = object.get().drawData().materials[0].diffuseColor().rgb();
                mat._specularColor = object.get().drawData().materials[0].specularColor().rgb();
                mat._shininess = object.get().drawData().materials[0].shininess();
                mat._scaling = object.get().drawData().scaling;

                /* Get the modified mesh */
                GL::Mesh* mesh = &object.get().drawData().meshes[0];

                /* Check if we already have it */
                auto it = _coloredObjects.insert(std::make_pair(&object.get(), nullptr));
                if (it.second) {
                    /* If not, create a new object and add it to our drawables list */
                    auto coloredObj = new ColoredObject(mesh, mat, static_cast<Object3D*>(&(object.get().object())), &_drawables);
                    if (object.get().shapeNode()->getShape()->getType() == dart::dynamics::SoftMeshShape::getStaticType())
                        coloredObj->setSoftBody();
                    it.first->second = coloredObj;
                }
                else {
                    /* Otherwise, update the mesh and the material data */
                    it.first->second->setMesh(mesh).setMaterial(mat);
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
        std::unordered_map<DartIntegration::Object*, ColoredObject*> _coloredObjects;
        std::vector<Object3D*> _dartObjs;
    };
} // namespace Magnum

namespace robot_dart {
    namespace graphics {
        class MagnumGraphics : public BaseGraphics {
        public:
            MagnumGraphics(const dart::simulation::WorldPtr& world, unsigned int width = 640, unsigned int height = 480, bool shadowed = true) : _world(world), _width(width), _height(height), _frame_counter(0), _enabled(true)
            {
                int argc = 0;
                char** argv = NULL;
                _magnum_app = std::unique_ptr<Magnum::DartApplication>(new Magnum::DartApplication({argc, argv}, world, width, height));
                set_render_period(world->getTimeStep());
            }

            ~MagnumGraphics() {}

            bool done() const override
            {
                return _magnum_app->done();
            }

            void refresh() override
            {
                if (!_enabled)
                    return;

                // process next frame
                if (_frame_counter % _render_period == 0)
                    _magnum_app->mainLoopIteration();
                _frame_counter++;
            }

            void set_render_period(double dt) override
            {
                // we want to display at around 60Hz of simulated time
                _render_period = std::floor(0.015 / dt);
                if (_render_period < 1)
                    _render_period = 1;
            }

            void set_enable(bool enable) override
            {
                _enabled = enable;
            }

            void look_at(const Eigen::Vector3d& camera_pos,
                const Eigen::Vector3d& look_at = Eigen::Vector3d(0, 0, 0),
                const Eigen::Vector3d& up = Eigen::Vector3d(0, 0, 1))
            {
                _magnum_app->lookAt(camera_pos, look_at, up);
            }

        protected:
            std::unique_ptr<Magnum::DartIntegration::World> _dartWorld;
            Eigen::Vector3d _look_at;
            Eigen::Vector3d _camera_pos;
            Eigen::Vector3d _camera_up;
            dart::simulation::WorldPtr _world;
            size_t _render_period, _width, _height, _frame_counter;
            bool _enabled;

            std::unique_ptr<Magnum::DartApplication> _magnum_app;
        };
    } // namespace graphics
} // namespace robot_dart

#endif
