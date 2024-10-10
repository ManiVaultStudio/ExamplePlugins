if(DEFINED CMAKE_TOOLCHAIN_FILE AND NOT CMAKE_TOOLCHAIN_FILE MATCHES "vcpkg")
find_package(LAPACK CONFIG REQUIRED)
if(NOT TARGET LAPACK::LAPACK)
  add_library(LAPACK::LAPACK INTERFACE IMPORTED)
  set(_lapack_libs "${LAPACK_LIBRARIES}")
  if(_lapack_libs AND TARGET BLAS::BLAS)
    # remove the ${BLAS_LIBRARIES} from the interface and replace it
    # with the BLAS::BLAS target
    list(REMOVE_ITEM _lapack_libs "${BLAS_LIBRARIES}")
  endif()

  if(_lapack_libs)
    set_target_properties(LAPACK::LAPACK PROPERTIES
      INTERFACE_LINK_LIBRARIES "${_lapack_libs}"
    )
  endif()
  unset(_lapack_libs)
endif()
endif()
