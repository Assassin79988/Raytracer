cmake_minimum_required(VERSION 3.10)

project(Raytracer)

#--- Load the common confiquration
include(common/config.cmake)

#--- Load third party packages
include(common/Eigen.cmake)
include(common/OpenCV.cmake)

#--- Adds project directory
add_subdirectory(src)

#--- C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)