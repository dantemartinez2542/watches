cmake_minimum_required(VERSION 4.3.2)

project(Watches LANGUAGES CXX)

# Single-configuration generators (Makefiles/Ninja) choose the build type
# when configuring. Multi-configuration generators choose it at build time.
if(NOT CMAKE_CONFIGURATION_TYPES)
    if(NOT CMAKE_BUILD_TYPE)
        set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build configuration" FORCE)
    endif()
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
        Debug Release RelWithDebInfo MinSizeRel)
endif()

# CMake supplies compiler-specific flags for each configuration:
# Debug includes debug symbols; Release enables optimization and NDEBUG.
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(watches main.cpp watches.cpp)
