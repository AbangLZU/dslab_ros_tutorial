cmake_minimum_required(VERSION 2.8.3)
project(plane_ground_filter)

set(CMAKE_CXX_STANDARD 14)

find_package(catkin REQUIRED COMPONENTS
        roscpp
        std_msgs
        sensor_msgs
        pcl_conversions
        pcl_ros
        )

find_package(PCL REQUIRED)

catkin_package(
        CATKIN_DEPENDS
        roscpp
        sensor_msgs
        pcl_conversions
        pcl_ros
)

include_directories(${catkin_INCLUDE_DIRS})
SET(CMAKE_CXX_FLAGS "-O2 -g -Wall ${CMAKE_CXX_FLAGS}")

add_executable(plane_ground_filter_node src/plane_ground_filter.cpp src/plane_ground_filter.h)

target_link_libraries(plane_ground_filter_node
	${catkin_LIBRARIES}
	${PCL_LIBRARIES}
	)


