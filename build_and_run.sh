#!/bin/bash

# Step 1: Create a build directory if it doesn't exist
BUILD_DIR="build"
if [ ! -d "$BUILD_DIR" ]; then
    echo "Creating build directory..."
    mkdir "$BUILD_DIR"
fi

# Step 2: Navigate into the build directory
cd "$BUILD_DIR"

# Step 3: Run CMake to configure and generate build files
echo "Configuring the project with CMake..."
cmake ..

# Step 4: Build the project
echo "Building the project..."
make

# Step 5: Run the executable with any passed parameters
echo "Running the program with parameters: $@"
./thesisProject "$@"
