#          Copyright The ViaDuck Project 2016 - 2018.
# Distributed under the Boost Software License, Version 1.0.
#    (See accompanying file LICENSE or copy at
#          http://www.boost.org/LICENSE_1_0.txt)

cmake_minimum_required(VERSION 3.1)
project(mariadbclientpp LANGUAGES CXX)

include(GNUInstallDirs)
option(MARIADBPP_TEST "Build mariadbpp tests" ON)
option(MARIADBPP_DOC "Build mariadbpp docs" ON)

# add additional cmake modules
list(INSERT CMAKE_MODULE_PATH 0 "${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules")

# dependencies
find_package(MariaDBClient REQUIRED)
find_package(Threads REQUIRED)

# find files
file(GLOB mariadbclientpp_PUBLIC_HEADERS include/mariadb++/*)
file(GLOB mariadbclientpp_PRIVATE_HEADERS src/*.hpp)
file(GLOB mariadbclientpp_SOURCES src/*.cpp)

# set up target
add_library(mariadbclientpp ${mariadbclientpp_SOURCES} ${mariadbclientpp_PUBLIC_HEADERS} ${mariadbclientpp_PRIVATE_HEADERS})

# target options
target_link_libraries(mariadbclientpp MariaDBClient::MariaDBClient Threads::Threads)

target_include_directories(
	mariadbclientpp
	PUBLIC
	"$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>"
	"$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>")

target_compile_features(mariadbclientpp
	PUBLIC cxx_std_11)

target_compile_options(mariadbclientpp PRIVATE -Wall -Wextra)

# install configuration
install(FILES ${mariadbclientpp_PUBLIC_HEADERS} DESTINATION include/mariadb++)
install(TARGETS mariadbclientpp
	EXPORT mariadbclientpp_export
	ARCHIVE DESTINATION lib
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
)

configure_file("cmake/mariadbclientpp-config.cmake.in"
	"${CMAKE_CURRENT_BINARY_DIR}/mariadbclientpp-config.cmake" @ONLY)

install(
	EXPORT mariadbclientpp_export
	DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/mariadbclientpp"
	NAMESPACE "mariadbclientpp::"
	# In this CMake config file no dependencies are considered. But since we
	# do not use any `find_package` call here this approach is sufficient.
	FILE mariadbclientpp-targets.cmake
)

install(FILES "${CMAKE_CURRENT_BINARY_DIR}/mariadbclientpp-config.cmake"
	"${PROJECT_SOURCE_DIR}/cmake/modules/FindMariaDBClient.cmake"
	DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/mariadbclientpp")

SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "MariaDB++")
SET(CPACK_PACKAGE_VENDOR "ViaDuck")
SET(CPACK_PACKAGE_VERSION "0.0.1")
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
