cmake_minimum_required(VERSION 3.17)

set(VCPKG_LIBRARY_LINKAGE "dynamic" CACHE STRING "Link vcpkg libraries dynamically")

if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    message(FATAL_ERROR "Define CMAKE_TOOLCHAIN_FILE poiting to vcpkg: PATH/vcpkg/scripts/buildsystems/vcpkg.cmake")
endif()

# -----------------------------------------------------------------------------
# ExamplePlugins
# -----------------------------------------------------------------------------
set(PROJECT "ExamplePlugins")
PROJECT(${PROJECT})

add_subdirectory(ExampleView)
add_subdirectory(ExampleViewJS)
add_subdirectory(ExampleViewOpenGL)
add_subdirectory(ExampleAnalysis)
add_subdirectory(ExampleTransformation)
add_subdirectory(ExampleLoader)
add_subdirectory(ExampleWriter)
add_subdirectory(ExampleData)
add_subdirectory(ExampleDependencies)
