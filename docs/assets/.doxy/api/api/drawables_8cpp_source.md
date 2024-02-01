

# File drawables.cpp

[**File List**](files.md) **>** [**gui**](dir_6a9d4b7ec29c938d1d9a486c655cfc8a.md) **>** [**magnum**](dir_5d18adecbc10cabf3ca51da31f2acdd1.md) **>** [**drawables.cpp**](drawables_8cpp.md)

[Go to the documentation of this file](drawables_8cpp.md)

```C++

#include <robot_dart/gui/magnum/drawables.hpp>
#include <robot_dart/gui_data.hpp>
#include <robot_dart/robot_dart_simu.hpp>
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
                RobotDARTSimu* simu,
                dart::dynamics::ShapeNode* shape,
                const std::vector<std::reference_wrapper<Magnum::GL::Mesh>>& meshes,
                const std::vector<gs::Material>& materials,
                gs::PhongMultiLight& color,
                gs::PhongMultiLight& texture,
                Object3D* parent,
                Magnum::SceneGraph::DrawableGroup3D* group)
                : Object3D{parent},
                  Magnum::SceneGraph::Drawable3D{*this, group},
                  _simu(simu),
                  _shape(shape),
                  _meshes{meshes},
                  _color_shader{color},
                  _texture_shader{texture},
                  _materials(materials)
            {
                _is_soft_body.resize(_meshes.size(), false);
            }

            DrawableObject& DrawableObject::set_meshes(const std::vector<std::reference_wrapper<Magnum::GL::Mesh>>& meshes)
            {
                _meshes = meshes;
                return *this;
            }

            DrawableObject& DrawableObject::set_materials(const std::vector<gs::Material>& materials)
            {
                _materials = materials;
                return *this;
            }

            DrawableObject& DrawableObject::set_soft_bodies(const std::vector<bool>& softBody)
            {
                _is_soft_body = softBody;
                return *this;
            }

            DrawableObject& DrawableObject::set_scalings(const std::vector<Magnum::Vector3>& scalings)
            {
                _scalings = scalings;

                _has_negative_scaling.resize(_scalings.size());
                for (size_t i = 0; i < scalings.size(); i++) {
                    _has_negative_scaling[i] = false;
                    for (size_t j = 0; j < 3; j++)
                        if (_scalings[i][j] < 0.f) {
                            _has_negative_scaling[i] = true;
                            break;
                        }
                }

                return *this;
            }

            DrawableObject& DrawableObject::set_transparent(bool transparent)
            {
                _isTransparent = transparent;
                return *this;
            }

            DrawableObject& DrawableObject::set_color_shader(std::reference_wrapper<gs::PhongMultiLight> shader)
            {
                _color_shader = shader;
                return *this;
            }

            DrawableObject& DrawableObject::set_texture_shader(std::reference_wrapper<gs::PhongMultiLight> shader)
            {
                _texture_shader = shader;
                return *this;
            }

            void DrawableObject::draw(const Magnum::Matrix4& transformationMatrix, Magnum::SceneGraph::Camera3D& camera)
            {
                for (size_t i = 0; i < _meshes.size(); i++) {
                    Magnum::GL::Mesh& mesh = _meshes[i];
                    Magnum::Matrix4 scalingMatrix = Magnum::Matrix4::scaling(_scalings[i]);
                    bool isColor = !_materials[i].has_diffuse_texture();
                    if (_is_soft_body[i])
                        Magnum::GL::Renderer::disable(Magnum::GL::Renderer::Feature::FaceCulling);
                    else if (_has_negative_scaling[i])
                        Magnum::GL::Renderer::setFaceCullingMode(Magnum::GL::Renderer::PolygonFacing::Front);
                    if (isColor) {
                        _color_shader.get()
                            .set_material(_materials[i])
                            .set_transformation_matrix(absoluteTransformationMatrix() * scalingMatrix)
                            .set_normal_matrix((transformationMatrix * scalingMatrix).rotationScaling())
                            .set_camera_matrix(camera.cameraMatrix())
                            .set_projection_matrix(camera.projectionMatrix())
                            .draw(mesh);
                    }
                    else {
                        _texture_shader.get()
                            .set_material(_materials[i])
                            .set_transformation_matrix(absoluteTransformationMatrix() * scalingMatrix)
                            .set_normal_matrix((transformationMatrix * scalingMatrix).rotationScaling())
                            .set_camera_matrix(camera.cameraMatrix())
                            .set_projection_matrix(camera.projectionMatrix())
                            .draw(mesh);
                    }

                    if (_is_soft_body[i])
                        Magnum::GL::Renderer::enable(Magnum::GL::Renderer::Feature::FaceCulling);
                    else if (_has_negative_scaling[i])
                        Magnum::GL::Renderer::setFaceCullingMode(Magnum::GL::Renderer::PolygonFacing::Back);
                }
            }

            // ShadowedObject
            ShadowedObject::ShadowedObject(
                RobotDARTSimu* simu,
                dart::dynamics::ShapeNode* shape,
                const std::vector<std::reference_wrapper<Magnum::GL::Mesh>>& meshes,
                gs::ShadowMap& shader,
                gs::ShadowMap& texture_shader,
                Object3D* parent,
                Magnum::SceneGraph::DrawableGroup3D* group)
                : Object3D{parent},
                  Magnum::SceneGraph::Drawable3D{*this, group},
                  _simu(simu),
                  _shape(shape),
                  _meshes{meshes},
                  _shader{shader},
                  _texture_shader(texture_shader) {}

            ShadowedObject& ShadowedObject::set_meshes(const std::vector<std::reference_wrapper<Magnum::GL::Mesh>>& meshes)
            {
                _meshes = meshes;
                return *this;
            }

            ShadowedObject& ShadowedObject::set_materials(const std::vector<gs::Material>& materials)
            {
                _materials = materials;
                return *this;
            }

            ShadowedObject& ShadowedObject::set_scalings(const std::vector<Magnum::Vector3>& scalings)
            {
                _scalings = scalings;
                return *this;
            }

            void ShadowedObject::draw(const Magnum::Matrix4& transformationMatrix, Magnum::SceneGraph::Camera3D& camera)
            {
                if (!_simu->gui_data()->cast_shadows(_shape))
                    return;
                for (size_t i = 0; i < _meshes.size(); i++) {
                    Magnum::GL::Mesh& mesh = _meshes[i];
                    Magnum::Matrix4 scalingMatrix = Magnum::Matrix4::scaling(_scalings[i]);
                    bool isColor = !_materials[i].has_diffuse_texture();
                    if (isColor) {
                        (_shader.get())
                            .set_transformation_matrix(transformationMatrix * scalingMatrix)
                            .set_projection_matrix(camera.projectionMatrix())
                            .set_material(_materials[i])
                            .draw(mesh);
                    }
                    else {
                        (_texture_shader.get())
                            .set_transformation_matrix(transformationMatrix * scalingMatrix)
                            .set_projection_matrix(camera.projectionMatrix())
                            .set_material(_materials[i])
                            .draw(mesh);
                    }
                }
            }

            // ShadowedColorObject
            ShadowedColorObject::ShadowedColorObject(
                RobotDARTSimu* simu,
                dart::dynamics::ShapeNode* shape,
                const std::vector<std::reference_wrapper<Magnum::GL::Mesh>>& meshes,
                gs::ShadowMapColor& shader,
                gs::ShadowMapColor& texture_shader,
                Object3D* parent,
                Magnum::SceneGraph::DrawableGroup3D* group)
                : Object3D{parent},
                  Magnum::SceneGraph::Drawable3D{*this, group},
                  _simu(simu),
                  _shape(shape),
                  _meshes{meshes},
                  _shader{shader},
                  _texture_shader(texture_shader) {}

            ShadowedColorObject& ShadowedColorObject::set_meshes(const std::vector<std::reference_wrapper<Magnum::GL::Mesh>>& meshes)
            {
                _meshes = meshes;
                return *this;
            }

            ShadowedColorObject& ShadowedColorObject::set_materials(const std::vector<gs::Material>& materials)
            {
                _materials = materials;
                return *this;
            }

            ShadowedColorObject& ShadowedColorObject::set_scalings(const std::vector<Magnum::Vector3>& scalings)
            {
                _scalings = scalings;
                return *this;
            }

            void ShadowedColorObject::draw(const Magnum::Matrix4& transformationMatrix, Magnum::SceneGraph::Camera3D& camera)
            {
                if (!_simu->gui_data()->cast_shadows(_shape))
                    return;
                for (size_t i = 0; i < _meshes.size(); i++) {
                    Magnum::GL::Mesh& mesh = _meshes[i];
                    Magnum::Matrix4 scalingMatrix = Magnum::Matrix4::scaling(_scalings[i]);
                    bool isColor = !_materials[i].has_diffuse_texture();
                    if (isColor) {
                        (_shader.get())
                            .set_transformation_matrix(transformationMatrix * scalingMatrix)
                            .set_projection_matrix(camera.projectionMatrix())
                            .set_material(_materials[i])
                            .draw(mesh);
                    }
                    else {
                        (_texture_shader.get())
                            .set_transformation_matrix(transformationMatrix * scalingMatrix)
                            .set_projection_matrix(camera.projectionMatrix())
                            .set_material(_materials[i])
                            .draw(mesh);
                    }
                }
            }

            // CubeMapShadowedObject
            CubeMapShadowedObject::CubeMapShadowedObject(
                RobotDARTSimu* simu,
                dart::dynamics::ShapeNode* shape,
                const std::vector<std::reference_wrapper<Magnum::GL::Mesh>>& meshes,
                gs::CubeMap& shader,
                gs::CubeMap& texture_shader,
                Object3D* parent,
                Magnum::SceneGraph::DrawableGroup3D* group)
                : Object3D{parent},
                  Magnum::SceneGraph::Drawable3D{*this, group},
                  _simu(simu),
                  _shape(shape),
                  _meshes{meshes},
                  _shader{shader},
                  _texture_shader(texture_shader) {}

            CubeMapShadowedObject& CubeMapShadowedObject::set_meshes(const std::vector<std::reference_wrapper<Magnum::GL::Mesh>>& meshes)
            {
                _meshes = meshes;
                return *this;
            }

            CubeMapShadowedObject& CubeMapShadowedObject::set_materials(const std::vector<gs::Material>& materials)
            {
                _materials = materials;
                return *this;
            }

            CubeMapShadowedObject& CubeMapShadowedObject::set_scalings(const std::vector<Magnum::Vector3>& scalings)
            {
                _scalings = scalings;
                return *this;
            }

            void CubeMapShadowedObject::draw(const Magnum::Matrix4&, Magnum::SceneGraph::Camera3D&)
            {
                for (size_t i = 0; i < _meshes.size(); i++) {
                    Magnum::GL::Mesh& mesh = _meshes[i];
                    Magnum::Matrix4 scalingMatrix = Magnum::Matrix4::scaling(_scalings[i]);
                    bool isColor = !_materials[i].has_diffuse_texture();
                    if (isColor) {
                        (_shader.get())
                            .set_transformation_matrix(absoluteTransformation() * scalingMatrix)
                            .set_material(_materials[i])
                            .draw(mesh);
                    }
                    else {
                        (_texture_shader.get())
                            .set_transformation_matrix(absoluteTransformation() * scalingMatrix)
                            .set_material(_materials[i])
                            .draw(mesh);
                    }
                }
            }

            // CubeMapShadowedColorObject
            CubeMapShadowedColorObject::CubeMapShadowedColorObject(
                RobotDARTSimu* simu,
                dart::dynamics::ShapeNode* shape,
                const std::vector<std::reference_wrapper<Magnum::GL::Mesh>>& meshes,
                gs::CubeMapColor& shader,
                gs::CubeMapColor& texture_shader,
                Object3D* parent,
                Magnum::SceneGraph::DrawableGroup3D* group)
                : Object3D{parent},
                  Magnum::SceneGraph::Drawable3D{*this, group},
                  _simu(simu),
                  _shape(shape),
                  _meshes{meshes},
                  _shader{shader},
                  _texture_shader(texture_shader) {}

            CubeMapShadowedColorObject& CubeMapShadowedColorObject::set_meshes(const std::vector<std::reference_wrapper<Magnum::GL::Mesh>>& meshes)
            {
                _meshes = meshes;
                return *this;
            }

            CubeMapShadowedColorObject& CubeMapShadowedColorObject::set_materials(const std::vector<gs::Material>& materials)
            {
                _materials = materials;
                return *this;
            }

            CubeMapShadowedColorObject& CubeMapShadowedColorObject::set_scalings(const std::vector<Magnum::Vector3>& scalings)
            {
                _scalings = scalings;
                return *this;
            }

            void CubeMapShadowedColorObject::draw(const Magnum::Matrix4&, Magnum::SceneGraph::Camera3D&)
            {
                if (!_simu->gui_data()->cast_shadows(_shape))
                    return;
                for (size_t i = 0; i < _meshes.size(); i++) {
                    Magnum::GL::Mesh& mesh = _meshes[i];
                    Magnum::Matrix4 scalingMatrix = Magnum::Matrix4::scaling(_scalings[i]);
                    bool isColor = !_materials[i].has_diffuse_texture();
                    if (isColor) {
                        (_shader.get())
                            .set_transformation_matrix(absoluteTransformation() * scalingMatrix)
                            .set_material(_materials[i])
                            .draw(mesh);
                    }
                    else {
                        (_texture_shader.get())
                            .set_transformation_matrix(absoluteTransformation() * scalingMatrix)
                            .set_material(_materials[i])
                            .draw(mesh);
                    }
                }
            }
        } // namespace magnum
    } // namespace gui
} // namespace robot_dart

```

