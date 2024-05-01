

# File types.hpp

[**File List**](files.md) **>** [**gui**](dir_6a9d4b7ec29c938d1d9a486c655cfc8a.md) **>** [**magnum**](dir_5d18adecbc10cabf3ca51da31f2acdd1.md) **>** [**types.hpp**](types_8hpp.md)

[Go to the documentation of this file](types_8hpp.md)


```C++
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
```


