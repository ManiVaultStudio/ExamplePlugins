cmake_minimum_required(VERSION 3.1.0)

set(PROJECT "ExampleAnalysisPlugin")

PROJECT(${PROJECT})

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /DWIN32 /EHsc /MP /permissive- /Zc:__cplusplus")
	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /NODEFAULTLIB:LIBCMT")
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MDd")
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MD")
endif(MSVC)

# Get the pre-defined install path environment variable
file(TO_CMAKE_PATH $ENV{HDPS_INSTALL_DIR} INSTALL_DIR)

find_package(Qt6 COMPONENTS Widgets WebEngineWidgets REQUIRED)

# Define the plugin sources
set(SOURCES
    src/ExampleAnalysisPlugin.h
    src/ExampleAnalysisPlugin.cpp
    src/SettingsAction.h
    src/SettingsAction.cpp
    src/ExampleAnalysisPlugin.json
)

source_group( Plugin FILES ${SOURCES})

# Create dynamic library for the plugin
add_library(${PROJECT} SHARED ${SOURCES})

# Include HDPS headers, including system data plugins
target_include_directories(${PROJECT} PRIVATE "${INSTALL_DIR}/$<CONFIGURATION>/include/")

# Request C++17
target_compile_features(${PROJECT} PRIVATE cxx_std_17)

# Link to Qt libraries
target_link_libraries(${PROJECT} PRIVATE Qt6::Widgets)
target_link_libraries(${PROJECT} PRIVATE Qt6::WebEngineWidgets)

# Link to HDPS and data plugins
# The link path in this repo assume that the ManiVault core was build locally
# in contrast to installed with an installed. In the latter case you'll have
# to adapt the HDPS_LINK_PATH and PLUGIN_LINK_PATH to your install folder
set(HDPS_LINK_PATH "${INSTALL_DIR}/$<CONFIGURATION>/lib")
set(PLUGIN_LINK_PATH "${INSTALL_DIR}/$<CONFIGURATION>/$<IF:$<CXX_COMPILER_ID:MSVC>,lib,Plugins>")
set(HDPS_LINK_SUFFIX $<IF:$<CXX_COMPILER_ID:MSVC>,${CMAKE_LINK_LIBRARY_SUFFIX},${CMAKE_SHARED_LIBRARY_SUFFIX}>)

set(HDPS_LINK_LIBRARY "${HDPS_LINK_PATH}/${CMAKE_SHARED_LIBRARY_PREFIX}HDPS_Public${HDPS_LINK_SUFFIX}")
set(POINTDATA_LINK_LIBRARY "${PLUGIN_LINK_PATH}/${CMAKE_SHARED_LIBRARY_PREFIX}PointData${HDPS_LINK_SUFFIX}") 

target_link_libraries(${PROJECT} PRIVATE "${HDPS_LINK_LIBRARY}")
target_link_libraries(${PROJECT} PRIVATE "${POINTDATA_LINK_LIBRARY}")

# Install the shared plugin libary to the "Plugins" folder in the ManiVault install directory
install(TARGETS ${PROJECT}
    RUNTIME DESTINATION Plugins COMPONENT PLUGINS # Windows .dll
    LIBRARY DESTINATION Plugins COMPONENT PLUGINS # Linux/Mac .so
)

add_custom_command(TARGET ${PROJECT} POST_BUILD
    COMMAND "${CMAKE_COMMAND}"
        --install ${CMAKE_CURRENT_BINARY_DIR}
        --config $<CONFIGURATION>
        --prefix ${INSTALL_DIR}/$<CONFIGURATION>
)

# Automatically set the debug environment (command + working directory) for MSVC
if(MSVC)
    set_property(TARGET ${PROJECT} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY $<IF:$<CONFIG:DEBUG>,${INSTALL_DIR}/debug,${INSTALL_DIR}/release>)
    set_property(TARGET ${PROJECT} PROPERTY VS_DEBUGGER_COMMAND $<IF:$<CONFIG:DEBUG>,${INSTALL_DIR}/debug/HDPS.exe,${INSTALL_DIR}/release/HDPS.exe>)
endif()
