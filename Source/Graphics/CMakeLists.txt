cmake_minimum_required(VERSION 3.28)

file(GLOB_RECURSE GRAPHICS_SOURCE_FILES
    "${CMAKE_SOURCE_DIR}/Source/Graphics/Source/*.*")

source_group(TREE "${CMAKE_SOURCE_DIR}/Source/Graphics" FILES ${GRAPHICS_SOURCE_FILES})

add_library(Graphics STATIC ${GRAPHICS_SOURCE_FILES})

set_target_properties(Graphics PROPERTIES FOLDER "Engine")

# Includes
target_include_directories(Graphics PUBLIC "${CMAKE_SOURCE_DIR}/Source/Graphics/Source")

# Libraries
target_link_libraries(Graphics PUBLIC Core)
target_link_libraries(Graphics PRIVATE d3d11.lib)
target_link_libraries(Graphics PRIVATE d3dcompiler.lib)

# Set warning levels based on the compiler
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    target_compile_options(Graphics PRIVATE -Wall -Wextra -Werror)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    target_compile_options(Graphics PRIVATE /W4 /WX)
endif()

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

# Set warning levels based on the compiler
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    target_compile_options(GraphicsTests PRIVATE -Wall -Wextra -Werror)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    target_compile_options(GraphicsTests PRIVATE /W4 /WX)
endif()
