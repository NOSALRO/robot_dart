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

namespace dart {
    namespace dynamics {
        class ShapeNode;
    }
} // namespace dart

namespace robot_dart {
    class RobotDARTSimu;

    namespace gui {
        namespace magnum {
            class DrawableObject : public Object3D, Magnum::SceneGraph::Drawable3D {
            public:
                explicit DrawableObject(
                    RobotDARTSimu* simu,
                    dart::dynamics::ShapeNode* shape,
                    const std::vector<std::reference_wrapper<Magnum::GL::Mesh>>& meshes,
                    const std::vector<gs::Material>& materials,
                    gs::PhongMultiLight& color,
                    gs::PhongMultiLight& texture,
                    Object3D* parent,
                    Magnum::SceneGraph::DrawableGroup3D* group);

                DrawableObject& set_meshes(const std::vector<std::reference_wrapper<Magnum::GL::Mesh>>& meshes);
                DrawableObject& set_materials(const std::vector<gs::Material>& materials);
                DrawableObject& set_soft_bodies(const std::vector<bool>& softBody);
                DrawableObject& set_scalings(const std::vector<Magnum::Vector3>& scalings);
                DrawableObject& set_transparent(bool transparent = true);

                DrawableObject& set_color_shader(std::reference_wrapper<gs::PhongMultiLight> shader);
                DrawableObject& set_texture_shader(std::reference_wrapper<gs::PhongMultiLight> shader);

                const std::vector<gs::Material>& materials() const { return _materials; }
                bool transparent() const { return _isTransparent; }

            private:
                void draw(const Magnum::Matrix4& transformationMatrix, Magnum::SceneGraph::Camera3D& camera) override;

                RobotDARTSimu* _simu;
                dart::dynamics::ShapeNode* _shape;
                std::vector<std::reference_wrapper<Magnum::GL::Mesh>> _meshes;
                std::reference_wrapper<gs::PhongMultiLight> _color_shader;
                std::reference_wrapper<gs::PhongMultiLight> _texture_shader;
                std::vector<gs::Material> _materials;
                std::vector<Magnum::Vector3> _scalings;
                std::vector<bool> _has_negative_scaling;
                std::vector<bool> _is_soft_body;
                bool _isTransparent;
            };

            class ShadowedObject : public Object3D, Magnum::SceneGraph::Drawable3D {
            public:
                explicit ShadowedObject(
                    RobotDARTSimu* simu,
                    dart::dynamics::ShapeNode* shape,
                    const std::vector<std::reference_wrapper<Magnum::GL::Mesh>>& meshes,
                    gs::ShadowMap& shader,
                    gs::ShadowMap& texture_shader,
                    Object3D* parent,
                    Magnum::SceneGraph::DrawableGroup3D* group);

                ShadowedObject& set_meshes(const std::vector<std::reference_wrapper<Magnum::GL::Mesh>>& meshes);
                ShadowedObject& set_materials(const std::vector<gs::Material>& materials);
                ShadowedObject& set_scalings(const std::vector<Magnum::Vector3>& scalings);

            private:
                void draw(const Magnum::Matrix4& transformationMatrix, Magnum::SceneGraph::Camera3D& camera) override;

                RobotDARTSimu* _simu;
                dart::dynamics::ShapeNode* _shape;
                std::vector<std::reference_wrapper<Magnum::GL::Mesh>> _meshes;
                std::reference_wrapper<gs::ShadowMap> _shader, _texture_shader;
                std::vector<gs::Material> _materials;
                std::vector<Magnum::Vector3> _scalings;
            };

            class ShadowedColorObject : public Object3D, Magnum::SceneGraph::Drawable3D {
            public:
                explicit ShadowedColorObject(
                    RobotDARTSimu* simu,
                    dart::dynamics::ShapeNode* shape,
                    const std::vector<std::reference_wrapper<Magnum::GL::Mesh>>& meshes,
                    gs::ShadowMapColor& shader,
                    gs::ShadowMapColor& texture_shader,
                    Object3D* parent,
                    Magnum::SceneGraph::DrawableGroup3D* group);

