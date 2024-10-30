#This CMAKE configuration file defines parameters required to compile/run the code

#cmake version
cmake_minimum_required(VERSION 3.10)

#project name
project(GraphC)

#define c++ standard (don't change these variables)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)


set(CMAKE_CXX_FLAGS "-Wall -Wextra -O3")

#define the source code directories, "include" is for headers and "src" is for implementation
include_directories(
        ${PROJECT_SOURCE_DIR}/include
        ${PROJECT_SOURCE_DIR}/src
)


#define a list, called SRC_FILES, that finds and stores all the source code files within "include" and "src" directories
file(GLOB_RECURSE SRC_FILES
        "${PROJECT_SOURCE_DIR}/include/*.h"
        "${PROJECT_SOURCE_DIR}/src/*.c"
        )

#include all the source code files to generate an executable
add_executable(GraphC ${SRC_FILES})
