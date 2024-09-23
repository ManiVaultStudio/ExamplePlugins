cmake_minimum_required(VERSION 3.17)

option(MV_UNITY_BUILD "Combine target source files into batches for faster compilation" OFF)

# -----------------------------------------------------------------------------
# ExampleAnalysis Plugin
# -----------------------------------------------------------------------------
PROJECT("ExampleDependenciesPlugin")

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

include(cmake/get_cpm.cmake)

# -----------------------------------------------------------------------------
# Dependencies
# -----------------------------------------------------------------------------
find_package(Qt6 COMPONENTS Widgets WebEngineWidgets REQUIRED)

find_package(ManiVault COMPONENTS Core PointData CONFIG)

find_package(blake3 CONFIG REQUIRED)

if(NOT APPLE)
    find_package(faiss CONFIG REQUIRED)
endif()

CPMAddPackage(
  NAME highway
  URL https://github.com/google/highway/archive/refs/tags/1.2.0.tar.gz
  URL_HASH SHA256=7e0be78b8318e8bdbf6fa545d2ecb4c90f947df03f7aadc42c1967f019e63343
  PATCHES "cmake/highway.patch" # see https://github.com/conan-io/conan-center-index/pull/24197/files, fixes https://github.com/google/highway/issues/2225
  OPTIONS "HWY_ENABLE_EXAMPLES OFF" "HWY_ENABLE_INSTALL OFF" "HWY_ENABLE_TESTS OFF" "HWY_ENABLE_CONTRIB ON" "BUILD_SHARED_LIBS ON" 
)

# -----------------------------------------------------------------------------
# Source files
# -----------------------------------------------------------------------------
# Define the plugin sources
set(PLUGIN_SOURCES
    src/ExampleDependenciesPlugin.h
    src/ExampleDependenciesPlugin.cpp
    src/SettingsAction.h
    src/SettingsAction.cpp
    src/ExampleDependenciesPlugin.json
)

source_group( Plugin FILES ${PLUGIN_SOURCES})

# -----------------------------------------------------------------------------
# CMake Target
# -----------------------------------------------------------------------------
# Create dynamic library for the plugin
add_library(${PROJECT_NAME} SHARED ${PLUGIN_SOURCES})

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

target_link_libraries(${PROJECT_NAME} PRIVATE BLAKE3::blake3)
target_link_libraries(${PROJECT_NAME} PRIVATE hwy hwy_contrib)

if(NOT APPLE)
    target_link_libraries(${PROJECT_NAME} PRIVATE faiss)
endif()

# -----------------------------------------------------------------------------
# Target installation
# -----------------------------------------------------------------------------
# Install the shared plugin libary to the "Plugins" folder in the ManiVault install directory
install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION Plugins COMPONENT PLUGINS # Windows .dll
    LIBRARY DESTINATION Plugins COMPONENT PLUGINS # Linux/Mac .so
)

if(WIN32)
  set(PRE_EXCLUDE ".*[\\/]api-ms.*" ".*[\\/]ext-ms.*" ".*[\\/]hvsi.*" ".*[\\/]pdmutilities.*" ".*[\\/]wpaxholder.*" ".*[\\/]dxgi.*" ".*[\\/]uxtheme.*" ".*[\\/]d3d11.*" ".*[\\/]winmm.*" ".*[\\/]wldp.*")
  set(POST_EXCLUDE ".*[\\/]WINDOWS[\\/]system32[\\/].*\\.dll")
elseif(APPLE)
  set(PRE_EXCLUDE "/usr/lib" "/System/")
  set(POST_EXCLUDE "")
else()
  set(PRE_EXCLUDE ".*/ld-linux-.*" ".*/linux-vdso.*" ".*/libm\\..*" ".*/libc\\..*" ".*/libpthread\\..*" ".*/libdl\\..*")
  set(POST_EXCLUDE "")
endif()

set(deps_folder "bin/${PROJECT_NAME}")

