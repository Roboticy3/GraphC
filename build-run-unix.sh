#!/bin/bash -x

#NOTE: this BASH script compiles/runs the code through the command line, rather than through CLion or any other IDE

#create the build directory
mkdir -p build-unix

#go to the directory
cd build-unix

#run cmake to check if the compiler, C++ libraries, etc. are correct in your system and generate the necessary files to compile the code
cmake ../

#run cmake to compile the code and build the executable
cmake --build ./