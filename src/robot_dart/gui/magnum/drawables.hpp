#ifndef ROBOT_DART_GUI_MAGNUM_DRAWABLES_HPP
#define ROBOT_DART_GUI_MAGNUM_DRAWABLES_HPP

#include <robot_dart/gui/helper.hpp>
#include <robot_dart/gui/magnum/gs/cube_map.hpp>
#include <robot_dart/gui/magnum/gs/cube_map_color.hpp>
#include <robot_dart/gui/magnum/gs/phong_multi_light.hpp>
#include <robot_dart/gui/magnum/gs/shadow_map.hpp>
#include <robot_dart/gui/magnum/gs/shadow_map_color.hpp>
#include <robot_dart/gui/magnum/types.hpp>

#include <Magnum/GL/Framebuffer.h>

#include <Magnum/SceneGraph/Drawable.h>

namespace robot_dart {
    namespace gui {
        namespace magnum {
            class DrawableObject : public Object3D, Magnum::SceneGraph::Drawable3D {
            public:
                explicit DrawableObject(
                    const std::vector<std::reference_wrapper<Magnum::GL::Mesh>>& meshes,
                    const std::vector<gs::Material>& materials,
                    gs::PhongMultiLight& color,
                    gs::PhongMultiLight& texture,
                    Object3D* parent,
                    Magnum::SceneGraph::DrawableGroup3D* group);

                DrawableObject& setMeshes(const std::vector<std::reference_wrapper<Magnum::GL::Mesh>>& meshes);
                DrawableObject& setMaterials(const std::vector<gs::Material>& materials);
                DrawableObject& setSoftBodies(const std::vector<bool>& softBody);
                DrawableObject& setScalings(const std::vector<Magnum::Vector3>& scalings);
                DrawableObject& setTransparent(bool transparent = true);

                DrawableObject& setColorShader(std::reference_wrapper<gs::PhongMultiLight> shader);
                DrawableObject& setTextureShader(std::reference_wrapper<gs::PhongMultiLight> shader);

                const std::vector<gs::Material>& materials() const { return _materials; }
                bool isTransparent() const { return _isTransparent; }

            private:
                void draw(const Magnum::Matrix4& transformationMatrix, Magnum::SceneGraph::Camera3D& camera) override;

                std::vector<std::reference_wrapper<Magnum::GL::Mesh>> _meshes;
                std::reference_wrapper<gs::PhongMultiLight> _color_shader;
                std::reference_wrapper<gs::PhongMultiLight> _texture_shader;
                std::vector<gs::Material> _materials;
                std::vector<Magnum::Vector3> _scalings;
                std::vector<bool> _hasNegativeScaling;
                std::vector<bool> _isSoftBody;
                bool _isTransparent;
            };

            class ShadowedObject : public Object3D, Magnum::SceneGraph::Drawable3D {
            public:
                explicit ShadowedObject(
                    const std::vector<std::reference_wrapper<Magnum::GL::Mesh>>& meshes,
                    gs::ShadowMap& shader,
                    gs::ShadowMap& texture_shader,
                    Object3D* parent,
                    Magnum::SceneGraph::DrawableGroup3D* group);

                ShadowedObject& setMeshes(const std::vector<std::reference_wrapper<Magnum::GL::Mesh>>& meshes);
                ShadowedObject& setMaterials(const std::vector<gs::Material>& materials);
                ShadowedObject& setScalings(const std::vector<Magnum::Vector3>& scalings);

            private:
                void draw(const Magnum::Matrix4& transformationMatrix, Magnum::SceneGraph::Camera3D& camera) override;

                std::vector<std::reference_wrapper<Magnum::GL::Mesh>> _meshes;
                std::reference_wrapper<gs::ShadowMap> _shader, _texture_shader;
                std::vector<gs::Material> _materials;
                std::vector<Magnum::Vector3> _scalings;
            };

