

# Class robot\_dart::collision\_filter::BitmaskContactFilter



[**ClassList**](annotated.md) **>** [**robot\_dart**](namespacerobot__dart.md) **>** [**collision\_filter**](namespacerobot__dart_1_1collision__filter.md) **>** [**BitmaskContactFilter**](classrobot__dart_1_1collision__filter_1_1BitmaskContactFilter.md)








Inherits the following classes: dart::collision::BodyNodeCollisionFilter












## Classes

| Type | Name |
| ---: | :--- |
| struct | [**Masks**](structrobot__dart_1_1collision__filter_1_1BitmaskContactFilter_1_1Masks.md) <br> |


## Public Types

| Type | Name |
| ---: | :--- |
| typedef const dart::collision::CollisionObject \* | [**DartCollisionConstPtr**](#typedef-dartcollisionconstptr)  <br> |
| typedef const dart::dynamics::ShapeNode \* | [**DartShapeConstPtr**](#typedef-dartshapeconstptr)  <br> |




















## Public Functions

| Type | Name |
| ---: | :--- |
|  void | [**add\_to\_map**](#function-add_to_map-12) (DartShapeConstPtr shape, uint32\_t col\_mask, uint32\_t cat\_mask) <br> |
|  void | [**add\_to\_map**](#function-add_to_map-22) (dart::dynamics::SkeletonPtr skel, uint32\_t col\_mask, uint32\_t cat\_mask) <br> |
|  void | [**clear\_all**](#function-clear_all) () <br> |
|  bool | [**ignoresCollision**](#function-ignorescollision) (DartCollisionConstPtr object1, DartCollisionConstPtr object2) override const<br> |
|  [**Masks**](structrobot__dart_1_1collision__filter_1_1BitmaskContactFilter_1_1Masks.md) | [**mask**](#function-mask) (DartShapeConstPtr shape) const<br> |
|  void | [**remove\_from\_map**](#function-remove_from_map-12) (DartShapeConstPtr shape) <br> |
|  void | [**remove\_from\_map**](#function-remove_from_map-22) (dart::dynamics::SkeletonPtr skel) <br> |
| virtual  | [**~BitmaskContactFilter**](#function-bitmaskcontactfilter) () = default<br> |




























## Public Types Documentation




### typedef DartCollisionConstPtr 

```C++
using robot_dart::collision_filter::BitmaskContactFilter::DartCollisionConstPtr =  const dart::collision::CollisionObject*;
```




<hr>



### typedef DartShapeConstPtr 

```C++
using robot_dart::collision_filter::BitmaskContactFilter::DartShapeConstPtr =  const dart::dynamics::ShapeNode*;
```




<hr>
## Public Functions Documentation




### function add\_to\_map [1/2]

```C++
inline void robot_dart::collision_filter::BitmaskContactFilter::add_to_map (
    DartShapeConstPtr shape,
    uint32_t col_mask,
    uint32_t cat_mask
) 
```




<hr>



### function add\_to\_map [2/2]

```C++
inline void robot_dart::collision_filter::BitmaskContactFilter::add_to_map (
    dart::dynamics::SkeletonPtr skel,
    uint32_t col_mask,
    uint32_t cat_mask
) 
```




<hr>



### function clear\_all 

```C++
inline void robot_dart::collision_filter::BitmaskContactFilter::clear_all () 
```




<hr>



### function ignoresCollision 

```C++
inline bool robot_dart::collision_filter::BitmaskContactFilter::ignoresCollision (
    DartCollisionConstPtr object1,
    DartCollisionConstPtr object2
) override const
```




<hr>



### function mask 

```C++
inline Masks robot_dart::collision_filter::BitmaskContactFilter::mask (
    DartShapeConstPtr shape
) const
```




<hr>



### function remove\_from\_map [1/2]

```C++
inline void robot_dart::collision_filter::BitmaskContactFilter::remove_from_map (
    DartShapeConstPtr shape
) 
```




<hr>



### function remove\_from\_map [2/2]

```C++
inline void robot_dart::collision_filter::BitmaskContactFilter::remove_from_map (
    dart::dynamics::SkeletonPtr skel
) 
```




<hr>



### function ~BitmaskContactFilter 

```C++
virtual robot_dart::collision_filter::BitmaskContactFilter::~BitmaskContactFilter () = default
```




<hr>

------------------------------
The documentation for this class was generated from the following file `robot_dart/robot_dart_simu.cpp`

