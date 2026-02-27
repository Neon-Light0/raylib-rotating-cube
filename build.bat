@echo off
rem Create build folder if it doesn't exist
if not exist build mkdir build

pushd build

rem Configure project
cmake -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE="Release" -DCMAKE_C_COMPILER_WORKS=1 -DCMAKE_CXX_COMPILER_WORKS=1 ..

rem Build using 8 cores
cmake --build . -j8

rem Run the executable
cube.exe

popd