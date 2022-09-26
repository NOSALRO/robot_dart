cmake_minimum_required(VERSION 3.10 FATAL_ERROR)
project(utheque_example)

find_package(Utheque REQUIRED)

add_executable(utheque_example example_utheque.cpp)
set_target_properties(utheque_example PROPERTIES CXX_STANDARD 17 CXX_STANDARD_REQUIRED YES) # This is not really needed!
target_link_libraries(utheque_example PUBLIC Utheque)
