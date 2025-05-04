#!/bin/bash

# Clean previous build artifacts
echo "Cleaning previous build files..."
rm -rf build/ CMakeFiles CMakeCache.txt Makefile cmake_install.cmake

# Recreate build folder
mkdir build
cd build

# Configure and build
echo "Running CMake..."
cmake ..

echo "Building project..."
make -j8

# Copy the backup file to the build directory and rename it
cd ../
cp backup.txt build/backup.txt
mv build/backup.txt build/test.txt

echo "Done!"
