#          Copyright The ViaDuck Project 2016 - 2024.
# Distributed under the Boost Software License, Version 1.0.
#    (See accompanying file LICENSE or copy at
#          http://www.boost.org/LICENSE_1_0.txt)

cmake_minimum_required(VERSION 3.8...3.16)
project(mariadbclientpp LANGUAGES CXX)

include(GNUInstallDirs)
option(MARIADBPP_TEST "Build mariadbpp tests" OFF)
option(MARIADBPP_DOC "Build mariadbpp docs" OFF)
option(MARIADBPP_QUIET "Turn off error logging" OFF)

# add additional cmake modules
list(INSERT CMAKE_MODULE_PATH 0 "${CMAKE_CURRENT_SOURCE_DIR}/external/cmake-modules")

# dependencies
find_package(MariaDBClient REQUIRED)
find_package(Threads REQUIRED)

# find files
file(GLOB_RECURSE MARIADBPP_PUBLIC_HEADERS include/mariadb++/*)
file(GLOB_RECURSE MARIADBPP_FILES src/*.hpp src/*.cpp)

# set up target
add_library(mariadbclientpp ${MARIADBPP_FILES} ${MARIADBPP_PUBLIC_HEADERS})
# include headers
target_include_directories(mariadbclientpp PUBLIC
    "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>"
    "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>")
# link dependencies
target_link_libraries(mariadbclientpp MariaDBClient::MariaDBClient Threads::Threads)
# compile options
target_compile_features(mariadbclientpp PUBLIC cxx_std_11)
target_compile_definitions(mariadbclientpp PUBLIC MARIADB_QUIET=$<BOOL:${MARIADBPP_QUIET}>)

if (MSVC)
    target_compile_options(mariadbclientpp PRIVATE /W4)
else()
    target_compile_options(mariadbclientpp PRIVATE -Wall -Wextra)
endif()

# install configuration
install(FILES ${MARIADBPP_PUBLIC_HEADERS} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/mariadb++)
install(TARGETS mariadbclientpp
    EXPORT mariadbclientpp_export
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

configure_file("cmake/mariadbclientpp-config.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/mariadbclientpp-config.cmake" @ONLY)

install(EXPORT mariadbclientpp_export
    DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/mariadbclientpp"
    NAMESPACE "mariadbclientpp::"
    # In this CMake config file no dependencies are considered. But since we
    # do not use any `find_package` call here this approach is sufficient.
    FILE mariadbclientpp-targets.cmake
)

install(FILES 
    "${CMAKE_CURRENT_BINARY_DIR}/mariadbclientpp-config.cmake"
    "${PROJECT_SOURCE_DIR}/external/cmake-modules/FindMariaDBClient.cmake"
    DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/mariadbclientpp"
)

SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "MariaDB++")
SET(CPACK_PACKAGE_VENDOR "ViaDuck")
SET(CPACK_PACKAGE_VERSION "1.0.0")
SET(CPACK_GENERATOR "RPM;DEB")
SET(CPACK_DEBIAN_PACKAGE_MAINTAINER "Nobody")
INCLUDE(CPack)

# tests
if (MARIADBPP_TEST)
    add_subdirectory(test)
endif()

if (MARIADBPP_DOC)
    # doxygen
    include(Doxygen)
    if (DOXYGEN_FOUND)
        setup_doxygen(mariadbpp_doc ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in)
    endif()
endif()
