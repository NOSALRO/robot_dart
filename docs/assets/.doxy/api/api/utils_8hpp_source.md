

# File utils.hpp

[**File List**](files.md) **>** [**robot\_dart**](dir_166284c5f0440000a6384365f2a45567.md) **>** [**utils.hpp**](utils_8hpp.md)

[Go to the documentation of this file](utils_8hpp.md)

```C++

#ifndef ROBOT_DART_UTILS_HPP
#define ROBOT_DART_UTILS_HPP

#include <exception>
#include <iostream>

#include <robot_dart/utils_headers_external.hpp>

#ifndef ROBOT_DART_SHOW_WARNINGS
#define ROBOT_DART_SHOW_WARNINGS true
#endif

#ifndef M_PIf
#define M_PIf static_cast<float>(M_PI)
#endif

namespace robot_dart {

    inline Eigen::VectorXd make_vector(std::initializer_list<double> args)
    {
        return Eigen::VectorXd::Map(args.begin(), args.size());
    }

    inline Eigen::Isometry3d make_tf(const Eigen::Matrix3d& R, const Eigen::Vector3d& t)
    {
        Eigen::Isometry3d tf = Eigen::Isometry3d::Identity();
        tf.linear().matrix() = R;
        tf.translation() = t;

        return tf;
    }

    inline Eigen::Isometry3d make_tf(const Eigen::Matrix3d& R)
    {
        Eigen::Isometry3d tf = Eigen::Isometry3d::Identity();
        tf.linear().matrix() = R;

        return tf;
    }

    inline Eigen::Isometry3d make_tf(const Eigen::Vector3d& t)
    {
        Eigen::Isometry3d tf = Eigen::Isometry3d::Identity();
        tf.translation() = t;

        return tf;
    }

    inline Eigen::Isometry3d make_tf(std::initializer_list<double> args)
    {
        Eigen::Isometry3d tf = Eigen::Isometry3d::Identity();
        tf.translation() = make_vector(args);

        return tf;
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

#define ROBOT_DART_ASSERT(condition, message, returnValue)                                                           \
    do {                                                                                                             \
        if (!(condition)) {                                                                                          \
            std::cerr << __LINE__ << " " << __FILE__ << " -> robot_dart assertion failed: " << message << std::endl; \
            return returnValue;                                                                                      \
        }                                                                                                            \
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

```

