cmake_minimum_required(VERSION 3.28)

# TODO: Separate Include and Source files in this library, making public only Include folder.

file(GLOB_RECURSE GRAPHICS_SOURCE_FILES
    #"${CMAKE_SOURCE_DIR}/Source/Graphics/Include/*.*"
    "${CMAKE_SOURCE_DIR}/Source/Graphics/Source/*.*")

source_group(TREE "${CMAKE_SOURCE_DIR}/Source/Graphics" FILES ${GRAPHICS_SOURCE_FILES})

add_library(Graphics STATIC ${GRAPHICS_SOURCE_FILES})

set_target_properties(Graphics PROPERTIES FOLDER "Engine")

# Includes
#target_include_directories(Graphics PUBLIC "${CMAKE_SOURCE_DIR}/Source/Graphics/Include")
target_include_directories(Graphics PUBLIC "${CMAKE_SOURCE_DIR}/Source/Graphics/Source")

# Libraries
target_link_libraries(Graphics PUBLIC Core)
target_link_libraries(Graphics PRIVATE d3d11.lib)
target_link_libraries(Graphics PRIVATE d3dcompiler.lib)

# ------------------------------------------
# Tests
# ------------------------------------------

file(GLOB_RECURSE GRAPHICS_TESTS_SOURCE_FILES
    "${CMAKE_SOURCE_DIR}/Source/Graphics/Tests/*.*")

source_group(TREE "${CMAKE_SOURCE_DIR}/Source/Graphics" FILES ${GRAPHICS_TESTS_SOURCE_FILES})

add_executable(GraphicsTests ${GRAPHICS_TESTS_SOURCE_FILES})

set_target_properties(GraphicsTests PROPERTIES FOLDER "Engine")

# Includes
target_include_directories(GraphicsTests PUBLIC "${CMAKE_SOURCE_DIR}/Source/Graphics/Tests")

# Libraries
target_link_libraries(GraphicsTests PRIVATE Graphics)
