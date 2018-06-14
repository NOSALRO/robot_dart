#ifndef ROBOT_DART_UTILS_HPP
#define ROBOT_DART_UTILS_HPP

#include <cstdlib>
#include <exception>
#include <iostream>

#ifndef ROBOT_DART_SHOW_WARNINGS
#define ROBOT_DART_SHOW_WARNINGS false
#endif

namespace robot_dart {

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

#define ROBOT_DART_WARNING(condition, message)                                    \
    if(ROBOT_DART_SHOW_WARNINGS && (condition)) {                                                            \
        std::cerr<< "[robot_dart WARNING]: \"" << message << "\"" << std::endl;   \
    }                                                                             \

#define ROBOT_DART_ASSERT(condition, message, returnValue)                        \
    do {                                                                          \
        if (!(condition)) {                                                       \
            std::cerr << "robot_dart assertion failed: " << message << std::endl; \
            return returnValue;                                                   \
        }                                                                         \
    } while (false)

#define ROBOT_DART_EXCEPTION_ASSERT(condition, message) \
    do {                                           \
        if (!(condition)) {                        \
            throw robot_dart::Assertion(message);  \
        }                                          \
    } while (false)

#define ROBOT_DART_INTERNAL_ASSERT(condition)                                                                                 \
    do {                                                                                                                      \
        if (!(condition)) {                                                                                                   \
            std::cerr << "Assertion '" << #condition << "' failed in '" << __FILE__ << "' on line " << __LINE__ << std::endl; \
            std::abort();                                                                                                     \
        }                                                                                                                     \
    } while (false)

#define ROBOT_DART_EXCEPTION_INTERNAL_ASSERT(condition)   \
    do {                                             \
        if (!(condition)) {                          \
            throw robot_dart::Assertion(#condition); \
        }                                            \
    } while (false)

#endif