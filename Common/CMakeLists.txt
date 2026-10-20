set(DIR ${CMAKE_CURRENT_SOURCE_DIR})

set(COMMON_HEADERS
    ${DIR}/common.h
	PARENT_SCOPE
)

set(COMMON_SOURCES
    ${DIR}/common.cpp
	PARENT_SCOPE
)

set(COMMON_FILES
    ${COMMON_HEADERS}
    ${COMMON_SOURCES}
	PARENT_SCOPE
)

if (DEFINED ENV{CI})
  set(temp_prefix_path ${CMAKE_PREFIX_PATH})
  message(STATUS "vcpkg search paths: ${CMAKE_PREFIX_PATH}")
  foreach(pkgpath ${temp_prefix_path})
    file(GLOB_RECURSE ALL_FILES CONFIGURE_DEPENDS "${pkgpath}/*")
    list(APPEND CMAKE_PREFIX_PATH "${pkgpath}/cmake/mv")
        message(STATUS "CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH}")
  endforeach()
  set(VCPKG_TRACE_FIND_PACKAGE ON)
endif()

find_package(ManiVault COMPONENTS Core CONFIG QUIET)

# Add the core cmake scripts - need this on the CI with vcpkg
# Note the core is called mv-core (due tocore being a vcpkg reserved name)
# TODO find a better mechanism
if(DEFINED VCPKG_TARGET_TRIPLET)
  message(STATUS "ManiVault found flag: ${ManiVault_FOUND}")
  message(STATUS "ManiVault installed dir: ${ManiVault_INSTALL_DIR}") 
  set(ManiVault_DIR "${ManiVault_INSTALL_DIR}/cmake/mv")
  message(STATUS "ManiVault core at: ${ManiVault_DIR}")
  include("${ManiVault_DIR}/mv_install_dependencies_utils.cmake")
  include("${ManiVault_DIR}/mv_plugin_meta_data.cmake")
  include("${ManiVault_DIR}/mv_project_defaults.cmake")
endif()

