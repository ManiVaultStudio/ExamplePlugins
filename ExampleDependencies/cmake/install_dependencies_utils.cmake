function(install_dependencies main_target)
    
    set(MAIN_PROJECT_TARGET ${main_target})

    foreach(TARGET_STR IN LISTS ARGV)
        list(APPEND DEPENDENCIES_FOLDERS "$<TARGET_FILE_DIR:${TARGET_STR}>")
    endforeach()

    include(CMakePackageConfigHelpers)
    configure_file("${CMAKE_CURRENT_SOURCE_DIR}/cmake/install_dependencies.cmake.in" "${CMAKE_CURRENT_SOURCE_DIR}/cmake/install_dependencies.cmake" @ONLY)

    include(cmake/install_dependencies.cmake)
endfunction()
