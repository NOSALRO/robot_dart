

# Class robot\_dart::RobotPool



[**ClassList**](annotated.md) **>** [**robot\_dart**](namespacerobot__dart.md) **>** [**RobotPool**](classrobot__dart_1_1RobotPool.md)






















## Public Types

| Type | Name |
| ---: | :--- |
| typedef std::function&lt; std::shared\_ptr&lt; [**Robot**](classrobot__dart_1_1Robot.md) &gt;()&gt; | [**robot\_creator\_t**](#typedef-robot_creator_t)  <br> |




















## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**RobotPool**](#function-robotpool-12) (const robot\_creator\_t & robot\_creator, size\_t pool\_size=32, bool verbose=true) <br> |
|   | [**RobotPool**](#function-robotpool-22) (const [**RobotPool**](classrobot__dart_1_1RobotPool.md) &) = delete<br> |
| virtual void | [**free\_robot**](#function-free_robot) (const std::shared\_ptr&lt; [**Robot**](classrobot__dart_1_1Robot.md) &gt; & robot) <br> |
| virtual std::shared\_ptr&lt; [**Robot**](classrobot__dart_1_1Robot.md) &gt; | [**get\_robot**](#function-get_robot) (const std::string & name="robot") <br> |
|  const std::string & | [**model\_filename**](#function-model_filename) () const<br> |
|  void | [**operator=**](#function-operator) (const [**RobotPool**](classrobot__dart_1_1RobotPool.md) &) = delete<br> |
| virtual  | [**~RobotPool**](#function-robotpool) () <br> |








## Protected Attributes

| Type | Name |
| ---: | :--- |
|  std::vector&lt; bool &gt; | [**\_free**](#variable-_free)  <br> |
|  std::string | [**\_model\_filename**](#variable-_model_filename)  <br> |
|  size\_t | [**\_pool\_size**](#variable-_pool_size)  <br> |
|  robot\_creator\_t | [**\_robot\_creator**](#variable-_robot_creator)  <br> |
|  std::mutex | [**\_skeleton\_mutex**](#variable-_skeleton_mutex)  <br> |
|  std::vector&lt; dart::dynamics::SkeletonPtr &gt; | [**\_skeletons**](#variable-_skeletons)  <br> |
|  bool | [**\_verbose**](#variable-_verbose)  <br> |
















## Protected Functions

| Type | Name |
| ---: | :--- |
| virtual void | [**\_reset\_robot**](#function-_reset_robot) (const std::shared\_ptr&lt; [**Robot**](classrobot__dart_1_1Robot.md) &gt; & robot) <br> |




## Public Types Documentation




### typedef robot\_creator\_t 

```C++
using robot_dart::RobotPool::robot_creator_t =  std::function<std::shared_ptr<Robot>()>;
```



## Public Functions Documentation




### function RobotPool [1/2]

```C++
robot_dart::RobotPool::RobotPool (
    const robot_creator_t & robot_creator,
    size_t pool_size=32,
    bool verbose=true
) 
```






### function RobotPool [2/2]

```C++
robot_dart::RobotPool::RobotPool (
    const RobotPool &
) = delete
```






### function free\_robot 

```C++
virtual void robot_dart::RobotPool::free_robot (
    const std::shared_ptr< Robot > & robot
) 
```






### function get\_robot 

```C++
virtual std::shared_ptr< Robot > robot_dart::RobotPool::get_robot (
    const std::string & name="robot"
) 
```






### function model\_filename 

```C++
inline const std::string & robot_dart::RobotPool::model_filename () const
```






### function operator= 

```C++
void robot_dart::RobotPool::operator= (
    const RobotPool &
) = delete
```






### function ~RobotPool 

```C++
inline virtual robot_dart::RobotPool::~RobotPool () 
```



## Protected Attributes Documentation




### variable \_free 

```C++
std::vector<bool> robot_dart::RobotPool::_free;
```






### variable \_model\_filename 

```C++
std::string robot_dart::RobotPool::_model_filename;
```






### variable \_pool\_size 

```C++
size_t robot_dart::RobotPool::_pool_size;
```






### variable \_robot\_creator 

```C++
robot_creator_t robot_dart::RobotPool::_robot_creator;
```






### variable \_skeleton\_mutex 

```C++
std::mutex robot_dart::RobotPool::_skeleton_mutex;
```






### variable \_skeletons 

```C++
std::vector<dart::dynamics::SkeletonPtr> robot_dart::RobotPool::_skeletons;
```






### variable \_verbose 

```C++
bool robot_dart::RobotPool::_verbose;
```



## Protected Functions Documentation




### function \_reset\_robot 

```C++
virtual void robot_dart::RobotPool::_reset_robot (
    const std::shared_ptr< Robot > & robot
) 
```




------------------------------
The documentation for this class was generated from the following file `robot_dart/robot_pool.hpp`

