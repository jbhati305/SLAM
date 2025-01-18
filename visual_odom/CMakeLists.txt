cmake_minimum_required(VERSION 3.5)
project(visual_odom)

find_package(OpenCV REQUIRED)
find_package(Pangolin REQUIRED)

# Define dataset path relative to project root
set(DATASET_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../Kitti-Odometry" 
    CACHE PATH "Path to Kitti dataset")

include_directories(
    ${Pangolin_INCLUDE_DIRS}
    ${OpenCV_INCLUDE_DIRS}
    "/usr/include/eigen3"
    "include"
    ${DATASET_PATH}
    )

# Pass dataset path to source code
add_definitions(-DDATASET_PATH="${DATASET_PATH}")

add_library(readData SHARED src/readData.cpp)
target_link_libraries(readData ${OpenCV_LIBS})

add_library(dataProcessing SHARED src/dataProcessing.cpp)
target_link_libraries(dataProcessing ${OpenCV_LIBS})

add_library(Draw SHARED src/Draw.cpp)
target_link_libraries(Draw ${Pangolin_LIBRARIES})

add_executable(visualOdom visualOdom.cpp)
target_link_libraries(visualOdom 
    readData
    Draw
    dataProcessing
    ${OpenCV_LIBS}
    ${Pangolin_LIBRARIES}
    )

add_executable(opencv_test opencv_test.cpp)
target_link_libraries(opencv_test ${OpenCV_LIBS})