            class ShadowedColorObject : public Object3D, Magnum::SceneGraph::Drawable3D {
            public:
                explicit ShadowedColorObject(
                    const std::vector<std::reference_wrapper<Magnum::GL::Mesh>>& meshes,
                    gs::ShadowMapColor& shader,
                    gs::ShadowMapColor& texture_shader,
                    Object3D* parent,
                    Magnum::SceneGraph::DrawableGroup3D* group);

                ShadowedColorObject& setMeshes(const std::vector<std::reference_wrapper<Magnum::GL::Mesh>>& meshes);
                ShadowedColorObject& setMaterials(const std::vector<gs::Material>& materials);
                ShadowedColorObject& setScalings(const std::vector<Magnum::Vector3>& scalings);

            private:
                void draw(const Magnum::Matrix4& transformationMatrix, Magnum::SceneGraph::Camera3D& camera) override;

                std::vector<std::reference_wrapper<Magnum::GL::Mesh>> _meshes;
                std::reference_wrapper<gs::ShadowMapColor> _shader, _texture_shader;
                std::vector<gs::Material> _materials;
                std::vector<Magnum::Vector3> _scalings;
            };

            class CubeMapShadowedObject : public Object3D, Magnum::SceneGraph::Drawable3D {
            public:
                explicit CubeMapShadowedObject(
                    const std::vector<std::reference_wrapper<Magnum::GL::Mesh>>& meshes,
                    gs::CubeMap& shader,
                    gs::CubeMap& texture_shader,
                    Object3D* parent,
                    Magnum::SceneGraph::DrawableGroup3D* group);

                CubeMapShadowedObject& setMeshes(const std::vector<std::reference_wrapper<Magnum::GL::Mesh>>& meshes);
                CubeMapShadowedObject& setMaterials(const std::vector<gs::Material>& materials);
                CubeMapShadowedObject& setScalings(const std::vector<Magnum::Vector3>& scalings);

            private:
                void draw(const Magnum::Matrix4& transformationMatrix, Magnum::SceneGraph::Camera3D& camera) override;

                std::vector<std::reference_wrapper<Magnum::GL::Mesh>> _meshes;
                std::reference_wrapper<gs::CubeMap> _shader, _texture_shader;
                std::vector<gs::Material> _materials;
                std::vector<Magnum::Vector3> _scalings;
            };

            class CubeMapShadowedColorObject : public Object3D, Magnum::SceneGraph::Drawable3D {
            public:
                explicit CubeMapShadowedColorObject(
                    const std::vector<std::reference_wrapper<Magnum::GL::Mesh>>& meshes,
                    gs::CubeMapColor& shader,
                    gs::CubeMapColor& texture_shader,
                    Object3D* parent,
                    Magnum::SceneGraph::DrawableGroup3D* group);

                CubeMapShadowedColorObject& setMeshes(const std::vector<std::reference_wrapper<Magnum::GL::Mesh>>& meshes);
                CubeMapShadowedColorObject& setMaterials(const std::vector<gs::Material>& materials);
                CubeMapShadowedColorObject& setScalings(const std::vector<Magnum::Vector3>& scalings);

            private:
                void draw(const Magnum::Matrix4& transformationMatrix, Magnum::SceneGraph::Camera3D& camera) override;

                std::vector<std::reference_wrapper<Magnum::GL::Mesh>> _meshes;
                std::reference_wrapper<gs::CubeMapColor> _shader, _texture_shader;
                std::vector<gs::Material> _materials;
                std::vector<Magnum::Vector3> _scalings;
            };

            struct ShadowData {
                Magnum::GL::Framebuffer shadowFramebuffer{Magnum::NoCreate};
                Magnum::GL::Framebuffer shadowColorFramebuffer{Magnum::NoCreate};
            };

            struct ObjectStruct {
                DrawableObject* drawable;
                ShadowedObject* shadowed;
                ShadowedColorObject* shadowed_color;
                CubeMapShadowedObject* cubemapped;
                CubeMapShadowedColorObject* cubemapped_color;
            };
        } // namespace magnum
    } // namespace gui
} // namespace robot_dart
#endif
