cmake_minimum_required(VERSION 3.17)

# -----------------------------------------------------------------------------
# ExampleAnalysis Plugin
# -----------------------------------------------------------------------------
set(EXAMPLEANALYSIS "ExampleAnalysisPlugin")
PROJECT(${EXAMPLEANALYSIS})

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
# Set install directory
# -----------------------------------------------------------------------------
# Get the env path and allow the user to change it
set(INSTALL_DIR $ENV{HDPS_INSTALL_DIR} CACHE PATH "The HDPS Install Directoy")
# Normalize the incoming install path
file(TO_CMAKE_PATH ${INSTALL_DIR} INSTALL_DIR)

# -----------------------------------------------------------------------------
# Dependencies
# -----------------------------------------------------------------------------
find_package(Qt6 COMPONENTS Widgets WebEngineWidgets REQUIRED)

# -----------------------------------------------------------------------------
# Source files
# -----------------------------------------------------------------------------
# Define the plugin sources
set(EXAMPLEANALYSIS_SOURCES
    src/ExampleAnalysisPlugin.h
    src/ExampleAnalysisPlugin.cpp
    src/SettingsAction.h
    src/SettingsAction.cpp
    src/ExampleAnalysisPlugin.json
)

set(PLUGIN_MOC_HEADERS
	src/ExampleAnalysisPlugin.h
)

source_group( Plugin FILES ${EXAMPLEANALYSIS_SOURCES})

# -----------------------------------------------------------------------------
# CMake Target
# -----------------------------------------------------------------------------
# Create dynamic library for the plugin
add_library(${EXAMPLEANALYSIS} SHARED ${EXAMPLEANALYSIS_SOURCES})

qt_wrap_cpp(EXAMPLEANALYSIS_MOC ${PLUGIN_MOC_HEADERS} TARGET ${EXAMPLEANALYSIS})
target_sources(${EXAMPLEANALYSIS} PRIVATE ${EXAMPLEANALYSIS_MOC})

# -----------------------------------------------------------------------------
# Target include directories
# -----------------------------------------------------------------------------
# Include HDPS headers, including system data plugins
target_include_directories(${EXAMPLEANALYSIS} PRIVATE "${INSTALL_DIR}/$<CONFIGURATION>/include/")

# -----------------------------------------------------------------------------
# Target properties
# -----------------------------------------------------------------------------
# Request C++17
target_compile_features(${EXAMPLEANALYSIS} PRIVATE cxx_std_17)

# Link to Qt libraries
target_link_libraries(${EXAMPLEANALYSIS} PRIVATE Qt6::Widgets)
target_link_libraries(${EXAMPLEANALYSIS} PRIVATE Qt6::WebEngineWidgets)

# -----------------------------------------------------------------------------
# Target library linking
# -----------------------------------------------------------------------------
# Link to HDPS and data plugins
# The link path in this repo assumes that the ManiVault core was built locally
# in contrast to having been installed with an installer. In the latter case you'll have
# to adapt the HDPS_LINK_PATH and PLUGIN_LINK_PATH to your install folder
set(HDPS_LINK_PATH "${INSTALL_DIR}/$<CONFIGURATION>/lib")
set(PLUGIN_LINK_PATH "${INSTALL_DIR}/$<CONFIGURATION>/$<IF:$<CXX_COMPILER_ID:MSVC>,lib,Plugins>")
set(HDPS_LINK_SUFFIX $<IF:$<CXX_COMPILER_ID:MSVC>,${CMAKE_LINK_LIBRARY_SUFFIX},${CMAKE_SHARED_LIBRARY_SUFFIX}>)

set(HDPS_LINK_LIBRARY "${HDPS_LINK_PATH}/${CMAKE_SHARED_LIBRARY_PREFIX}HDPS_Public${HDPS_LINK_SUFFIX}")
set(POINTDATA_LINK_LIBRARY "${PLUGIN_LINK_PATH}/${CMAKE_SHARED_LIBRARY_PREFIX}PointData${HDPS_LINK_SUFFIX}") 

target_link_libraries(${EXAMPLEANALYSIS} PRIVATE "${HDPS_LINK_LIBRARY}")
target_link_libraries(${EXAMPLEANALYSIS} PRIVATE "${POINTDATA_LINK_LIBRARY}")

# -----------------------------------------------------------------------------
# Target installation
# -----------------------------------------------------------------------------
# Install the shared plugin libary to the "Plugins" folder in the ManiVault install directory
install(TARGETS ${EXAMPLEANALYSIS}
    RUNTIME DESTINATION Plugins COMPONENT PLUGINS # Windows .dll
    LIBRARY DESTINATION Plugins COMPONENT PLUGINS # Linux/Mac .so
)

add_custom_command(TARGET ${EXAMPLEANALYSIS} POST_BUILD
    COMMAND "${CMAKE_COMMAND}"
        --install ${CMAKE_CURRENT_BINARY_DIR}
        --config $<CONFIGURATION>
        --prefix ${INSTALL_DIR}/$<CONFIGURATION>
)

# -----------------------------------------------------------------------------
# Miscellaneous
# -----------------------------------------------------------------------------
# Automatically set the debug environment (command + working directory) for MSVC
if(MSVC)
    set_property(TARGET ${EXAMPLEANALYSIS} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY $<IF:$<CONFIG:DEBUG>,${INSTALL_DIR}/debug,${INSTALL_DIR}/release>)
    set_property(TARGET ${EXAMPLEANALYSIS} PROPERTY VS_DEBUGGER_COMMAND $<IF:$<CONFIG:DEBUG>,${INSTALL_DIR}/debug/HDPS.exe,${INSTALL_DIR}/release/HDPS.exe>)
endif()
