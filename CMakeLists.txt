cmake_minimum_required(VERSION 2.8.3)
project(robofit)

set(CMAKE_CXX_FLAGS "-std=c++11")
find_package(catkin REQUIRED COMPONENTS
  geometry_msgs
  roscpp
  std_msgs
  sensor_msgs
  image_transport
  cv_bridge
  smach
  smach_ros
  rospy
  tf  
)

find_package(OpenCV REQUIRED)

if(OpenCV_found)
    include_directories(${OpenCV_INCLUDE_DIR})
endif(OpenCV_found)


catkin_package(
)

###########
## Build ##
###########

include_directories(
  ${catkin_INCLUDE_DIRS}
)

add_executable(video_streamer src/robofit/video_streamer.cpp)
add_executable(sound_streamer src/robofit/sound_streamer.cpp)
add_executable(activity_monitor src/robofit/activity_monitor.cpp)

target_link_libraries(video_streamer ${OpenCV_LIBS} ${catkin_LIBRARIES})
target_link_libraries(sound_streamer ${catkin_LIBRARIES})
target_link_libraries(activity_monitor ${catkin_LIBRARIES})

