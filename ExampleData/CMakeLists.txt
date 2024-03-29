cmake_minimum_required(VERSION 3.17)

# -----------------------------------------------------------------------------
# ExampleData Plugin
# -----------------------------------------------------------------------------
set(EXAMPLEDATA "ExampleDataPlugin")
PROJECT(${EXAMPLEDATA})

# -----------------------------------------------------------------------------
# CMake Options and Setup
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

# Include cmake tools
include(GenerateExportHeader)

# -----------------------------------------------------------------------------
# Set install directory
# -----------------------------------------------------------------------------
# Check if the directory to the ManiVault installation has been provided
if(NOT DEFINED MV_INSTALL_DIR)
    set(MV_INSTALL_DIR "" CACHE PATH "Directory where ManiVault is installed")
    message(FATAL_ERROR "Please set MV_INSTALL_DIR to the directory where ManiVault is installed")
endif()
file(TO_CMAKE_PATH ${MV_INSTALL_DIR} MV_INSTALL_DIR)

# -----------------------------------------------------------------------------
# Dependencies
# -----------------------------------------------------------------------------
find_package(Qt6 COMPONENTS Widgets WebEngineWidgets REQUIRED)

# -----------------------------------------------------------------------------
# Source files
# -----------------------------------------------------------------------------
# Define the plugin sources
set(EXAMPLEDATA_SOURCES
    src/ExampleDataPlugin.h
    src/ExampleDataPlugin.cpp
    src/ExampleDataPlugin.json
)

set(EXAMPLE_DATA_HEADERS
    src/ExampleDataPlugin.h
)

source_group( Plugin FILES ${EXAMPLEDATA_SOURCES})

# -----------------------------------------------------------------------------
# CMake Target
# -----------------------------------------------------------------------------
# Create dynamic library for the plugin
add_library(${EXAMPLEDATA} SHARED ${EXAMPLEDATA_SOURCES})

target_sources(${EXAMPLEDATA} PRIVATE 
    ${EXAMPLEDATA_SOURCES}
)

# Generate a header file that contains the EXPORT macro for this library.
generate_export_header(${EXAMPLEDATA} EXPORT_FILE_NAME ${CMAKE_CURRENT_BINARY_DIR}/${EXAMPLEDATA}_export.h)

# Add the generated header to the plugin's header files
list(APPEND EXAMPLE_DATA_HEADERS ${CMAKE_CURRENT_BINARY_DIR}/${EXAMPLEDATA}_export.h)

# -----------------------------------------------------------------------------
# Target include directories
# -----------------------------------------------------------------------------
# Include ManiVault headers, including system data plugins
target_include_directories(${EXAMPLEDATA} PRIVATE "${MV_INSTALL_DIR}/$<CONFIGURATION>/include/")

# -----------------------------------------------------------------------------
# Target properties
# -----------------------------------------------------------------------------
# Request C++17
target_compile_features(${EXAMPLEDATA} PRIVATE cxx_std_17)

set_target_properties(${EXAMPLEDATA} PROPERTIES PUBLIC_HEADER "${EXAMPLE_DATA_HEADERS}")

# -----------------------------------------------------------------------------
# Target library linking
# -----------------------------------------------------------------------------
# Link to Qt libraries
target_link_libraries(${EXAMPLEDATA} PRIVATE Qt6::Widgets)
target_link_libraries(${EXAMPLEDATA} PRIVATE Qt6::WebEngineWidgets)

# Link to ManiVault and data plugins
# The link path in this repo assumes that the ManiVault core was built locally
# in contrast to having been installed with an installer. In the latter case you'll have
# to adapt the MV_LINK_PATH and PLUGIN_LINK_PATH to your install folder
set(MV_LINK_PATH "${MV_INSTALL_DIR}/$<CONFIGURATION>/lib")
set(PLUGIN_LINK_PATH "${MV_INSTALL_DIR}/$<CONFIGURATION>/$<IF:$<CXX_COMPILER_ID:MSVC>,lib,Plugins>")
set(MV_LINK_SUFFIX $<IF:$<CXX_COMPILER_ID:MSVC>,${CMAKE_LINK_LIBRARY_SUFFIX},${CMAKE_SHARED_LIBRARY_SUFFIX}>)

set(MV_LINK_LIBRARY "${MV_LINK_PATH}/${CMAKE_SHARED_LIBRARY_PREFIX}MV_Public${MV_LINK_SUFFIX}")
set(POINTDATA_LINK_LIBRARY "${PLUGIN_LINK_PATH}/${CMAKE_SHARED_LIBRARY_PREFIX}PointData${MV_LINK_SUFFIX}") 

target_link_libraries(${EXAMPLEDATA} PRIVATE "${MV_LINK_LIBRARY}")
target_link_libraries(${EXAMPLEDATA} PRIVATE "${POINTDATA_LINK_LIBRARY}")

# -----------------------------------------------------------------------------
# Target installation
# -----------------------------------------------------------------------------
# In contrast to other plugin types, for data plugins we want to install headers
# (as well as the dynamic link library on Windows) 
# in addition to the dynamic plugins libraries so that other plugins can link against this new data type
# more info: https://cmake.org/cmake/help/book/mastering-cmake/chapter/Install.html
install(TARGETS ${EXAMPLEDATA}
    RUNTIME DESTINATION Plugins COMPONENT PLUGINS # Windows .dll
    LIBRARY DESTINATION Plugins COMPONENT PLUGINS # Linux/Mac .so
    ARCHIVE DESTINATION lib     COMPONENT LINKLIB # Windows .lib
    PUBLIC_HEADER DESTINATION include/${EXAMPLEDATA} COMPONENT HEADERS
)

add_custom_command(TARGET ${EXAMPLEDATA} POST_BUILD
    COMMAND "${CMAKE_COMMAND}"
        --install ${CMAKE_CURRENT_BINARY_DIR}
        --config $<CONFIGURATION>
        --prefix ${MV_INSTALL_DIR}/$<CONFIGURATION>
)

# -----------------------------------------------------------------------------
# Miscellaneous
# -----------------------------------------------------------------------------
# Automatically set the debug environment (command + working directory) for MSVC
if(MSVC)
    set_property(TARGET ${EXAMPLEDATA} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY $<IF:$<CONFIG:DEBUG>,${MV_INSTALL_DIR}/debug,${MV_INSTALL_DIR}/release>)
    set_property(TARGET ${EXAMPLEDATA} PROPERTY VS_DEBUGGER_COMMAND $<IF:$<CONFIG:DEBUG>,"${MV_INSTALL_DIR}/debug/ManiVault Studio.exe","${MV_INSTALL_DIR}/release/ManiVault Studio.exe">)
endif()
