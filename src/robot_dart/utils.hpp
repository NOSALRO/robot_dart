#ifndef ROBOT_DART_UTILS_HPP
#define ROBOT_DART_UTILS_HPP

#include <cstdlib>
#include <exception>
#include <iostream>

#define ROBOT_DART_PRAGMA(x) _Pragma(#x)

#define ROBOT_DART_COMPILER_DIAGNOSTIC_PUSH ROBOT_DART_PRAGMA(GCC diagnostic push)
#define ROBOT_DART_COMPILER_DIAGNOSTIC_POP ROBOT_DART_PRAGMA(GCC diagnostic pop)
#if defined(__clang__)
#define ROBOT_DART_COMPILER_IGNORE_WARNINGS                                     \
    ROBOT_DART_PRAGMA(GCC diagnostic ignored "-Wold-style-cast")                \
    ROBOT_DART_PRAGMA(GCC diagnostic ignored "-Wzero-as-null-pointer-constant") \
    ROBOT_DART_PRAGMA(GCC diagnostic ignored "-Wdeprecated-copy")               \
    ROBOT_DART_PRAGMA(GCC diagnostic ignored "-Wpedantic")                      \
    ROBOT_DART_PRAGMA(GCC diagnostic ignored "-Wmissing-declarations")
#else // #elif defined(__GNUC__) || defined(__GNUG__)
#define ROBOT_DART_COMPILER_IGNORE_WARNINGS                                     \
    ROBOT_DART_PRAGMA(GCC diagnostic ignored "-Wold-style-cast")                \
    ROBOT_DART_PRAGMA(GCC diagnostic ignored "-Wzero-as-null-pointer-constant") \
    ROBOT_DART_PRAGMA(GCC diagnostic ignored "-Wmaybe-uninitialized")           \
    ROBOT_DART_PRAGMA(GCC diagnostic ignored "-Wpedantic")                      \
    ROBOT_DART_PRAGMA(GCC diagnostic ignored "-Wmissing-declarations")          \
    ROBOT_DART_PRAGMA(GCC diagnostic ignored "-Wunused-parameter")
#endif

ROBOT_DART_COMPILER_DIAGNOSTIC_PUSH
ROBOT_DART_COMPILER_IGNORE_WARNINGS
#include <Eigen/Core>
#include <Eigen/Geometry>

#include <dart/config.hpp>
ROBOT_DART_COMPILER_DIAGNOSTIC_POP

#ifndef ROBOT_DART_SHOW_WARNINGS
#define ROBOT_DART_SHOW_WARNINGS true
#endif

#define M_PIf static_cast<float>(M_PI)

namespace robot_dart {

    inline Eigen::VectorXd make_vector(std::initializer_list<double> args)
    {
        return Eigen::VectorXd::Map(args.begin(), args.size());
    }

    class Assertion : public std::exception {
    public:
        Assertion(const std::string& msg = "") : _msg(_make_message(msg)) {}

        const char* what() const throw()
        {
            return _msg.c_str();
        }

    private:
        std::string _msg;

        std::string _make_message(const std::string& msg) const
        {
            std::string message = "robot_dart assertion failed";
            if (msg != "")
                message += ": '" + msg + "'";
            return message;
        }
    };
} // namespace robot_dart

#define ROBOT_DART_UNUSED_VARIABLE(var) (void)(var)

#define ROBOT_DART_WARNING(condition, message)                                   \
    if (ROBOT_DART_SHOW_WARNINGS && (condition)) {                               \
        std::cerr << "[robot_dart WARNING]: \"" << message << "\"" << std::endl; \
    }

#define ROBOT_DART_ASSERT(condition, message, returnValue)                        \
    do {                                                                          \
        if (!(condition)) {                                                       \
            std::cerr << "robot_dart assertion failed: " << message << std::endl; \
            return returnValue;                                                   \
        }                                                                         \
    } while (false)

#define ROBOT_DART_EXCEPTION_ASSERT(condition, message) \
    do {                                                \
        if (!(condition)) {                             \
            throw robot_dart::Assertion(message);       \
        }                                               \
    } while (false)

#define ROBOT_DART_INTERNAL_ASSERT(condition)                                                                                 \
    do {                                                                                                                      \
        if (!(condition)) {                                                                                                   \
            std::cerr << "Assertion '" << #condition << "' failed in '" << __FILE__ << "' on line " << __LINE__ << std::endl; \
            std::abort();                                                                                                     \
        }                                                                                                                     \
    } while (false)

#define ROBOT_DART_EXCEPTION_INTERNAL_ASSERT(condition) \
    do {                                                \
        if (!(condition)) {                             \
            throw robot_dart::Assertion(#condition);    \
        }                                               \
    } while (false)

#endif