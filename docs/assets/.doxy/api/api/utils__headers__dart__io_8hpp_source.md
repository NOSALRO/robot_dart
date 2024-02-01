

# File utils\_headers\_dart\_io.hpp

[**File List**](files.md) **>** [**robot\_dart**](dir_166284c5f0440000a6384365f2a45567.md) **>** [**utils\_headers\_dart\_io.hpp**](utils__headers__dart__io_8hpp.md)

[Go to the documentation of this file](utils__headers__dart__io_8hpp.md)

```C++

#ifndef ROBOT_DART_UTILS_HEADERS_DART_IO_HPP
#define ROBOT_DART_UTILS_HEADERS_DART_IO_HPP

#pragma GCC system_header

#include <dart/config.hpp>

#if DART_VERSION_AT_LEAST(7, 0, 0)
#include <dart/io/SkelParser.hpp>
#include <dart/io/sdf/SdfParser.hpp>
#include <dart/io/urdf/urdf.hpp>
#else
#include <dart/utils/SkelParser.hpp>
#include <dart/utils/sdf/SdfParser.hpp>
#include <dart/utils/urdf/urdf.hpp>

// namespace alias for compatibility
namespace dart {
    namespace io = utils;
}
#endif

#endif

```

