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
