cmake_minimum_required(VERSION 3.28)

file(GLOB_RECURSE EDITOR_APPLICATION_SOURCE_FILES
    "${CMAKE_SOURCE_DIR}/Source/EditorApplication/Source/*.*")

source_group(TREE "${CMAKE_SOURCE_DIR}/Source/EditorApplication" FILES 
    ${EDITOR_APPLICATION_SOURCE_FILES})

add_executable(EditorApplication
    ${EDITOR_APPLICATION_SOURCE_FILES})

# Includes
target_include_directories(EditorApplication PRIVATE "${CMAKE_SOURCE_DIR}/Source/EditorApplication/Source")

# Libraries
target_link_libraries(EditorApplication PRIVATE Runtime)
