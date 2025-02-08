

# Class robot\_dart::Scheduler



[**ClassList**](annotated.md) **>** [**robot\_dart**](namespacerobot__dart.md) **>** [**Scheduler**](classrobot__dart_1_1Scheduler.md)










































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**Scheduler**](#function-scheduler) (double dt, bool sync=false) <br> |
|  double | [**current\_time**](#function-current_time) () const<br>_current time according to the simulation (simulation clock)_  |
|  double | [**dt**](#function-dt) () const<br>_dt used by the simulation (simulation clock)_  |
|  double | [**it\_duration**](#function-it_duration) () const<br> |
|  double | [**last\_it\_duration**](#function-last_it_duration) () const<br> |
|  double | [**next\_time**](#function-next_time) () const<br>_next time according to the simulation (simulation clock)_  |
|  bool | [**operator()**](#function-operator()) (int frequency) <br> |
|  double | [**real\_time**](#function-real_time) () const<br>_time according to the clock's computer (wall clock)_  |
|  double | [**real\_time\_factor**](#function-real_time_factor) () const<br> |
|  void | [**reset**](#function-reset) (double dt, bool sync=false, double current\_time=0., double real\_time=0.) <br> |
|  bool | [**schedule**](#function-schedule) (int frequency) <br> |
|  void | [**set\_sync**](#function-set_sync) (bool enable) <br> |
|  double | [**step**](#function-step) () <br> |
|  bool | [**sync**](#function-sync) () const<br> |




## Protected Types

| Type | Name |
| ---: | :--- |
| typedef std::chrono::high\_resolution\_clock | [**clock\_t**](#typedef-clock_t)  <br> |




## Protected Attributes

| Type | Name |
| ---: | :--- |
|  double | [**\_average\_it\_duration**](#variable-_average_it_duration)   = = 0.<br> |
|  int | [**\_current\_step**](#variable-_current_step)   = = 0<br> |
|  double | [**\_current\_time**](#variable-_current_time)   = = 0.<br> |
|  double | [**\_dt**](#variable-_dt)  <br> |
|  double | [**\_it\_duration**](#variable-_it_duration)   = = 0.<br> |
|  clock\_t::time\_point | [**\_last\_iteration\_time**](#variable-_last_iteration_time)  <br> |
|  int | [**\_max\_frequency**](#variable-_max_frequency)   = = -1<br> |
|  double | [**\_real\_start\_time**](#variable-_real_start_time)   = = 0.<br> |
|  double | [**\_real\_time**](#variable-_real_time)   = = 0.<br> |
|  double | [**\_simu\_start\_time**](#variable-_simu_start_time)   = = 0.<br> |
|  clock\_t::time\_point | [**\_start\_time**](#variable-_start_time)  <br> |
|  bool | [**\_sync**](#variable-_sync)  <br> |




















## Public Functions Documentation




### function Scheduler 

```C++
inline robot_dart::Scheduler::Scheduler (
    double dt,
    bool sync=false
) 
```




<hr>



### function current\_time 

```C++
inline double robot_dart::Scheduler::current_time () const
```




<hr>



### function dt 

```C++
inline double robot_dart::Scheduler::dt () const
```




<hr>



### function it\_duration 

```C++
inline double robot_dart::Scheduler::it_duration () const
```




<hr>



### function last\_it\_duration 

```C++
inline double robot_dart::Scheduler::last_it_duration () const
```




<hr>



### function next\_time 

```C++
inline double robot_dart::Scheduler::next_time () const
```




<hr>



### function operator() 

```C++
inline bool robot_dart::Scheduler::operator() (
    int frequency
) 
```




<hr>



### function real\_time 

```C++
inline double robot_dart::Scheduler::real_time () const
```




<hr>



### function real\_time\_factor 

```C++
inline double robot_dart::Scheduler::real_time_factor () const
```




<hr>



### function reset 

```C++
void robot_dart::Scheduler::reset (
    double dt,
    bool sync=false,
    double current_time=0.,
    double real_time=0.
) 
```




<hr>



### function schedule 

```C++
bool robot_dart::Scheduler::schedule (
    int frequency
) 
```




<hr>



### function set\_sync 


```C++
inline void robot_dart::Scheduler::set_sync (
    bool enable
) 
```



synchronize the simulation clock with the wall clock (when possible, i.e. when the simulation is faster than real time) 


        

<hr>



### function step 


```C++
double robot_dart::Scheduler::step () 
```



call this at the end of the loop (see examples) this will synchronize with real time if requested and increase the counter; returns the real-time (in seconds) 


        

<hr>



### function sync 

```C++
inline bool robot_dart::Scheduler::sync () const
```




<hr>
## Protected Types Documentation




### typedef clock\_t 

```C++
using robot_dart::Scheduler::clock_t =  std::chrono::high_resolution_clock;
```




<hr>
## Protected Attributes Documentation




### variable \_average\_it\_duration 

```C++
double robot_dart::Scheduler::_average_it_duration;
```




<hr>



### variable \_current\_step 

```C++
int robot_dart::Scheduler::_current_step;
```




<hr>



### variable \_current\_time 

```C++
double robot_dart::Scheduler::_current_time;
```




<hr>



### variable \_dt 

```C++
double robot_dart::Scheduler::_dt;
```




<hr>



### variable \_it\_duration 

```C++
double robot_dart::Scheduler::_it_duration;
```




<hr>



### variable \_last\_iteration\_time 

```C++
clock_t::time_point robot_dart::Scheduler::_last_iteration_time;
```




<hr>



### variable \_max\_frequency 

```C++
int robot_dart::Scheduler::_max_frequency;
```




<hr>



### variable \_real\_start\_time 

```C++
double robot_dart::Scheduler::_real_start_time;
```




<hr>



### variable \_real\_time 

```C++
double robot_dart::Scheduler::_real_time;
```




<hr>



### variable \_simu\_start\_time 

```C++
double robot_dart::Scheduler::_simu_start_time;
```




<hr>



### variable \_start\_time 

```C++
clock_t::time_point robot_dart::Scheduler::_start_time;
```




<hr>



### variable \_sync 

```C++
bool robot_dart::Scheduler::_sync;
```




<hr>

------------------------------
The documentation for this class was generated from the following file `robot_dart/scheduler.hpp`

