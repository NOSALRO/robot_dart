

# Struct robot\_dart::sensor::IMUConfig



[**ClassList**](annotated.md) **>** [**robot\_dart**](namespacerobot__dart.md) **>** [**sensor**](namespacerobot__dart_1_1sensor.md) **>** [**IMUConfig**](structrobot__dart_1_1sensor_1_1IMUConfig.md)


























## Public Attributes

| Type | Name |
| ---: | :--- |
|  Eigen::Vector3d | [**accel\_bias**](#variable-accel_bias)   = = Eigen::Vector3d::Zero()<br> |
|  dart::dynamics::BodyNode \* | [**body**](#variable-body)   = = nullptr<br> |
|  size\_t | [**frequency**](#variable-frequency)   = = 200<br> |
|  Eigen::Vector3d | [**gyro\_bias**](#variable-gyro_bias)   = = Eigen::Vector3d::Zero()<br> |
















## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**IMUConfig**](#function-imuconfig-13) (dart::dynamics::BodyNode \* b, size\_t f) <br> |
|   | [**IMUConfig**](#function-imuconfig-23) (const Eigen::Vector3d & gyro\_bias, const Eigen::Vector3d & accel\_bias, dart::dynamics::BodyNode \* b, size\_t f) <br> |
|   | [**IMUConfig**](#function-imuconfig-33) () <br> |




























## Public Attributes Documentation




### variable accel\_bias 

```C++
Eigen::Vector3d robot_dart::sensor::IMUConfig::accel_bias;
```






### variable body 

```C++
dart::dynamics::BodyNode* robot_dart::sensor::IMUConfig::body;
```






### variable frequency 

```C++
size_t robot_dart::sensor::IMUConfig::frequency;
```






### variable gyro\_bias 

```C++
Eigen::Vector3d robot_dart::sensor::IMUConfig::gyro_bias;
```



## Public Functions Documentation




### function IMUConfig [1/3]

```C++
inline robot_dart::sensor::IMUConfig::IMUConfig (
    dart::dynamics::BodyNode * b,
    size_t f
) 
```






### function IMUConfig [2/3]

```C++
inline robot_dart::sensor::IMUConfig::IMUConfig (
    const Eigen::Vector3d & gyro_bias,
    const Eigen::Vector3d & accel_bias,
    dart::dynamics::BodyNode * b,
    size_t f
) 
```






### function IMUConfig [3/3]

```C++
inline robot_dart::sensor::IMUConfig::IMUConfig () 
```




------------------------------
The documentation for this class was generated from the following file `robot_dart/sensor/imu.hpp`

