cmake_minimum_required(VERSION 3.28)

# TODO: Separate Include and Source files in this library, making public only Include folder.

file(GLOB_RECURSE RUNTIME_SOURCE_FILES
    #"${CMAKE_SOURCE_DIR}/Source/Runtime/Include/*.*"
    "${CMAKE_SOURCE_DIR}/Source/Runtime/Source/*.*")

source_group(TREE "${CMAKE_SOURCE_DIR}/Source/Runtime" FILES ${RUNTIME_SOURCE_FILES})

add_library(Runtime STATIC ${RUNTIME_SOURCE_FILES})

set_target_properties(Runtime PROPERTIES FOLDER "Engine")

# Includes
#target_include_directories(Runtime PUBLIC "${CMAKE_SOURCE_DIR}/Source/Runtime/Include")
target_include_directories(Runtime PUBLIC "${CMAKE_SOURCE_DIR}/Source/Runtime/Source")

# Libraries
target_link_libraries(Runtime PUBLIC Core)
target_link_libraries(Runtime PRIVATE glfw)
target_link_libraries(Runtime PRIVATE d3d11.lib)
target_link_libraries(Runtime PRIVATE d3dcompiler.lib)
