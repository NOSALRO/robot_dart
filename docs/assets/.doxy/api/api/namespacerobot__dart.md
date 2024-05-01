

# Namespace robot\_dart



[**Namespace List**](namespaces.md) **>** [**robot\_dart**](namespacerobot__dart.md)


















## Namespaces

| Type | Name |
| ---: | :--- |
| namespace | [**collision\_filter**](namespacerobot__dart_1_1collision__filter.md) <br> |
| namespace | [**control**](namespacerobot__dart_1_1control.md) <br> |
| namespace | [**detail**](namespacerobot__dart_1_1detail.md) <br> |
| namespace | [**gui**](namespacerobot__dart_1_1gui.md) <br> |
| namespace | [**robots**](namespacerobot__dart_1_1robots.md) <br> |
| namespace | [**sensor**](namespacerobot__dart_1_1sensor.md) <br> |
| namespace | [**simu**](namespacerobot__dart_1_1simu.md) <br> |


## Classes

| Type | Name |
| ---: | :--- |
| class | [**Assertion**](classrobot__dart_1_1Assertion.md) <br> |
| class | [**Robot**](classrobot__dart_1_1Robot.md) <br> |
| class | [**RobotDARTSimu**](classrobot__dart_1_1RobotDARTSimu.md) <br> |
| class | [**RobotPool**](classrobot__dart_1_1RobotPool.md) <br> |
| class | [**Scheduler**](classrobot__dart_1_1Scheduler.md) <br> |






## Public Attributes

| Type | Name |
| ---: | :--- |
|  auto | [**body\_node\_get\_friction\_coeff**](#variable-body_node_get_friction_coeff)   = = [](dart::dynamics::BodyNode\* body) {








        return body-&gt;getFrictionCoeff();

    }<br> |
|  auto | [**body\_node\_get\_restitution\_coeff**](#variable-body_node_get_restitution_coeff)   = = [](dart::dynamics::BodyNode\* body) {








        return body-&gt;getRestitutionCoeff();

    }<br> |
|  auto | [**body\_node\_set\_friction\_coeff**](#variable-body_node_set_friction_coeff)   = = [](dart::dynamics::BodyNode\* body, double value) {






        body-&gt;setFrictionCoeff(value);

    }<br> |
|  auto | [**body\_node\_set\_restitution\_coeff**](#variable-body_node_set_restitution_coeff)   = = [](dart::dynamics::BodyNode\* body, double value) {






        body-&gt;setRestitutionCoeff(value);

    }<br> |
















## Public Functions

| Type | Name |
| ---: | :--- |
|  Eigen::Isometry3d | [**make\_tf**](#function-make_tf) (const Eigen::Matrix3d & R, const Eigen::Vector3d & t) <br> |
|  Eigen::Isometry3d | [**make\_tf**](#function-make_tf) (const Eigen::Matrix3d & R) <br> |
|  Eigen::Isometry3d | [**make\_tf**](#function-make_tf) (const Eigen::Vector3d & t) <br> |
|  Eigen::Isometry3d | [**make\_tf**](#function-make_tf) (std::initializer\_list&lt; double &gt; args) <br> |
|  Eigen::VectorXd | [**make\_vector**](#function-make_vector) (std::initializer\_list&lt; double &gt; args) <br> |




























## Public Attributes Documentation




### variable body\_node\_get\_friction\_coeff 

```C++
auto robot_dart::body_node_get_friction_coeff;
```




<hr>



### variable body\_node\_get\_restitution\_coeff 

```C++
auto robot_dart::body_node_get_restitution_coeff;
```




<hr>



### variable body\_node\_set\_friction\_coeff 

```C++
auto robot_dart::body_node_set_friction_coeff;
```




<hr>



### variable body\_node\_set\_restitution\_coeff 

```C++
auto robot_dart::body_node_set_restitution_coeff;
```




<hr>
## Public Functions Documentation




### function make\_tf 

```C++
inline Eigen::Isometry3d robot_dart::make_tf (
    const Eigen::Matrix3d & R,
    const Eigen::Vector3d & t
) 
```




<hr>



### function make\_tf 

```C++
inline Eigen::Isometry3d robot_dart::make_tf (
    const Eigen::Matrix3d & R
) 
```




<hr>



### function make\_tf 

```C++
inline Eigen::Isometry3d robot_dart::make_tf (
    const Eigen::Vector3d & t
) 
```




<hr>



### function make\_tf 

```C++
inline Eigen::Isometry3d robot_dart::make_tf (
    std::initializer_list< double > args
) 
```




<hr>



### function make\_vector 

```C++
inline Eigen::VectorXd robot_dart::make_vector (
    std::initializer_list< double > args
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `robot_dart/control/pd_control.cpp`

