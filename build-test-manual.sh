#!/bin/bash

#NOTE: this doesn't actually build the program, run build-run-unix.sh first

for ((i = 0; i < 41; i++)); do
    sleep 0.1
    ./build-unix/cmake_base_project $i
done