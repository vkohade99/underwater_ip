cmake_minimum_required(VERSION 2.8)

project(uw)

## Find catkin and any catkin packages
find_package(catkin REQUIRED COMPONENTS roscpp rospy std_msgs genmsg image_transport cv_bridge)
find_package(OpenCV REQUIRED)

## Declare a catkin package
catkin_package(CATKIN_DEPENDS std_msgs sensor_msgs)
## Build talker and listener
include_directories(include ${catkin_INCLUDE_DIRS})
include_directories(${OpenCV_INCLUDE_DIRS})

#color-correction
add_executable(uw_cc src/color-correct.cpp)
target_link_libraries(uw_cc ${catkin_LIBRARIES})
target_link_libraries(uw_cc ${OpenCV_LIBRARIES})
add_dependencies(uw_cc uw_generate_messages_cpp)

#richardson-lucy
add_executable(uw_rl src/rl_deconv.cpp)
target_link_libraries(uw_rl ${catkin_LIBRARIES})
target_link_libraries(uw_rl ${OpenCV_LIBRARIES})
add_dependencies(uw_rl uw_generate_messages_cpp)

#contrast change
add_executable(uw_cs src/contrast_stretch.cpp)
target_link_libraries(uw_cs ${catkin_LIBRARIES})
target_link_libraries(uw_cs ${OpenCV_LIBRARIES})
add_dependencies(uw_cs uw_generate_messages_cpp)