                ShadowedColorObject& set_meshes(const std::vector<std::reference_wrapper<Magnum::GL::Mesh>>& meshes);
                ShadowedColorObject& set_materials(const std::vector<gs::Material>& materials);
                ShadowedColorObject& set_scalings(const std::vector<Magnum::Vector3>& scalings);

            private:
                void draw(const Magnum::Matrix4& transformationMatrix, Magnum::SceneGraph::Camera3D& camera) override;

                RobotDARTSimu* _simu;
                dart::dynamics::ShapeNode* _shape;
                std::vector<std::reference_wrapper<Magnum::GL::Mesh>> _meshes;
                std::reference_wrapper<gs::ShadowMapColor> _shader, _texture_shader;
                std::vector<gs::Material> _materials;
                std::vector<Magnum::Vector3> _scalings;
            };

            class CubeMapShadowedObject : public Object3D, Magnum::SceneGraph::Drawable3D {
            public:
                explicit CubeMapShadowedObject(
                    RobotDARTSimu* simu,
                    dart::dynamics::ShapeNode* shape,
                    const std::vector<std::reference_wrapper<Magnum::GL::Mesh>>& meshes,
                    gs::CubeMap& shader,
                    gs::CubeMap& texture_shader,
                    Object3D* parent,
                    Magnum::SceneGraph::DrawableGroup3D* group);

                CubeMapShadowedObject& set_meshes(const std::vector<std::reference_wrapper<Magnum::GL::Mesh>>& meshes);
                CubeMapShadowedObject& set_materials(const std::vector<gs::Material>& materials);
                CubeMapShadowedObject& set_scalings(const std::vector<Magnum::Vector3>& scalings);

            private:
                void draw(const Magnum::Matrix4& transformationMatrix, Magnum::SceneGraph::Camera3D& camera) override;

                RobotDARTSimu* _simu;
                dart::dynamics::ShapeNode* _shape;
                std::vector<std::reference_wrapper<Magnum::GL::Mesh>> _meshes;
                std::reference_wrapper<gs::CubeMap> _shader, _texture_shader;
                std::vector<gs::Material> _materials;
                std::vector<Magnum::Vector3> _scalings;
            };

            class CubeMapShadowedColorObject : public Object3D, Magnum::SceneGraph::Drawable3D {
            public:
                explicit CubeMapShadowedColorObject(
                    RobotDARTSimu* simu,
                    dart::dynamics::ShapeNode* shape,
                    const std::vector<std::reference_wrapper<Magnum::GL::Mesh>>& meshes,
                    gs::CubeMapColor& shader,
                    gs::CubeMapColor& texture_shader,
                    Object3D* parent,
                    Magnum::SceneGraph::DrawableGroup3D* group);

                CubeMapShadowedColorObject& set_meshes(const std::vector<std::reference_wrapper<Magnum::GL::Mesh>>& meshes);
                CubeMapShadowedColorObject& set_materials(const std::vector<gs::Material>& materials);
                CubeMapShadowedColorObject& set_scalings(const std::vector<Magnum::Vector3>& scalings);

            private:
                void draw(const Magnum::Matrix4& transformationMatrix, Magnum::SceneGraph::Camera3D& camera) override;

                RobotDARTSimu* _simu;
                dart::dynamics::ShapeNode* _shape;
                std::vector<std::reference_wrapper<Magnum::GL::Mesh>> _meshes;
                std::reference_wrapper<gs::CubeMapColor> _shader, _texture_shader;
                std::vector<gs::Material> _materials;
                std::vector<Magnum::Vector3> _scalings;
            };

            struct ShadowData {
                Magnum::GL::Framebuffer shadow_framebuffer{Magnum::NoCreate};
                Magnum::GL::Framebuffer shadow_color_framebuffer{Magnum::NoCreate};
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
