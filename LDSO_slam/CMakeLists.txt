cmake_minimum_required(VERSION 3.5)
project(LDSO_slam)

# check for the build type and set it if necessary
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
  message(STATUS "Setting build type to '${CMAKE_BUILD_TYPE}' as none was specified.")
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

# CHECH FOR C++ STANDARD
if(${CMAKE_VERSION} VERSION_LESS "3.1")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

else()
  set(CMAKE_CXX_STANDARD 11)
  set(CMAKE_CXX_STANDARD_REQUIRED ON)
  set(CMAKE_CXX_EXTENSIONS OFF)
endif()

# CUSTOM CMAKE MODULES
LIST(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

#  binary and library
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

# required packages
find_package(Eigen3 REQUIRED NO_MODULE)
find_package(Pangolin REQUIRED)
find_package(OpenCV REQUIRED)
find_package(Glog REQUIRED)

# optional packages
find_package(LibZip QUIET)

# set additional compiler flags
set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-unused-but-set-variable -Wno-unused-local-typedefs -Wno-unused-private-field -Wno-unused-value")

if(NOT APPLE)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
endif()

# Discision if we have ziplib 
if(LIBZIP_LIBRARY)
  add_definitions(-DHAS_LIBZIP=1)
  include_directories(${LIBZIP_INCLUDE_DIR_ZIP} ${LIBZIP_INCLUDE_DIR_ZIPCONF})
  message(STATUS "Found libzip: ${LIBZIP_INCLUDE_DIRS}")
else()
  message(STATUS "libzip not found, will not support zip files")
  set(LIBZIP_LIBRARY "")
endif()

# add the include directories
include_directories(
    ${OpenCV_INCLUDE_DIRS}
    ${Glog_INCLUDE_DIRS}
    ${EIGEN3_INCLUDE_DIRS}
    ${Pangolin_INCLUDE_DIRS}
    
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/thirdparty/
    ${PROJECT_SOURCE_DIR}/thirdparty/DBoW3/
    ${PROJECT_SOURCE_DIR}/thirdparty/g2o/
)

set(THIRD_PARTY_LIBS
    ${OpenCV_LIBS}
    ${GLOG_LIBRARIES}
    ${Pangolin_LIBRARIES}
    ${PROJECT_SOURCE_DIR}/thirdparty/DBoW3/build/src/libDBoW3${CMAKE_SHARED_LIBRARY_SUFFIX}
    ${PROJECT_SOURCE_DIR}/thirdparty/g2o/lib/libg2o${CMAKE_SHARED_LIBRARY_SUFFIX}
    ${LIBZIP_LIBRARY}
    )

# the main source code of LDSO_slam
add_subdirectory(src) #contaiins its own CMakeLists.txt
add_subdirectory(examples) #contaiins its own CMakeLists.txt










