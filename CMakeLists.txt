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

# --------------------------------------
# Main executable

file(GLOB_RECURSE SOURCE_FILES
    "${CMAKE_SOURCE_DIR}/Source/*.*")

file(GLOB_RECURSE SCRIPT_FILES
    "${CMAKE_SOURCE_DIR}/Scripts/*.*")

file(GLOB_RECURSE ASSET_FILES
    "${CMAKE_SOURCE_DIR}/Assets/*.hlsl")

# Set the Asset files to be treated as non-compilable files
foreach(ASSET_FILE ${ASSET_FILES})
    set_source_files_properties(${ASSET_FILE} PROPERTIES HEADER_FILE_ONLY TRUE)
endforeach()

source_group(TREE "${CMAKE_SOURCE_DIR}" FILES 
    ${SOURCE_FILES}
    ${SCRIPT_FILES}
    ${ASSET_FILES})

add_executable(main README.md .gitignore
    ${SOURCE_FILES}
    ${SCRIPT_FILES}
    ${ASSET_FILES})

# Includes
target_include_directories(main PRIVATE "${CMAKE_SOURCE_DIR}/Source")

# Libraries
target_link_libraries(main PRIVATE glfw)
target_link_libraries(main PRIVATE mathfu)
target_link_libraries(main PRIVATE d3d11.lib)
#target_link_libraries(main PRIVATE d3dcompiler.lib)
#target_link_libraries(main PRIVATE dxguid.lib)

# Set main as the default project in Visual Studio
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT main)
