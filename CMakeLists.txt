cmake_minimum_required(VERSION 3.17)

if(NOT DEFINED MV_EXAMPLES_USE_VCPKG)
    set(MV_EXAMPLES_USE_VCPKG OFF)
elseif(MV_EXAMPLES_USE_VCPKG)
    message(STATUS "ExamplePlugins: Using vcpkg to install dependencies")
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

set_target_properties(ExampleViewPlugin ExampleViewJSPlugin ExampleViewOpenGLPlugin ExampleAnalysisPlugin ExampleTransformationPlugin ExampleLoaderPlugin ExampleWriterPlugin ExampleDataPlugin ExampleDependenciesPlugin
    PROPERTIES
    FOLDER ExamplePlugins
)