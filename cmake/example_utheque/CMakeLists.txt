cmake_minimum_required(VERSION 3.10 FATAL_ERROR)
project(utheque_example)

find_package(Utheque REQUIRED)

add_executable(utheque_example example.cpp)

target_link_libraries(utheque_example Utheque)