message(STATUS "Install deps:: ${ManiVault_INSTALL_DIR}/$<CONFIGURATION>/${deps_folder}")

install(CODE [[
    function(install_deps LIBRARY)
    file(INSTALL
      DESTINATION "${CMAKE_INSTALL_PREFIX}/PluginDependencies/ExampleDependenciesPlugin"
      TYPE SHARED_LIBRARY
      FOLLOW_SYMLINK_CHAIN
      FILES "${LIBRARY}"
    )
    endfunction()
  file(GET_RUNTIME_DEPENDENCIES
    EXECUTABLES "$<TARGET_FILE:ExampleDependenciesPlugin>"
    RESOLVED_DEPENDENCIES_VAR RESOLVED_DEPS
    UNRESOLVED_DEPENDENCIES_VAR UNRESOLVED_DEPS
    CONFLICTING_DEPENDENCIES_PREFIX CONFLICTING_DEPS
    DIRECTORIES "$<TARGET_FILE_DIR:ExampleDependenciesPlugin>" "$<TARGET_FILE_DIR:hwy>" "$<TARGET_FILE_DIR:hwy_contrib>"
    PRE_EXCLUDE_REGEXES ".*[\\/]api-ms.*" ".*[\\/]ext-ms.*" ".*[\\/]hvsi.*" ".*[\\/]pdmutilities.*" ".*[\\/]wpaxholder.*" ".*[\\/]dxgi.*" ".*[\\/]uxtheme.*" ".*[\\/]d3d11.*" ".*[\\/]winmm.*" ".*[\\/]wldp.*"
    POST_EXCLUDE_REGEXES ".*[\\/]WINDOWS[\\/]system32[\\/].*\\.dll"
  )

  foreach(FILE ${RESOLVED_DEPS})
    message(STATUS "Resolved: ${FILE}")
    install_deps(${FILE})
  endforeach()

  foreach(FILE ${UNRESOLVED_DEPS})
    message(STATUS "Unresolved: ${FILE}")
  endforeach()

  if(CONFLICTING_DEPS)
    message(WARNING "Conflicting dependencies!")
    foreach(FILE ${CONFLICTING_DEPS})
      message(STATUS "Conflicting: ${FILE}")
    endforeach()
  endif()
  
  ]]
  COMPONENT DEP_PLUGIN_PRINT
)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E echo "Installing: Create dir"
    COMMAND ${CMAKE_COMMAND} -E make_directory ${ManiVault_INSTALL_DIR}/$<CONFIGURATION>/${deps_folder}
    COMMAND "${CMAKE_COMMAND}"
        --install ${CMAKE_CURRENT_BINARY_DIR}
        --config $<CONFIGURATION>
        --component PLUGINS
        --prefix ${ManiVault_INSTALL_DIR}/$<CONFIGURATION>
        --verbose
    COMMAND "${CMAKE_COMMAND}"
        --install ${CMAKE_CURRENT_BINARY_DIR}
        --config $<CONFIGURATION>
        --component DEP_PLUGIN_PRINT
        --prefix ${ManiVault_INSTALL_DIR}/$<CONFIGURATION>
        --verbose
)

# -----------------------------------------------------------------------------
# Miscellaneous
# -----------------------------------------------------------------------------
# Automatically set the debug environment (command + working directory) for MSVC
if(MSVC)
    set_property(TARGET ${PROJECT_NAME} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY $<IF:$<CONFIG:DEBUG>,${ManiVault_INSTALL_DIR}/debug,${ManiVault_INSTALL_DIR}/release>)
    set_property(TARGET ${PROJECT_NAME} PROPERTY VS_DEBUGGER_COMMAND $<IF:$<CONFIG:DEBUG>,${ManiVault_INSTALL_DIR}/debug/ManiVault\ Studio.exe,${ManiVault_INSTALL_DIR}/release/ManiVault\ Studio.exe>)
endif()
