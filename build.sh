#!/bin/bash

# Default build type
BUILD_TYPE="Release"

# Parse command line arguments
while [[ $# -gt 0 ]]; do
    case $1 in
        -d|--debug)
            BUILD_TYPE="Debug"
            shift
            ;;
        -r|--release)
            BUILD_TYPE="Release"
            shift
            ;;
        -h|--help)
            echo "Usage: $0 [-d|--debug] [-r|--release] [-h|--help]"
            echo "Options:"
            echo "  -d, --debug    Build with debug symbols and no optimization"
            echo "  -r, --release  Build with optimizations enabled (default)"
            echo "  -h, --help     Show this help message"
            exit 0
            ;;
        *)
            echo "Unknown option: $1"
            exit 1
            ;;
    esac
done

# Create build directory if it doesn't exist
mkdir -p build
cd build || exit 1

# Configure and build
echo "Configuring build type: $BUILD_TYPE"
if ! cmake -DCMAKE_BUILD_TYPE="$BUILD_TYPE" ..; then
    echo "CMake configuration failed"
    exit 1
fi

echo "Building..."
if ! cmake --build . -j$(nproc); then
    echo "Build failed"
    exit 1
fi

echo "Build successful!"
if [ "$BUILD_TYPE" = "Debug" ]; then
    echo "Debug build completed with no optimizations"
else
    echo "Release build completed with full optimizations"
fi

./cube
cd ..