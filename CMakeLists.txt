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

# ----------------------------------------------------
# 3rd party libraries

include("${CMAKE_SOURCE_DIR}/Scripts/FetchLibraries.cmake")

# ----------------------------------------------------
# Engine libraries

add_subdirectory(Source/Core)
add_subdirectory(Source/Runtime)

# ----------------------------------------------------
# EditorApplication project

add_subdirectory(Source/EditorApplication)

# Set EditorApplication as the default project in Visual Studio
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT EditorApplication)

# ----------------------------------------------------
# Project with content from root folder for easy access from Visual Studio

set(ROOT_FILES "${CMAKE_SOURCE_DIR}/README.md" "${CMAKE_SOURCE_DIR}/.gitignore")

file(GLOB_RECURSE SCRIPT_FILES
    "${CMAKE_SOURCE_DIR}/Scripts/*.*")

file(GLOB_RECURSE ASSET_FILES
    "${CMAKE_SOURCE_DIR}/Assets/*.*")
    
# Set the Asset files to be treated as non-compilable files
foreach(ASSET_FILE ${ASSET_FILES})
    set_source_files_properties(${ASSET_FILE} PROPERTIES HEADER_FILE_ONLY TRUE)
endforeach()

source_group(TREE "${CMAKE_SOURCE_DIR}" FILES 
    ${ROOT_FILES}
    ${SCRIPT_FILES}
    ${ASSET_FILES})

add_library(Content INTERFACE 
    ${ROOT_FILES}
    ${SCRIPT_FILES}
    ${ASSET_FILES})
