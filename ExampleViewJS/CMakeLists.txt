cmake_minimum_required(VERSION 3.17)

option(MV_UNITY_BUILD "Combine target source files into batches for faster compilation" OFF)

# -----------------------------------------------------------------------------
# ExampleViewJS Plugin
# -----------------------------------------------------------------------------
PROJECT("ExampleViewJSPlugin")

# -----------------------------------------------------------------------------
# CMake Options
# -----------------------------------------------------------------------------
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /DWIN32 /EHsc /MP /permissive- /Zc:__cplusplus")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /NODEFAULTLIB:LIBCMT")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MDd")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MD")
endif(MSVC)

# -----------------------------------------------------------------------------
# Dependencies
# -----------------------------------------------------------------------------
find_package(Qt6 COMPONENTS Widgets WebEngineWidgets REQUIRED)

find_package(ManiVault COMPONENTS Core PointData CONFIG QUIET)

# -----------------------------------------------------------------------------
# Source files
# -----------------------------------------------------------------------------
# Define the plugin sources
set(PLUGIN_SOURCES
    src/ExampleViewJSPlugin.h
    src/ExampleViewJSPlugin.cpp
    src/ChartWidget.h
    src/ChartWidget.cpp
    src/ExampleViewJSPlugin.json
)

set(WEB
    res/example_chart/radar_chart.html
    res/example_chart/radar_chart.tools.js
    res/js_libs/d3.v3.min.js
    res/js_libs/qwebchannel.tools.js
    res/js_libs/radar-chart.min.js
    res/js_libs/radar-chart.min.css
)

set(AUX
    res/example_chart.qrc
)

qt6_add_resources(RESOURCE_FILES res/example_chart.qrc)

source_group( Plugin FILES ${PLUGIN_SOURCES})
source_group( Web FILES ${WEB})
source_group( Aux FILES ${AUX})

# -----------------------------------------------------------------------------
# CMake Target
# -----------------------------------------------------------------------------
# Create dynamic library for the plugin
add_library(${PROJECT_NAME} SHARED ${PLUGIN_SOURCES} ${AUX} ${WEB} ${RESOURCE_FILES})

# -----------------------------------------------------------------------------
# Target include directories
# -----------------------------------------------------------------------------
# Include ManiVault headers, including system data plugins
target_include_directories(${PROJECT_NAME} PRIVATE "${ManiVault_INCLUDE_DIR}")

# -----------------------------------------------------------------------------
# Target properties
# -----------------------------------------------------------------------------
# Request C++17
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)

# Enable unity build
if(MV_UNITY_BUILD)
    set_target_properties(${PROJECT_NAME} PROPERTIES UNITY_BUILD ON)
endif()

# -----------------------------------------------------------------------------
# Target library linking
# -----------------------------------------------------------------------------
# Link to Qt libraries
target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Widgets)
target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::WebEngineWidgets)

# Link to ManiVault and data plugins
target_link_libraries(${PROJECT_NAME} PRIVATE ManiVault::Core)
target_link_libraries(${PROJECT_NAME} PRIVATE ManiVault::PointData)

# -----------------------------------------------------------------------------
# Target installation
# -----------------------------------------------------------------------------
# Install the shared plugin library to the "Plugins" folder in the ManiVault install directory
install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION Plugins COMPONENT PLUGINS # Windows .dll
    LIBRARY DESTINATION Plugins COMPONENT PLUGINS # Linux/Mac .so
)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND "${CMAKE_COMMAND}"
        --install ${CMAKE_CURRENT_BINARY_DIR}
        --config $<CONFIGURATION>
        --prefix ${ManiVault_INSTALL_DIR}/$<CONFIGURATION>
)

# -----------------------------------------------------------------------------
# Miscellaneous
# -----------------------------------------------------------------------------
# Automatically set the debug environment (command + working directory) for MSVC
if(MSVC)
    set_property(TARGET ${PROJECT_NAME} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY $<IF:$<CONFIG:DEBUG>,${ManiVault_INSTALL_DIR}/debug,${ManiVault_INSTALL_DIR}/release>)
    set_property(TARGET ${PROJECT_NAME} PROPERTY VS_DEBUGGER_COMMAND $<IF:$<CONFIG:DEBUG>,${ManiVault_INSTALL_DIR}/debug/ManiVault\ Studio.exe,${ManiVault_INSTALL_DIR}/release/ManiVault\ Studio.exe>)
endif()
