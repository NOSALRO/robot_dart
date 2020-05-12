# - Config file for the robot_dart package
# example:
# find_package(RobotDART REQUIRED)
# add_executable(robot_dart_example example.cpp)
# target_link_libraries(robot_dart_example RobotDART::no-graphics)

include(CMakeFindDependencyMacro)

find_dependency(Eigen3 3.3 REQUIRED NO_MODULE)
find_dependency(DART REQUIRED COMPONENTS dart utils utils-urdf collision-bullet collision-ode CONFIG)
find_dependency(Boost REQUIRED COMPONENTS iostreams filesystem thread date_time CONFIG) 
find_dependency(Magnum CONFIG)
find_dependency(MagnumIntegration COMPONENTS Dart Eigen CONFIG)
find_dependency(MagnumPlugins COMPONENTS AssimpImporter PngImporter CONFIG)

# add the robot_dart paths (we get them from waf)
set(RobotDART_INCLUDE_DIRS "/Users/jmouret/include;")
set(RobotDART_LIBRARY_DIRS "/Users/jmouret/lib/")

# we get the compilation options (AVX) from Dart (-march=native)

set(RobotDART_LIBRARIES "Eigen3::Eigen;${DART_LIBRARIES};${Boost_LIBRARIES};RobotDARTSimu")

add_library(RobotDART::no-graphics INTERFACE IMPORTED)
set_target_properties(RobotDART::no-graphics PROPERTIES
  INTERFACE_INCLUDE_DIRECTORIES "${RobotDART_INCLUDE_DIRS}"
  INTERFACE_LINK_DIRECTORIES "${RobotDART_LIBRARY_DIRS}"
  INTERFACE_LINK_LIBRARIES "${RobotDART_LIBRARIES}"
  INTERFACE_COMPILE_OPTIONS "${RobotDART_DEFINITIONS}"
  )

add_library(RobotDART::Magnum INTERFACE IMPORTED)
set_target_properties(RobotDART::Magnum PROPERTIES
  INTERFACE_INCLUDE_DIRECTORIES "${RobotDART_INCLUDE_DIRS}"
  INTERFACE_LINK_LIBRARIES "${RobotDART_LIBRARIES}")
