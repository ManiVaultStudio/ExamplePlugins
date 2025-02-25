cmake_minimum_required(VERSION 3.22)

if(NOT DEFINED MV_EXAMPLES_USE_VCPKG)
    set(MV_EXAMPLES_USE_VCPKG OFF)
elseif(MV_EXAMPLES_USE_VCPKG)
    message(STATUS "ExamplePlugins: Using vcpkg to install dependencies")
endif()

# -----------------------------------------------------------------------------
# ExamplePlugins
# -----------------------------------------------------------------------------
set(PROJECT "ExamplePlugins")
PROJECT(${PROJECT} C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

add_subdirectory(Common)

add_subdirectory(ExampleView)
add_subdirectory(ExampleViewJS)
add_subdirectory(ExampleViewOpenGL)
add_subdirectory(ExampleAnalysis)
add_subdirectory(ExampleTransformation)
add_subdirectory(ExampleLoader)
add_subdirectory(ExampleWriter)
add_subdirectory(ExampleData)
add_subdirectory(ExampleDependencies)

set_target_properties(ExampleViewPlugin ExampleViewJSPlugin ExampleViewOpenGLPlugin ExampleAnalysisPlugin ExampleTransformationPlugin ExampleLoaderPlugin ExampleWriterPlugin ExampleDataPlugin ExampleDependenciesPlugin
    PROPERTIES
    FOLDER ExamplePlugins
)
