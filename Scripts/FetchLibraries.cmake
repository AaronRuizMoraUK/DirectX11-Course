cmake_minimum_required(VERSION 3.28)

include(FetchContent)

set(3rdparty_folder "3rdParty")

# ---------------------------------------------------------------------
# glfw
#
# Library to handle the window of the application.
# ---------------------------------------------------------------------
FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 3.4
)

FetchContent_MakeAvailable(glfw)

set(glfw_folder "${3rdparty_folder}/GLFW3")
set_target_properties(glfw PROPERTIES FOLDER ${glfw_folder})
set_target_properties(uninstall PROPERTIES FOLDER ${glfw_folder})
set_target_properties(update_mappings PROPERTIES FOLDER ${glfw_folder})

# ---------------------------------------------------------------------
# mathfu
#
# Provides a lightweight and efficient implementation of vectors,
# matrices and quaternions.
# ---------------------------------------------------------------------
set(mathfu_enable_simd ON CACHE INTERNAL "Turn on SIMD")
set(mathfu_build_benchmarks OFF CACHE INTERNAL "Turn off benchmarks")
set(mathfu_build_tests OFF CACHE INTERNAL "Turn off tests")
set(mathfu_patch git apply ${CMAKE_SOURCE_DIR}/Scripts/Patches/mathfu.patch)

FetchContent_Declare(
    mathfu
    GIT_REPOSITORY https://github.com/google/mathfu.git
    GIT_TAG da23a1227bb65fbb7f2f5b6c504fbbdd1dfdab4b # Last commit on master (9th May 2022)
    PATCH_COMMAND ${mathfu_patch} # Patch to fix std::max/min usage and increase CMake min version
    UPDATE_DISCONNECTED 1 # Avoid to apply the patch again once it has been populated
)

FetchContent_MakeAvailable(mathfu)

file(GLOB_RECURSE VECTORIAL_SOURCE_FILES
    "${mathfu_SOURCE_DIR}/dependencies/vectorial/include/*.*")
source_group(TREE "${mathfu_SOURCE_DIR}/dependencies/vectorial/include" FILES ${VECTORIAL_SOURCE_FILES})
add_library(vectorial INTERFACE ${VECTORIAL_SOURCE_FILES})
target_include_directories(vectorial INTERFACE "${mathfu_SOURCE_DIR}/dependencies/vectorial/include") 

file(GLOB_RECURSE MATHFU_SOURCE_FILES
    "${mathfu_SOURCE_DIR}/include/*.*")
source_group(TREE "${mathfu_SOURCE_DIR}/include" FILES ${MATHFU_SOURCE_FILES})
add_library(mathfu INTERFACE ${MATHFU_SOURCE_FILES})
target_include_directories(mathfu INTERFACE "${mathfu_SOURCE_DIR}/include") 
target_link_libraries(mathfu INTERFACE vectorial)

set(mathfu_folder "${3rdparty_folder}/mathfu")
set_target_properties(mathfu PROPERTIES FOLDER ${mathfu_folder})
set_target_properties(vectorial PROPERTIES FOLDER ${mathfu_folder})
