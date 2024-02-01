

# Class robot\_dart::RobotDARTSimu



[**ClassList**](annotated.md) **>** [**robot\_dart**](namespacerobot__dart.md) **>** [**RobotDARTSimu**](classrobot__dart_1_1RobotDARTSimu.md)






















## Public Types

| Type | Name |
| ---: | :--- |
| typedef std::shared\_ptr&lt; [**Robot**](classrobot__dart_1_1Robot.md) &gt; | [**robot\_t**](#typedef-robot_t)  <br> |




















## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**RobotDARTSimu**](#function-robotdartsimu) (double timestep=0.015) <br> |
|  std::shared\_ptr&lt; [**Robot**](classrobot__dart_1_1Robot.md) &gt; | [**add\_checkerboard\_floor**](#function-add_checkerboard_floor) (double floor\_width=10.0, double floor\_height=0.1, double size=1., const Eigen::Isometry3d & tf=Eigen::Isometry3d::Identity(), const std::string & floor\_name="checkerboard\_floor", const Eigen::Vector4d & first\_color=dart::Color::White(1.), const Eigen::Vector4d & second\_color=dart::Color::Gray(1.)) <br> |
|  std::shared\_ptr&lt; [**Robot**](classrobot__dart_1_1Robot.md) &gt; | [**add\_floor**](#function-add_floor) (double floor\_width=10.0, double floor\_height=0.1, const Eigen::Isometry3d & tf=Eigen::Isometry3d::Identity(), const std::string & floor\_name="floor") <br> |
|  void | [**add\_robot**](#function-add_robot) (const robot\_t & robot) <br> |
|  std::shared\_ptr&lt; T &gt; | [**add\_sensor**](#function-add_sensor-12) (Args &&... args) <br> |
|  void | [**add\_sensor**](#function-add_sensor-22) (const std::shared\_ptr&lt; [**sensor::Sensor**](classrobot__dart_1_1sensor_1_1Sensor.md) &gt; & sensor) <br> |
|  std::shared\_ptr&lt; [**simu::TextData**](structrobot__dart_1_1simu_1_1TextData.md) &gt; | [**add\_text**](#function-add_text) (const std::string & text, const Eigen::Affine2d & tf=Eigen::Affine2d::Identity(), Eigen::Vector4d color=Eigen::Vector4d(1, 1, 1, 1), std::uint8\_t alignment=(1\|3&lt;&lt; 3), bool draw\_bg=false, Eigen::Vector4d bg\_color=Eigen::Vector4d(0, 0, 0, 0.75), double font\_size=28) <br> |
|  void | [**add\_visual\_robot**](#function-add_visual_robot) (const robot\_t & robot) <br> |
|  void | [**clear\_robots**](#function-clear_robots) () <br> |
|  void | [**clear\_sensors**](#function-clear_sensors) () <br> |
|  uint32\_t | [**collision\_category**](#function-collision_category-12) (size\_t robot\_index, const std::string & body\_name) <br> |
|  uint32\_t | [**collision\_category**](#function-collision_category-22) (size\_t robot\_index, size\_t body\_index) <br> |
|  const std::string & | [**collision\_detector**](#function-collision_detector) () const<br> |
|  uint32\_t | [**collision\_mask**](#function-collision_mask-12) (size\_t robot\_index, const std::string & body\_name) <br> |
|  uint32\_t | [**collision\_mask**](#function-collision_mask-22) (size\_t robot\_index, size\_t body\_index) <br> |
|  std::pair&lt; uint32\_t, uint32\_t &gt; | [**collision\_masks**](#function-collision_masks-12) (size\_t robot\_index, const std::string & body\_name) <br> |
|  std::pair&lt; uint32\_t, uint32\_t &gt; | [**collision\_masks**](#function-collision_masks-22) (size\_t robot\_index, size\_t body\_index) <br> |
|  int | [**control\_freq**](#function-control_freq) () const<br> |
|  void | [**enable\_status\_bar**](#function-enable_status_bar) (bool enable=true, double font\_size=-1) <br> |
|  void | [**enable\_text\_panel**](#function-enable_text_panel) (bool enable=true, double font\_size=-1) <br> |
|  std::shared\_ptr&lt; [**gui::Base**](classrobot__dart_1_1gui_1_1Base.md) &gt; | [**graphics**](#function-graphics) () const<br> |
|  int | [**graphics\_freq**](#function-graphics_freq) () const<br> |
|  Eigen::Vector3d | [**gravity**](#function-gravity) () const<br> |
|  [**simu::GUIData**](structrobot__dart_1_1simu_1_1GUIData.md) \* | [**gui\_data**](#function-gui_data) () <br> |
|  bool | [**halted\_sim**](#function-halted_sim) () const<br> |
|  size\_t | [**num\_robots**](#function-num_robots) () const<br> |
|  int | [**physics\_freq**](#function-physics_freq) () const<br> |
|  void | [**remove\_all\_collision\_masks**](#function-remove_all_collision_masks) () <br> |
|  void | [**remove\_collision\_masks**](#function-remove_collision_masks-13) (size\_t robot\_index) <br> |
|  void | [**remove\_collision\_masks**](#function-remove_collision_masks-23) (size\_t robot\_index, const std::string & body\_name) <br> |
|  void | [**remove\_collision\_masks**](#function-remove_collision_masks-33) (size\_t robot\_index, size\_t body\_index) <br> |
|  void | [**remove\_robot**](#function-remove_robot-12) (const robot\_t & robot) <br> |
|  void | [**remove\_robot**](#function-remove_robot-22) (size\_t index) <br> |
|  void | [**remove\_sensor**](#function-remove_sensor-12) (const std::shared\_ptr&lt; [**sensor::Sensor**](classrobot__dart_1_1sensor_1_1Sensor.md) &gt; & sensor) <br> |
|  void | [**remove\_sensor**](#function-remove_sensor-22) (size\_t index) <br> |
|  void | [**remove\_sensors**](#function-remove_sensors) (const std::string & type) <br> |
|  robot\_t | [**robot**](#function-robot) (size\_t index) const<br> |
|  int | [**robot\_index**](#function-robot_index) (const robot\_t & robot) const<br> |
|  const std::vector&lt; robot\_t &gt; & | [**robots**](#function-robots) () const<br> |
|  void | [**run**](#function-run) (double max\_duration=5.0, bool reset\_commands=false, bool force\_position\_bounds=true) <br> |
|  bool | [**schedule**](#function-schedule) (int freq) <br> |
|  [**Scheduler**](classrobot__dart_1_1Scheduler.md) & | [**scheduler**](#function-scheduler-12) () <br> |
|  const [**Scheduler**](classrobot__dart_1_1Scheduler.md) & | [**scheduler**](#function-scheduler-22) () const<br> |
|  std::shared\_ptr&lt; [**sensor::Sensor**](classrobot__dart_1_1sensor_1_1Sensor.md) &gt; | [**sensor**](#function-sensor) (size\_t index) const<br> |
|  std::vector&lt; std::shared\_ptr&lt; [**sensor::Sensor**](classrobot__dart_1_1sensor_1_1Sensor.md) &gt; &gt; | [**sensors**](#function-sensors) () const<br> |
|  void | [**set\_collision\_detector**](#function-set_collision_detector) (const std::string & collision\_detector) <br> |
|  void | [**set\_collision\_masks**](#function-set_collision_masks-13) (size\_t robot\_index, uint32\_t category\_mask, uint32\_t collision\_mask) <br> |
|  void | [**set\_collision\_masks**](#function-set_collision_masks-23) (size\_t robot\_index, const std::string & body\_name, uint32\_t category\_mask, uint32\_t collision\_mask) <br> |
|  void | [**set\_collision\_masks**](#function-set_collision_masks-33) (size\_t robot\_index, size\_t body\_index, uint32\_t category\_mask, uint32\_t collision\_mask) <br> |
|  void | [**set\_control\_freq**](#function-set_control_freq) (int frequency) <br> |
|  void | [**set\_graphics**](#function-set_graphics) (const std::shared\_ptr&lt; [**gui::Base**](classrobot__dart_1_1gui_1_1Base.md) &gt; & graphics) <br> |
|  void | [**set\_graphics\_freq**](#function-set_graphics_freq) (int frequency) <br> |
|  void | [**set\_gravity**](#function-set_gravity) (const Eigen::Vector3d & gravity) <br> |
|  void | [**set\_text\_panel**](#function-set_text_panel) (const std::string & str) <br> |
|  void | [**set\_timestep**](#function-set_timestep) (double timestep, bool update\_control\_freq=true) <br> |
|  std::string | [**status\_bar\_text**](#function-status_bar_text) () const<br> |
|  bool | [**step**](#function-step) (bool reset\_commands=false, bool force\_position\_bounds=true) <br> |
|  bool | [**step\_world**](#function-step_world) (bool reset\_commands=false, bool force\_position\_bounds=true) <br> |
|  void | [**stop\_sim**](#function-stop_sim) (bool disable=true) <br> |
|  std::string | [**text\_panel\_text**](#function-text_panel_text) () const<br> |
|  double | [**timestep**](#function-timestep) () const<br> |
|  dart::simulation::WorldPtr | [**world**](#function-world) () <br> |
|   | [**~RobotDARTSimu**](#function-robotdartsimu) () <br> |








## Protected Attributes

| Type | Name |
| ---: | :--- |
|  bool | [**\_break**](#variable-_break)  <br> |
|  int | [**\_control\_freq**](#variable-_control_freq)   = = -1<br> |
|  std::shared\_ptr&lt; [**gui::Base**](classrobot__dart_1_1gui_1_1Base.md) &gt; | [**\_graphics**](#variable-_graphics)  <br> |
|  int | [**\_graphics\_freq**](#variable-_graphics_freq)   = = 40<br> |
|  std::unique\_ptr&lt; [**simu::GUIData**](structrobot__dart_1_1simu_1_1GUIData.md) &gt; | [**\_gui\_data**](#variable-_gui_data)  <br> |
|  size\_t | [**\_old\_index**](#variable-_old_index)  <br> |
|  int | [**\_physics\_freq**](#variable-_physics_freq)   = = -1<br> |
|  std::vector&lt; robot\_t &gt; | [**\_robots**](#variable-_robots)  <br> |
|  [**Scheduler**](classrobot__dart_1_1Scheduler.md) | [**\_scheduler**](#variable-_scheduler)  <br> |
|  std::vector&lt; std::shared\_ptr&lt; [**sensor::Sensor**](classrobot__dart_1_1sensor_1_1Sensor.md) &gt; &gt; | [**\_sensors**](#variable-_sensors)  <br> |
|  std::shared\_ptr&lt; [**simu::TextData**](structrobot__dart_1_1simu_1_1TextData.md) &gt; | [**\_status\_bar**](#variable-_status_bar)   = = nullptr<br> |
|  std::shared\_ptr&lt; [**simu::TextData**](structrobot__dart_1_1simu_1_1TextData.md) &gt; | [**\_text\_panel**](#variable-_text_panel)   = = nullptr<br> |
|  dart::simulation::WorldPtr | [**\_world**](#variable-_world)  <br> |
















## Protected Functions

| Type | Name |
| ---: | :--- |
|  void | [**\_enable**](#function-_enable) (std::shared\_ptr&lt; [**simu::TextData**](structrobot__dart_1_1simu_1_1TextData.md) &gt; & text, bool enable, double font\_size) <br> |




## Public Types Documentation




### typedef robot\_t 

```C++
using robot_dart::RobotDARTSimu::robot_t =  std::shared_ptr<Robot>;
```



## Public Functions Documentation




### function RobotDARTSimu 

```C++
robot_dart::RobotDARTSimu::RobotDARTSimu (
    double timestep=0.015
) 
```






### function add\_checkerboard\_floor 

```C++
std::shared_ptr< Robot > robot_dart::RobotDARTSimu::add_checkerboard_floor (
    double floor_width=10.0,
    double floor_height=0.1,
    double size=1.,
    const Eigen::Isometry3d & tf=Eigen::Isometry3d::Identity(),
    const std::string & floor_name="checkerboard_floor",
    const Eigen::Vector4d & first_color=dart::Color::White(1.),
    const Eigen::Vector4d & second_color=dart::Color::Gray(1.)
) 
```






### function add\_floor 

```C++
std::shared_ptr< Robot > robot_dart::RobotDARTSimu::add_floor (
    double floor_width=10.0,
    double floor_height=0.1,
    const Eigen::Isometry3d & tf=Eigen::Isometry3d::Identity(),
    const std::string & floor_name="floor"
) 
```






### function add\_robot 

```C++
void robot_dart::RobotDARTSimu::add_robot (
    const robot_t & robot
) 
```






### function add\_sensor [1/2]

```C++
template<typename T typename T, typename... Args>
inline std::shared_ptr< T > robot_dart::RobotDARTSimu::add_sensor (
    Args &&... args
) 
```






### function add\_sensor [2/2]

```C++
void robot_dart::RobotDARTSimu::add_sensor (
    const std::shared_ptr< sensor::Sensor > & sensor
) 
```






### function add\_text 

```C++
std::shared_ptr< simu::TextData > robot_dart::RobotDARTSimu::add_text (
    const std::string & text,
    const Eigen::Affine2d & tf=Eigen::Affine2d::Identity(),
    Eigen::Vector4d color=Eigen::Vector4d(1, 1, 1, 1),
    std::uint8_t alignment=(1|3<< 3),
    bool draw_bg=false,
    Eigen::Vector4d bg_color=Eigen::Vector4d(0, 0, 0, 0.75),
    double font_size=28
) 
```






### function add\_visual\_robot 

```C++
void robot_dart::RobotDARTSimu::add_visual_robot (
    const robot_t & robot
) 
```






### function clear\_robots 

```C++
void robot_dart::RobotDARTSimu::clear_robots () 
```






### function clear\_sensors 

```C++
void robot_dart::RobotDARTSimu::clear_sensors () 
```






### function collision\_category [1/2]

```C++
uint32_t robot_dart::RobotDARTSimu::collision_category (
    size_t robot_index,
    const std::string & body_name
) 
```






### function collision\_category [2/2]

```C++
uint32_t robot_dart::RobotDARTSimu::collision_category (
    size_t robot_index,
    size_t body_index
) 
```






### function collision\_detector 

```C++
const std::string & robot_dart::RobotDARTSimu::collision_detector () const
```






### function collision\_mask [1/2]

```C++
uint32_t robot_dart::RobotDARTSimu::collision_mask (
    size_t robot_index,
    const std::string & body_name
) 
```






### function collision\_mask [2/2]

```C++
uint32_t robot_dart::RobotDARTSimu::collision_mask (
    size_t robot_index,
    size_t body_index
) 
```






### function collision\_masks [1/2]

```C++
std::pair< uint32_t, uint32_t > robot_dart::RobotDARTSimu::collision_masks (
    size_t robot_index,
    const std::string & body_name
) 
```






### function collision\_masks [2/2]

```C++
std::pair< uint32_t, uint32_t > robot_dart::RobotDARTSimu::collision_masks (
    size_t robot_index,
    size_t body_index
) 
```






### function control\_freq 

```C++
inline int robot_dart::RobotDARTSimu::control_freq () const
```






### function enable\_status\_bar 

```C++
void robot_dart::RobotDARTSimu::enable_status_bar (
    bool enable=true,
    double font_size=-1
) 
```






### function enable\_text\_panel 

```C++
void robot_dart::RobotDARTSimu::enable_text_panel (
    bool enable=true,
    double font_size=-1
) 
```






### function graphics 

```C++
std::shared_ptr< gui::Base > robot_dart::RobotDARTSimu::graphics () const
```






### function graphics\_freq 

```C++
inline int robot_dart::RobotDARTSimu::graphics_freq () const
```






### function gravity 

```C++
Eigen::Vector3d robot_dart::RobotDARTSimu::gravity () const
```






### function gui\_data 

```C++
simu::GUIData * robot_dart::RobotDARTSimu::gui_data () 
```






### function halted\_sim 

```C++
bool robot_dart::RobotDARTSimu::halted_sim () const
```






### function num\_robots 

```C++
size_t robot_dart::RobotDARTSimu::num_robots () const
```






### function physics\_freq 

```C++
inline int robot_dart::RobotDARTSimu::physics_freq () const
```






### function remove\_all\_collision\_masks 

```C++
void robot_dart::RobotDARTSimu::remove_all_collision_masks () 
```






### function remove\_collision\_masks [1/3]

```C++
void robot_dart::RobotDARTSimu::remove_collision_masks (
    size_t robot_index
) 
```






### function remove\_collision\_masks [2/3]

```C++
void robot_dart::RobotDARTSimu::remove_collision_masks (
    size_t robot_index,
    const std::string & body_name
) 
```






### function remove\_collision\_masks [3/3]

```C++
void robot_dart::RobotDARTSimu::remove_collision_masks (
    size_t robot_index,
    size_t body_index
) 
```






### function remove\_robot [1/2]

```C++
void robot_dart::RobotDARTSimu::remove_robot (
    const robot_t & robot
) 
```






### function remove\_robot [2/2]

```C++
void robot_dart::RobotDARTSimu::remove_robot (
    size_t index
) 
```






### function remove\_sensor [1/2]

```C++
void robot_dart::RobotDARTSimu::remove_sensor (
    const std::shared_ptr< sensor::Sensor > & sensor
) 
```






### function remove\_sensor [2/2]

```C++
void robot_dart::RobotDARTSimu::remove_sensor (
    size_t index
) 
```






### function remove\_sensors 

```C++
void robot_dart::RobotDARTSimu::remove_sensors (
    const std::string & type
) 
```






### function robot 

```C++
robot_t robot_dart::RobotDARTSimu::robot (
    size_t index
) const
```






### function robot\_index 

```C++
int robot_dart::RobotDARTSimu::robot_index (
    const robot_t & robot
) const
```






### function robots 

```C++
const std::vector< robot_t > & robot_dart::RobotDARTSimu::robots () const
```






### function run 

```C++
void robot_dart::RobotDARTSimu::run (
    double max_duration=5.0,
    bool reset_commands=false,
    bool force_position_bounds=true
) 
```






### function schedule 

```C++
inline bool robot_dart::RobotDARTSimu::schedule (
    int freq
) 
```






### function scheduler [1/2]

```C++
inline Scheduler & robot_dart::RobotDARTSimu::scheduler () 
```






### function scheduler [2/2]

```C++
inline const Scheduler & robot_dart::RobotDARTSimu::scheduler () const
```






### function sensor 

```C++
std::shared_ptr< sensor::Sensor > robot_dart::RobotDARTSimu::sensor (
    size_t index
) const
```






### function sensors 

```C++
std::vector< std::shared_ptr< sensor::Sensor > > robot_dart::RobotDARTSimu::sensors () const
```






### function set\_collision\_detector 

```C++
void robot_dart::RobotDARTSimu::set_collision_detector (
    const std::string & collision_detector
) 
```






### function set\_collision\_masks [1/3]

```C++
void robot_dart::RobotDARTSimu::set_collision_masks (
    size_t robot_index,
    uint32_t category_mask,
    uint32_t collision_mask
) 
```






### function set\_collision\_masks [2/3]

```C++
void robot_dart::RobotDARTSimu::set_collision_masks (
    size_t robot_index,
    const std::string & body_name,
    uint32_t category_mask,
    uint32_t collision_mask
) 
```






### function set\_collision\_masks [3/3]

```C++
void robot_dart::RobotDARTSimu::set_collision_masks (
    size_t robot_index,
    size_t body_index,
    uint32_t category_mask,
    uint32_t collision_mask
) 
```






### function set\_control\_freq 

```C++
inline void robot_dart::RobotDARTSimu::set_control_freq (
    int frequency
) 
```






### function set\_graphics 

```C++
void robot_dart::RobotDARTSimu::set_graphics (
    const std::shared_ptr< gui::Base > & graphics
) 
```






### function set\_graphics\_freq 

```C++
inline void robot_dart::RobotDARTSimu::set_graphics_freq (
    int frequency
) 
```






### function set\_gravity 

```C++
void robot_dart::RobotDARTSimu::set_gravity (
    const Eigen::Vector3d & gravity
) 
```






### function set\_text\_panel 

```C++
void robot_dart::RobotDARTSimu::set_text_panel (
    const std::string & str
) 
```






### function set\_timestep 

```C++
void robot_dart::RobotDARTSimu::set_timestep (
    double timestep,
    bool update_control_freq=true
) 
```






### function status\_bar\_text 

```C++
std::string robot_dart::RobotDARTSimu::status_bar_text () const
```






### function step 

```C++
bool robot_dart::RobotDARTSimu::step (
    bool reset_commands=false,
    bool force_position_bounds=true
) 
```






### function step\_world 

```C++
bool robot_dart::RobotDARTSimu::step_world (
    bool reset_commands=false,
    bool force_position_bounds=true
) 
```






### function stop\_sim 

```C++
void robot_dart::RobotDARTSimu::stop_sim (
    bool disable=true
) 
```






### function text\_panel\_text 

```C++
std::string robot_dart::RobotDARTSimu::text_panel_text () const
```






### function timestep 

```C++
double robot_dart::RobotDARTSimu::timestep () const
```






### function world 

```C++
dart::simulation::WorldPtr robot_dart::RobotDARTSimu::world () 
```






### function ~RobotDARTSimu 

```C++
robot_dart::RobotDARTSimu::~RobotDARTSimu () 
```



## Protected Attributes Documentation




### variable \_break 

```C++
bool robot_dart::RobotDARTSimu::_break;
```






### variable \_control\_freq 

```C++
int robot_dart::RobotDARTSimu::_control_freq;
```






### variable \_graphics 

```C++
std::shared_ptr<gui::Base> robot_dart::RobotDARTSimu::_graphics;
```






### variable \_graphics\_freq 

```C++
int robot_dart::RobotDARTSimu::_graphics_freq;
```






### variable \_gui\_data 

```C++
std::unique_ptr<simu::GUIData> robot_dart::RobotDARTSimu::_gui_data;
```






### variable \_old\_index 

```C++
size_t robot_dart::RobotDARTSimu::_old_index;
```






### variable \_physics\_freq 

```C++
int robot_dart::RobotDARTSimu::_physics_freq;
```






### variable \_robots 

```C++
std::vector<robot_t> robot_dart::RobotDARTSimu::_robots;
```






### variable \_scheduler 

```C++
Scheduler robot_dart::RobotDARTSimu::_scheduler;
```






### variable \_sensors 

```C++
std::vector<std::shared_ptr<sensor::Sensor> > robot_dart::RobotDARTSimu::_sensors;
```






### variable \_status\_bar 

```C++
std::shared_ptr<simu::TextData> robot_dart::RobotDARTSimu::_status_bar;
```






### variable \_text\_panel 

```C++
std::shared_ptr<simu::TextData> robot_dart::RobotDARTSimu::_text_panel;
```






### variable \_world 

```C++
dart::simulation::WorldPtr robot_dart::RobotDARTSimu::_world;
```



## Protected Functions Documentation




### function \_enable 

```C++
void robot_dart::RobotDARTSimu::_enable (
    std::shared_ptr< simu::TextData > & text,
    bool enable,
    double font_size
) 
```




------------------------------
The documentation for this class was generated from the following file `robot_dart/robot_dart_simu.hpp`

