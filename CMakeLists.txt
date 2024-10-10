cmake_minimum_required(VERSION 3.17)

set(MV_EXAMPLES_USE_VCPKG OFF)
if(DEFINED CMAKE_TOOLCHAIN_FILE AND CMAKE_TOOLCHAIN_FILE MATCHES "vcpkg")
    set(MV_EXAMPLES_USE_VCPKG ON)
    set(VCPKG_LIBRARY_LINKAGE "dynamic" CACHE STRING "Link vcpkg libraries dynamically")
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
