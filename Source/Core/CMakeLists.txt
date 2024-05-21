cmake_minimum_required(VERSION 3.28)

# TODO: Separate Include and Source files in this library, making public only Include folder.

file(GLOB_RECURSE CORE_SOURCE_FILES
    #"${CMAKE_SOURCE_DIR}/Source/Core/Include/*.*"
    "${CMAKE_SOURCE_DIR}/Source/Core/Source/*.*")

source_group(TREE "${CMAKE_SOURCE_DIR}/Source/Core" FILES ${CORE_SOURCE_FILES})

add_library(Core STATIC ${CORE_SOURCE_FILES})

set_target_properties(Core PROPERTIES FOLDER "Engine")

# Includes
#target_include_directories(Core PUBLIC "${CMAKE_SOURCE_DIR}/Source/Core/Include")
target_include_directories(Core PUBLIC "${CMAKE_SOURCE_DIR}/Source/Core/Source")

# Libraries
target_link_libraries(Core PUBLIC mathfu)
