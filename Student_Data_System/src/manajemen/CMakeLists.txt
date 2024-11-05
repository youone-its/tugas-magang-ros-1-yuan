cmake_minimum_required(VERSION 3.0.2)
project(manajemen)
link_directories(${catkin_LIBRARY_DIRS})
## Uncomment to use C++11 standard
add_compile_options(-std=c++11)

## Find catkin macros and libraries
find_package(catkin REQUIRED COMPONENTS
  custom_msg
  roscpp
  std_msgs
)

## Declare catkin package
catkin_package(
  CATKIN_DEPENDS custom_msg roscpp std_msgs
)

## Include directories
include_directories(
  ${catkin_INCLUDE_DIRS}
)

## Add executables and link libraries
add_executable(impor src/impor_node.cpp)
target_link_libraries(impor ${catkin_LIBRARIES})

add_executable(ekspor src/ekspor_node.cpp)
target_link_libraries(ekspor ${catkin_LIBRARIES})

add_executable(irt src/irt_node.cpp)
target_link_libraries(irt ${catkin_LIBRARIES})

add_executable(main src/main_node.cpp)
target_link_libraries(main ${catkin_LIBRARIES})

add_executable(controll src/controll.cpp)
target_link_libraries(controll ${catkin_LIBRARIES})