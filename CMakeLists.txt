# Better check cmake version before you define this
cmake_minimum_required(VERSION 3.14.4)

# Compiler flag
set(CMAKE_CXX_FLAGS "-std=c++11")

# Define compiler, clear declaration can minimize the ambiguity
set(CMAKE_C_COMPILER "/usr/bin/clang")
set(CMAKE_CXX_COMPILER "/usr/bin/clang++")

# Project name
project(CalibrateImages)

# Set the output folder where your program will be created
set(CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR})

set(INSTALL_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)
set(INSTALL_BIN_DIR ${PROJECT_SOURCE_DIR}/bin)
set(INSTALL_LIB_DIR ${PROJECT_SOURCE_DIR}/lib)

include_directories(${INSTALL_INCLUDE_DIR})

# Library OpenCV
find_package(OpenCV REQUIRED)

# Library Eigen3
find_package (Eigen3 3.3 REQUIRED NO_MODULE)

# The compiled library code is here
add_subdirectory(src)

# The compiled code is here
add_subdirectory(app)

