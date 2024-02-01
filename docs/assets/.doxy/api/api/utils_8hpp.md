

# File utils.hpp



[**FileList**](files.md) **>** [**robot\_dart**](dir_166284c5f0440000a6384365f2a45567.md) **>** [**utils.hpp**](utils_8hpp.md)

[Go to the source code of this file](utils_8hpp_source.md)



* `#include <exception>`
* `#include <iostream>`
* `#include <robot_dart/utils_headers_external.hpp>`













## Namespaces

| Type | Name |
| ---: | :--- |
| namespace | [**robot\_dart**](namespacerobot__dart.md) <br> |


## Classes

| Type | Name |
| ---: | :--- |
| class | [**Assertion**](classrobot__dart_1_1Assertion.md) <br> |

















































## Macros

| Type | Name |
| ---: | :--- |
| define  | [**M\_PIf**](utils_8hpp.md#define-m_pif)  static\_cast&lt;float&gt;(M\_PI)<br> |
| define  | [**ROBOT\_DART\_ASSERT**](utils_8hpp.md#define-robot_dart_assert) (condition, message, returnValue) <br> |
| define  | [**ROBOT\_DART\_EXCEPTION\_ASSERT**](utils_8hpp.md#define-robot_dart_exception_assert) (condition, message) <br> |
| define  | [**ROBOT\_DART\_EXCEPTION\_INTERNAL\_ASSERT**](utils_8hpp.md#define-robot_dart_exception_internal_assert) (condition) <br> |
| define  | [**ROBOT\_DART\_INTERNAL\_ASSERT**](utils_8hpp.md#define-robot_dart_internal_assert) (condition) <br> |
| define  | [**ROBOT\_DART\_SHOW\_WARNINGS**](utils_8hpp.md#define-robot_dart_show_warnings)  true<br> |
| define  | [**ROBOT\_DART\_UNUSED\_VARIABLE**](utils_8hpp.md#define-robot_dart_unused_variable) (var) (void)(var)<br> |
| define  | [**ROBOT\_DART\_WARNING**](utils_8hpp.md#define-robot_dart_warning) (condition, message) <br> |

## Macro Definition Documentation





### define M\_PIf 

```C++
#define M_PIf static_cast<float>(M_PI)
```






### define ROBOT\_DART\_ASSERT 

```C++
#define ROBOT_DART_ASSERT (
    condition,
    message,
    returnValue
) do {                                                                                                             \
        if (!(condition)) {                                                                                          \
            std::cerr << __LINE__ << " " << __FILE__ << " -> robot_dart assertion failed: " << message << std::endl; \
            return returnValue;                                                                                      \
        }                                                                                                            \
    } while (false)
```






### define ROBOT\_DART\_EXCEPTION\_ASSERT 

```C++
#define ROBOT_DART_EXCEPTION_ASSERT (
    condition,
    message
) do {                                                \
        if (!(condition)) {                             \
            throw robot_dart::Assertion (message);       \
        }                                               \
    } while (false)
```






### define ROBOT\_DART\_EXCEPTION\_INTERNAL\_ASSERT 

```C++
#define ROBOT_DART_EXCEPTION_INTERNAL_ASSERT (
    condition
) do {                                                \
        if (!(condition)) {                             \
            throw robot_dart::Assertion (#condition);    \
        }                                               \
    } while (false)
```






### define ROBOT\_DART\_INTERNAL\_ASSERT 

```C++
#define ROBOT_DART_INTERNAL_ASSERT (
    condition
) do {                                                                                                                      \
        if (!(condition)) {                                                                                                   \
            std::cerr << "Assertion '" << #condition << "' failed in '" << __FILE__ << "' on line " << __LINE__ << std::endl; \
            std::abort();                                                                                                     \
        }                                                                                                                     \
    } while (false)
```






### define ROBOT\_DART\_SHOW\_WARNINGS 

```C++
#define ROBOT_DART_SHOW_WARNINGS true
```






### define ROBOT\_DART\_UNUSED\_VARIABLE 

```C++
#define ROBOT_DART_UNUSED_VARIABLE (
    var
) (void)(var)
```






### define ROBOT\_DART\_WARNING 

```C++
#define ROBOT_DART_WARNING (
    condition,
    message
) if (ROBOT_DART_SHOW_WARNINGS && (condition)) {                               \
        std::cerr << "[robot_dart WARNING]: \"" << message << "\"" << std::endl; \
    }
```




------------------------------
The documentation for this class was generated from the following file `robot_dart/utils.hpp`

