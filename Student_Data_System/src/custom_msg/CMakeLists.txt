cmake_minimum_required(VERSION 3.0.2)
project(custom_msg)
link_directories(${catkin_LIBRARY_DIRS})

find_package(catkin REQUIRED COMPONENTS
  message_generation
  message_runtime
  roscpp
  std_msgs
)

include_directories(
# include
  ${catkin_INCLUDE_DIRS}
)

# Get all .msg files recursively
file(GLOB_RECURSE MSG_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "msg/*.msg")

# Create an empty list to hold the modified paths
set(MODIFIED_MSG_FILES "")

# Loop over each file
foreach(FILE ${MSG_FILES})
    # Get the relative path to the file
    file(RELATIVE_PATH RELATIVE_FILE ${CMAKE_CURRENT_SOURCE_DIR}/msg ${CMAKE_CURRENT_SOURCE_DIR}/${FILE})

    # Add the modified path to the list
    list(APPEND MODIFIED_MSG_FILES ${RELATIVE_FILE})
endforeach()

# Now MODIFIED_MSG_FILES contains the paths to all .msg files, without the leading 'msg/'
message(STATUS "Generating messages ${MODIFIED_MSG_FILES}")

## Generate messages in the 'msg' folder
add_message_files(
  FILES
  ${MODIFIED_MSG_FILES}
)

## Generate added messages and services with any dependencies listed here
generate_messages(
  DEPENDENCIES
  std_msgs
)

catkin_package(
#  INCLUDE_DIRS include
#  LIBRARIES custom_msg
 CATKIN_DEPENDS message_generation message_runtime roscpp rospy std_msgs
#  DEPENDS system_lib
)

include_directories(
# include
  ${catkin_INCLUDE_DIRS}
)