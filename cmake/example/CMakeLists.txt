cmake_minimum_required(VERSION 3.10 FATAL_ERROR)
project(robot_dart_example)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
#add_definitions(-std=c++14)



find_package(RobotDART REQUIRED)

add_executable(robot_dart_example example.cpp)

target_link_libraries(robot_dart_example
   RobotDART::Simu
)

if(${RobotDART_USE_MAGNUM} STREQUAL "True")
  add_executable(robot_dart_example_graphics example.cpp)
  target_link_libraries(robot_dart_example_graphics
    RobotDART::Simu
    RobotDART::Magnum
  )
endif()
