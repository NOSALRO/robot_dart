#include <robot_dart/gui/magnum/drawables.hpp>
#include <robot_dart/utils.hpp>

#include <Magnum/GL/CubeMapTexture.h>
#include <Magnum/GL/DefaultFramebuffer.h>
#include <Magnum/GL/Mesh.h>
#include <Magnum/GL/Renderer.h>

#include <Magnum/GL/AbstractFramebuffer.h>
#include <Magnum/GL/GL.h>

namespace robot_dart {
    namespace gui {
        namespace magnum {
            // DrawableObject
            DrawableObject::DrawableObject(
                const std::vector<std::reference_wrapper<Magnum::GL::Mesh>>& meshes,
                const std::vector<gs::Material>& materials,
                gs::PhongMultiLight& color,
                gs::PhongMultiLight& texture,
                Object3D* parent,
                Magnum::SceneGraph::DrawableGroup3D* group)
                : Object3D{parent},
                  Magnum::SceneGraph::Drawable3D{*this, group},
                  _meshes{meshes},
                  _color_shader{color},
                  _texture_shader{texture},
                  _materials(materials)
            {
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

                _hasNegativeScaling.resize(_scalings.size());
                for (size_t i = 0; i < scalings.size(); i++) {
                    _hasNegativeScaling[i] = false;
                    for (size_t j = 0; j < 3; j++)
                        if (_scalings[i][j] < 0.f) {
                            _hasNegativeScaling[i] = true;
                            break;
                        }
                }

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
                    else if (_hasNegativeScaling[i])
                        Magnum::GL::Renderer::setFaceCullingMode(Magnum::GL::Renderer::PolygonFacing::Front);
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
                    else if (_hasNegativeScaling[i])
                        Magnum::GL::Renderer::setFaceCullingMode(Magnum::GL::Renderer::PolygonFacing::Back);
                }
            }

            // ShadowedObject
            ShadowedObject::ShadowedObject(
                const std::vector<std::reference_wrapper<Magnum::GL::Mesh>>& meshes,
                gs::ShadowMap& shader,
                gs::ShadowMap& texture_shader,
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
                gs::ShadowMapColor& shader,
                gs::ShadowMapColor& texture_shader,
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
                gs::CubeMap& shader,
                gs::CubeMap& texture_shader,
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
                gs::CubeMapColor& shader,
                gs::CubeMapColor& texture_shader,
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
        } // namespace magnum
    } // namespace gui
} // namespace robot_dart