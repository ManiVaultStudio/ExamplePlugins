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

# Add the core cmake scripts - need this on the CI with vcpkg
# Note the core is called mv-core (due tocore being a vcpkg reserved name)
# TODO find a better mechanism
if(DEFINED VCPKG_TARGET_TRIPLET)
  message(STATUS "ManiVault installed ${ManiVault_INSTALL_DIR}") 
  set(ManiVault_DIR "${ManiVault_INSTALL_DIR}/cmake/mv")
  message(STATUS "ManiVault core at ${ManiVault_DIR}")
  include("${ManiVault_DIR}/mv_install_dependencies_utils.cmake")
  include("${ManiVault_DIR}/mv_plugin_meta_data.cmake")
endif()