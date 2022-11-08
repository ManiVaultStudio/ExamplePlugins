cmake_minimum_required(VERSION 3.1.0)

set(PROJECT "ExampleViewPlugin")

PROJECT(${PROJECT})

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /DWIN32 /EHsc /MP")
	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /NODEFAULTLIB:LIBCMT")
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MDd")
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MD")
endif(MSVC)

file(TO_CMAKE_PATH $ENV{HDPS_INSTALL_DIR} INSTALL_DIR)

find_package(Qt6 COMPONENTS Widgets WebEngineWidgets REQUIRED)

set(SOURCES
    src/ExampleViewPlugin.h
    src/ExampleViewPlugin.cpp
    src/ExampleViewPlugin.json
)

source_group( Plugin FILES ${SOURCES})

include_directories("${INSTALL_DIR}/$<CONFIGURATION>/include/")

add_library(${PROJECT} SHARED ${SOURCES})

target_link_libraries(${PROJECT} Qt6::Widgets)
target_link_libraries(${PROJECT} Qt6::WebEngineWidgets)
target_link_libraries(${PROJECT} "${INSTALL_DIR}/$<CONFIGURATION>/lib/${CMAKE_SHARED_LIBRARY_PREFIX}HDPS_Public.lib")
target_link_libraries(${PROJECT} "${INSTALL_DIR}/$<CONFIGURATION>/lib/${CMAKE_SHARED_LIBRARY_PREFIX}PointData.lib")

add_custom_command(TARGET ${PROJECT} POST_BUILD
    COMMAND "${CMAKE_COMMAND}" -E copy_if_different
	"$<TARGET_FILE:${PROJECT}>"
	"${INSTALL_DIR}/$<CONFIGURATION>/Plugins/$<TARGET_FILE_NAME:${PROJECT}>"
)

# Automatically set the debug environment (command + working directory) for MSVC in debug mode
set_property(TARGET ${PROJECT} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY $<IF:$<CONFIG:DEBUG>,${INSTALL_DIR}/debug,${INSTALL_DIR}/release>)
set_property(TARGET ${PROJECT} PROPERTY VS_DEBUGGER_COMMAND $<IF:$<CONFIG:DEBUG>,${INSTALL_DIR}/debug/HDPS.exe,${INSTALL_DIR}/release/HDPS.exe>)