

# Namespace robot\_dart::gui::magnum



[**Namespace List**](namespaces.md) **>** [**robot\_dart**](namespacerobot__dart.md) **>** [**gui**](namespacerobot__dart_1_1gui.md) **>** [**magnum**](namespacerobot__dart_1_1gui_1_1magnum.md)


















## Namespaces

| Type | Name |
| ---: | :--- |
| namespace | [**gs**](namespacerobot__dart_1_1gui_1_1magnum_1_1gs.md) <br> |
| namespace | [**sensor**](namespacerobot__dart_1_1gui_1_1magnum_1_1sensor.md) <br> |


## Classes

| Type | Name |
| ---: | :--- |
| class | [**BaseApplication**](classrobot__dart_1_1gui_1_1magnum_1_1BaseApplication.md) <br> |
| class | [**BaseGraphics**](classrobot__dart_1_1gui_1_1magnum_1_1BaseGraphics.md) &lt;typename T&gt;<br> |
| class | [**CubeMapShadowedColorObject**](classrobot__dart_1_1gui_1_1magnum_1_1CubeMapShadowedColorObject.md) <br> |
| class | [**CubeMapShadowedObject**](classrobot__dart_1_1gui_1_1magnum_1_1CubeMapShadowedObject.md) <br> |
| struct | [**DebugDrawData**](structrobot__dart_1_1gui_1_1magnum_1_1DebugDrawData.md) <br> |
| class | [**DrawableObject**](classrobot__dart_1_1gui_1_1magnum_1_1DrawableObject.md) <br> |
| class | [**GlfwApplication**](classrobot__dart_1_1gui_1_1magnum_1_1GlfwApplication.md) <br> |
| struct | [**GlobalData**](structrobot__dart_1_1gui_1_1magnum_1_1GlobalData.md) <br> |
| class | [**Graphics**](classrobot__dart_1_1gui_1_1magnum_1_1Graphics.md) <br> |
| struct | [**GraphicsConfiguration**](structrobot__dart_1_1gui_1_1magnum_1_1GraphicsConfiguration.md) <br> |
| struct | [**ObjectStruct**](structrobot__dart_1_1gui_1_1magnum_1_1ObjectStruct.md) <br> |
| struct | [**ShadowData**](structrobot__dart_1_1gui_1_1magnum_1_1ShadowData.md) <br> |
| class | [**ShadowedColorObject**](classrobot__dart_1_1gui_1_1magnum_1_1ShadowedColorObject.md) <br> |
| class | [**ShadowedObject**](classrobot__dart_1_1gui_1_1magnum_1_1ShadowedObject.md) <br> |
| class | [**WindowlessGLApplication**](classrobot__dart_1_1gui_1_1magnum_1_1WindowlessGLApplication.md) <br> |
| class | [**WindowlessGraphics**](classrobot__dart_1_1gui_1_1magnum_1_1WindowlessGraphics.md) <br> |


## Public Types

| Type | Name |
| ---: | :--- |
| typedef Magnum::SceneGraph::Camera3D | [**Camera3D**](#typedef-camera3d)  <br> |
| typedef Magnum::SceneGraph::Object&lt; Magnum::SceneGraph::MatrixTransformation3D &gt; | [**Object3D**](#typedef-object3d)  <br> |
| typedef Magnum::SceneGraph::Scene&lt; Magnum::SceneGraph::MatrixTransformation3D &gt; | [**Scene3D**](#typedef-scene3d)  <br> |




















## Public Functions

| Type | Name |
| ---: | :--- |
|  [**BaseApplication**](classrobot__dart_1_1gui_1_1magnum_1_1BaseApplication.md) \* | [**make\_application**](#function-make_application) ([**RobotDARTSimu**](classrobot__dart_1_1RobotDARTSimu.md) \* simu, const [**GraphicsConfiguration**](structrobot__dart_1_1gui_1_1magnum_1_1GraphicsConfiguration.md) & configuration=[**GraphicsConfiguration**](structrobot__dart_1_1gui_1_1magnum_1_1GraphicsConfiguration.md)()) <br> |




























## Public Types Documentation




### typedef Camera3D 

```C++
using robot_dart::gui::magnum::Camera3D =  Magnum::SceneGraph::Camera3D;
```




<hr>



### typedef Object3D 

```C++
using robot_dart::gui::magnum::Object3D =  Magnum::SceneGraph::Object<Magnum::SceneGraph::MatrixTransformation3D>;
```




<hr>



### typedef Scene3D 

```C++
using robot_dart::gui::magnum::Scene3D =  Magnum::SceneGraph::Scene<Magnum::SceneGraph::MatrixTransformation3D>;
```




<hr>
## Public Functions Documentation




### function make\_application 

```C++
template<typename T>
inline BaseApplication * robot_dart::gui::magnum::make_application (
    RobotDARTSimu * simu,
    const GraphicsConfiguration & configuration=GraphicsConfiguration ()
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `robot_dart/gui/magnum/base_application.cpp`

