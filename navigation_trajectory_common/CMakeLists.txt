cmake_minimum_required(VERSION 2.4.6)
include($ENV{ROS_ROOT}/core/rosbuild/rosbuild.cmake)

# Set the build type.  Options are:
#  Coverage       : w/ debug symbols, w/o optimization, w/ code-coverage
#  Debug          : w/ debug symbols, w/o optimization
#  Release        : w/o debug symbols, w/ optimization
#  RelWithDebInfo : w/ debug symbols, w/ optimization
#  MinSizeRel     : w/o debug symbols, w/ optimization, stripped binaries
set(ROS_BUILD_TYPE Debug)

rosbuild_init()
rosbuild_genmsg()

add_definitions (-D DEBUG)

#set the default path for built executables to the "bin" directory
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
#set the default path for built libraries to the "lib" directory
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
#set include directories
include_directories(${PROJECT_SOURCE_DIR}/include)

#common commands for building c++ executables and libraries
rosbuild_add_library(trajectory_common 
    src/FrameWithId.cpp
    src/TwistWithId.cpp
    src/TrajectoryWithId.cpp
    src/Stopwatch.cpp
    src/Pose2D.cpp
    src/Twist2D.cpp
)


