cmake_minimum_required(VERSION 3.28)

# Use C++20 standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set the build configurations to only Debug and Release
set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "Limited Configs" FORCE)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
    cmake_policy(SET CMP0141 NEW)
endif()

# Project name
project ("DirectX11-Course")

# --------------------------------------
# 3rd party libraries

include("${CMAKE_SOURCE_DIR}/Scripts/FetchLibraries.cmake")

# GLFW
FetchContent_MakeAvailable(glfw)

# --------------------------------------
# Main executable

file(GLOB_RECURSE SOURCE_FILES
    "${CMAKE_SOURCE_DIR}/Source/*.h"
    "${CMAKE_SOURCE_DIR}/Source/*.cpp")

source_group(TREE "${CMAKE_SOURCE_DIR}" FILES ${SOURCE_FILES})

add_executable(main .gitignore README.md ${SOURCE_FILES})

target_include_directories(main PRIVATE "${CMAKE_SOURCE_DIR}/Source")

# Libraries
target_link_libraries(main PRIVATE glfw)
target_link_libraries(main PRIVATE d3d11.lib)
target_link_libraries(main PRIVATE d3dcompiler.lib)
target_link_libraries(main PRIVATE dxguid.lib)

# Set main as the default project in Visual Studio
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT main)
