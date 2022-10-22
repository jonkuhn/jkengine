#!/bin/sh

# Create a directory in which to do the build
mkdir build
cd build

# Use conan to install dependencies and generate conanbuildinfo.cmake
conan install .. --build=missing

# configure the project and generate a native build system
cmake ..

# build the project
cmake --build . --config Release -j 6

if [ $? -ne 0 ]; then
    echo "Build Failed"
    exit 1
fi

cd bin
./JkEng.Physics.PerformanceTests

