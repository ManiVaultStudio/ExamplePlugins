cmake_minimum_required(VERSION 3.22)

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
    set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} /MD")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MD")
endif(MSVC)

include(cmake/get_cpm.cmake)

# -----------------------------------------------------------------------------
# Dependencies
# -----------------------------------------------------------------------------
find_package(Qt6 COMPONENTS Widgets WebEngineWidgets REQUIRED)

find_package(ManiVault COMPONENTS Core PointData CONFIG QUIET)

include(CheckCXXSourceCompiles)

# RISC-V only has 32-bit and 64-bit atomic instructions. GCC is supposed
# to convert smaller atomics to those larger ones via masking and
# shifting like LLVM, but it’s a known bug that it does not. This means
# anything that wants to use atomics on 1-byte or 2-byte types needs
# -latomic, but not 4-byte or 8-byte (though it does no harm).
set(atomic_code
    "
     #include <atomic>
     #include <cstdint>
     std::atomic<uint8_t> n8 (0); // riscv64
     std::atomic<uint64_t> n64 (0); // armel, mipsel, powerpc
     int main() {
       ++n8;
       ++n64;
       return 0;
     }")

message(STATUS "Test start.")

check_cxx_source_compiles("${atomic_code}" ATOMICS_LOCK_FREE_INSTRUCTIONS)

message(STATUS "Test end.")

if(ATOMICS_LOCK_FREE_INSTRUCTIONS)
  set(ATOMICS_FOUND TRUE)
  set(ATOMICS_LIBRARIES)
  message(STATUS "Great.")
else()
  set(CMAKE_REQUIRED_LIBRARIES "-latomic")
  check_cxx_source_compiles("${atomic_code}" ATOMICS_IN_LIBRARY)
  set(CMAKE_REQUIRED_LIBRARIES)
  if(ATOMICS_IN_LIBRARY)
    set(ATOMICS_LIBRARY atomic)
    include(FindPackageHandleStandardArgs)
    find_package_handle_standard_args(Atomics DEFAULT_MSG ATOMICS_LIBRARY)
    set(ATOMICS_LIBRARIES ${ATOMICS_LIBRARY})
    unset(ATOMICS_LIBRARY)
    message(STATUS "OK.")
  else()
    if(Atomics_FIND_REQUIRED)
      message(FATAL_ERROR "Neither lock free instructions nor -latomic found.")
    endif()
  endif()
endif()
unset(atomic_code)

CPMAddPackage(
  NAME highway
  URL https://github.com/google/highway/archive/refs/tags/1.2.0.tar.gz
  URL_HASH SHA256=7e0be78b8318e8bdbf6fa545d2ecb4c90f947df03f7aadc42c1967f019e63343
  PATCHES "cmake/highway.patch" # see https://github.com/conan-io/conan-center-index/pull/24197/files, fixes https://github.com/google/highway/issues/2225
  OPTIONS "HWY_ENABLE_EXAMPLES OFF" "HWY_ENABLE_INSTALL OFF" "HWY_ENABLE_TESTS OFF" "HWY_ENABLE_CONTRIB ON" "BUILD_SHARED_LIBS ON" 
)

if(${MV_EXAMPLES_USE_VCPKG})
    find_package(blake3 CONFIG REQUIRED)

    if(NOT APPLE)
        include(cmake/ci_fixes.cmake)
        find_package(faiss CONFIG REQUIRED)
    endif()
endif()

# -----------------------------------------------------------------------------
# Include Common directory
# -----------------------------------------------------------------------------
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../Common ${CMAKE_CURRENT_BINARY_DIR}/Common)

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

source_group(Plugin FILES ${PLUGIN_SOURCES})
source_group(Common FILES ${COMMON_FILES})

# -----------------------------------------------------------------------------
# CMake Target
# -----------------------------------------------------------------------------
# Create dynamic library for the plugin
add_library(${PROJECT_NAME} SHARED ${PLUGIN_SOURCES} ${COMMON_FILES})

# -----------------------------------------------------------------------------
# Target include directories
# -----------------------------------------------------------------------------
# Include ManiVault headers, including system data plugins
target_include_directories(${PROJECT_NAME} PRIVATE "${ManiVault_INCLUDE_DIR}")

# -----------------------------------------------------------------------------
# Target properties
# -----------------------------------------------------------------------------
# Request C++20
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)

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

target_link_libraries(${PROJECT_NAME} PRIVATE hwy hwy_contrib)

if(${MV_EXAMPLES_USE_VCPKG})
    target_link_libraries(${PROJECT_NAME} PRIVATE BLAKE3::blake3)

    if(NOT APPLE)
    target_link_libraries(${PROJECT_NAME} PRIVATE faiss)
    endif()
    
    target_compile_definitions (${PROJECT_NAME} PRIVATE __USE_VCPKG__)
endif()

# Not used here, but you can use this in your plugin
# automatically available when using find_package(ManiVault)
mv_check_and_set_AVX(${PROJECT_NAME} OFF)

# -----------------------------------------------------------------------------
# Target installation
# -----------------------------------------------------------------------------
# Install the shared plugin library to the "Plugins" folder in the ManiVault install directory
install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION Plugins COMPONENT PLUGINS # Windows .dll
    LIBRARY DESTINATION Plugins COMPONENT PLUGINS # Linux/Mac .so
)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E echo "Installing: Plugin files"
    COMMAND "${CMAKE_COMMAND}"
        --install ${CMAKE_CURRENT_BINARY_DIR}
        --config $<CONFIGURATION>
        --component PLUGINS
        --prefix ${ManiVault_INSTALL_DIR}/$<CONFIGURATION>
        --verbose
)

set_target_properties(hwy hwy_contrib hwy_list_targets hwy_test
    PROPERTIES
    FOLDER ExamplePlugins/Dependencies
)

# Automatically available when using find_package(ManiVault)
mv_install_dependencies(${PROJECT_NAME} "hwy" "hwy_contrib")

# -----------------------------------------------------------------------------
# Miscellaneous
# -----------------------------------------------------------------------------
# Automatically set the debug environment (command + working directory) for MSVC
if(MSVC)
    set_property(TARGET ${PROJECT_NAME} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY $<IF:$<CONFIG:DEBUG>,${ManiVault_INSTALL_DIR}/Debug,$<IF:$<CONFIG:RELWITHDEBINFO>,${ManiVault_INSTALL_DIR}/RelWithDebInfo,${ManiVault_INSTALL_DIR}/Release>>)
    set_property(TARGET ${PROJECT_NAME} PROPERTY VS_DEBUGGER_COMMAND $<IF:$<CONFIG:DEBUG>,"${ManiVault_INSTALL_DIR}/Debug/ManiVault Studio.exe",$<IF:$<CONFIG:RELWITHDEBINFO>,"${ManiVault_INSTALL_DIR}/RelWithDebInfo/ManiVault Studio.exe","${ManiVault_INSTALL_DIR}/Release/ManiVault Studio.exe">>)
endif()
