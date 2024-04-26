

# Namespace robot\_dart::detail



[**Namespace List**](namespaces.md) **>** [**robot\_dart**](namespacerobot__dart.md) **>** [**detail**](namespacerobot__dart_1_1detail.md)










































## Public Functions

| Type | Name |
| ---: | :--- |
|  void | [**add\_dof\_data**](#function-add_dof_data) (const Eigen::VectorXd & data, dart::dynamics::SkeletonPtr skeleton, const std::vector&lt; std::string &gt; & dof\_names, const std::unordered\_map&lt; std::string, size\_t &gt; & dof\_map) <br> |
|  Eigen::VectorXd | [**dof\_data**](#function-dof_data) (dart::dynamics::SkeletonPtr skeleton, const std::vector&lt; std::string &gt; & dof\_names, const std::unordered\_map&lt; std::string, size\_t &gt; & dof\_map) <br> |
|  void | [**set\_dof\_data**](#function-set_dof_data) (const Eigen::VectorXd & data, dart::dynamics::SkeletonPtr skeleton, const std::vector&lt; std::string &gt; & dof\_names, const std::unordered\_map&lt; std::string, size\_t &gt; & dof\_map) <br> |




























## Public Functions Documentation




### function add\_dof\_data 

```C++
template<int content>
void robot_dart::detail::add_dof_data (
    const Eigen::VectorXd & data,
    dart::dynamics::SkeletonPtr skeleton,
    const std::vector< std::string > & dof_names,
    const std::unordered_map< std::string, size_t > & dof_map
) 
```




<hr>



### function dof\_data 

```C++
template<int content>
Eigen::VectorXd robot_dart::detail::dof_data (
    dart::dynamics::SkeletonPtr skeleton,
    const std::vector< std::string > & dof_names,
    const std::unordered_map< std::string, size_t > & dof_map
) 
```




<hr>



### function set\_dof\_data 

```C++
template<int content>
void robot_dart::detail::set_dof_data (
    const Eigen::VectorXd & data,
    dart::dynamics::SkeletonPtr skeleton,
    const std::vector< std::string > & dof_names,
    const std::unordered_map< std::string, size_t > & dof_map
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `robot_dart/robot.cpp`

