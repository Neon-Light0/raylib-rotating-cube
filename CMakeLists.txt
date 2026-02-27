cmake_minimum_required(VERSION 4.2.3)

set(CMAKE_CXX_COMPILER g++)
set(CMAKE_C_COMPILER gcc)

# Set default build type if not specified
#if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
#endif()

# Configure build types
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -Wall -Wextra")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")

# Enable Link Time Optimization for Release builds
include(CheckIPOSupported)
check_ipo_supported(RESULT supported OUTPUT error)
if(supported AND CMAKE_BUILD_TYPE STREQUAL "Release")
    set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)
    message(STATUS "LTO enabled in Release mode")
endif()

# Add clang-format target
find_program(CLANG_FORMAT "clang-format")
if(CLANG_FORMAT)
    file(GLOB_RECURSE ALL_SOURCE_FILES 
        ${CMAKE_SOURCE_DIR}/src/*.cpp
        ${CMAKE_SOURCE_DIR}/includes/*.hpp
        ${CMAKE_SOURCE_DIR}/includes/*.h
    )
    add_custom_target(
        format
        COMMAND ${CLANG_FORMAT} -i -style=file ${ALL_SOURCE_FILES}
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMENT "Formatting source files..."
        VERBATIM
    )
endif()

project(cube)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add include directories
include_directories(${CMAKE_SOURCE_DIR}/libs/raylib/include)
include_directories(${CMAKE_SOURCE_DIR}/includes)

# Add library directories
link_directories(${CMAKE_SOURCE_DIR}/libs/raylib/lib)

# Add executable
add_executable(${PROJECT_NAME} 
    src/main.cpp
)

# Link libraries
if(WIN32)
    target_link_libraries(${PROJECT_NAME} 
        raylib
        winmm  
    )
else()
    target_link_libraries(${PROJECT_NAME} 
        raylib
    )
endif()