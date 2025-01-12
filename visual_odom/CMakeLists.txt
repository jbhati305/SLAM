cmake_minimum_required(VERSION 3.5)
project(visual_odom)

find_package(OpenCV REQUIRED)
find_package(Pangolin REQUIRED)

include_directories("/usr/include/eigen3")
include_directories(${Pangolin_INCLUDE_DIRS})

# Define dataset path relative to project root
set(DATASET_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../Kitti-Odometry" 
    CACHE PATH "Path to Kitti dataset")
# Add dataset include path
include_directories(${DATASET_PATH})
# Pass dataset path to source code
add_definitions(-DDATASET_PATH="${DATASET_PATH}")

add_library(readData SHARED readData.cpp)
add_executable(visualOdom visualOdom.cpp)
target_link_libraries(visualOdom readData)
# target_link_libraries(visualOdom ${OpenCV_LIBS})
target_link_libraries(visualOdom ${Pangolin_LIBRARIES})
