#ifndef ROBOT_DART_GUI_MAGNUM_TYPES_HPP
#define ROBOT_DART_GUI_MAGNUM_TYPES_HPP

#include <Magnum/SceneGraph/Camera.h>
#include <Magnum/SceneGraph/MatrixTransformation3D.h>
#include <Magnum/SceneGraph/Scene.h>

namespace robot_dart {
    namespace gui {
        namespace magnum {
            using Object3D = Magnum::SceneGraph::Object<Magnum::SceneGraph::MatrixTransformation3D>;
            using Scene3D = Magnum::SceneGraph::Scene<Magnum::SceneGraph::MatrixTransformation3D>;
            using Camera3D = Magnum::SceneGraph::Camera3D;
        } // namespace magnum
    } // namespace gui
} // namespace robot_dart

#endif