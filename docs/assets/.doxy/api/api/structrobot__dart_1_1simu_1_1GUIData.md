

# Struct robot\_dart::simu::GUIData



[**ClassList**](annotated.md) **>** [**robot\_dart**](namespacerobot__dart.md) **>** [**simu**](namespacerobot__dart_1_1simu.md) **>** [**GUIData**](structrobot__dart_1_1simu_1_1GUIData.md)










































## Public Functions

| Type | Name |
| ---: | :--- |
|  std::shared\_ptr&lt; [**simu::TextData**](structrobot__dart_1_1simu_1_1TextData.md) &gt; | [**add\_text**](#function-add_text) (const std::string & text, const Eigen::Affine2d & tf=Eigen::Affine2d::Identity(), Eigen::Vector4d color=Eigen::Vector4d(1, 1, 1, 1), std::uint8\_t alignment=(1\|3&lt;&lt; 3), bool draw\_bg=false, Eigen::Vector4d bg\_color=Eigen::Vector4d(0, 0, 0, 0.75), double font\_size=28) <br> |
|  bool | [**cast\_shadows**](#function-cast_shadows) (dart::dynamics::ShapeNode \* shape) const<br> |
|  std::vector&lt; std::pair&lt; dart::dynamics::BodyNode \*, double &gt; &gt; | [**drawing\_axes**](#function-drawing_axes) () const<br> |
|  const std::vector&lt; std::shared\_ptr&lt; [**simu::TextData**](structrobot__dart_1_1simu_1_1TextData.md) &gt; &gt; & | [**drawing\_texts**](#function-drawing_texts) () const<br> |
|  bool | [**ghost**](#function-ghost) (dart::dynamics::ShapeNode \* shape) const<br> |
|  void | [**remove\_robot**](#function-remove_robot) (const std::shared\_ptr&lt; [**Robot**](classrobot__dart_1_1Robot.md) &gt; & robot) <br> |
|  void | [**remove\_text**](#function-remove_text-12) (const std::shared\_ptr&lt; [**simu::TextData**](structrobot__dart_1_1simu_1_1TextData.md) &gt; & data) <br> |
|  void | [**remove\_text**](#function-remove_text-22) (size\_t index) <br> |
|  void | [**update\_robot**](#function-update_robot) (const std::shared\_ptr&lt; [**Robot**](classrobot__dart_1_1Robot.md) &gt; & robot) <br> |




























## Public Functions Documentation




### function add\_text 

```C++
inline std::shared_ptr< simu::TextData > robot_dart::simu::GUIData::add_text (
    const std::string & text,
    const Eigen::Affine2d & tf=Eigen::Affine2d::Identity(),
    Eigen::Vector4d color=Eigen::Vector4d(1, 1, 1, 1),
    std::uint8_t alignment=(1|3<< 3),
    bool draw_bg=false,
    Eigen::Vector4d bg_color=Eigen::Vector4d(0, 0, 0, 0.75),
    double font_size=28
) 
```






### function cast\_shadows 

```C++
inline bool robot_dart::simu::GUIData::cast_shadows (
    dart::dynamics::ShapeNode * shape
) const
```






### function drawing\_axes 

```C++
inline std::vector< std::pair< dart::dynamics::BodyNode *, double > > robot_dart::simu::GUIData::drawing_axes () const
```






### function drawing\_texts 

```C++
inline const std::vector< std::shared_ptr< simu::TextData > > & robot_dart::simu::GUIData::drawing_texts () const
```






### function ghost 

```C++
inline bool robot_dart::simu::GUIData::ghost (
    dart::dynamics::ShapeNode * shape
) const
```






### function remove\_robot 

```C++
inline void robot_dart::simu::GUIData::remove_robot (
    const std::shared_ptr< Robot > & robot
) 
```






### function remove\_text [1/2]

```C++
inline void robot_dart::simu::GUIData::remove_text (
    const std::shared_ptr< simu::TextData > & data
) 
```






### function remove\_text [2/2]

```C++
inline void robot_dart::simu::GUIData::remove_text (
    size_t index
) 
```






### function update\_robot 

```C++
inline void robot_dart::simu::GUIData::update_robot (
    const std::shared_ptr< Robot > & robot
) 
```




------------------------------
The documentation for this class was generated from the following file `robot_dart/gui_data.hpp`

