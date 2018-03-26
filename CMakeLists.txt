#          Copyright The ViaDuck Project 2016 - 2018.
# Distributed under the Boost Software License, Version 1.0.
#    (See accompanying file LICENSE or copy at
#          http://www.boost.org/LICENSE_1_0.txt)

cmake_minimum_required(VERSION 3.1)
project(mariadbclientpp)

# add additional cmake modules
list(INSERT CMAKE_MODULE_PATH 0 "${CMAKE_CURRENT_SOURCE_DIR}/external/cmake-modules")

# c++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# dependencies
find_library(mariadb_LIBRARY mariadbclient)
if (NOT mariadb_LIBRARY)

	find_library(mariadb_LIBRARY mysqlclient)
    if (NOT mariadb_LIBRARY)
        message(FATAL_ERROR "Could not find mysqlclient lib")
    endif()
endif()

# find files
file(GLOB mariadbclientpp_PUBLIC_HEADERS include/mariadb++/*)
file(GLOB mariadbclientpp_PRIVATE_HEADERS src/*.hpp)
file(GLOB mariadbclientpp_SOURCES src/*.cpp)

# set up target
add_library(mariadbclientpp STATIC ${mariadbclientpp_SOURCES} ${mariadbclientpp_PUBLIC_HEADERS} ${mariadbclientpp_PRIVATE_HEADERS})

# target options
target_link_libraries(mariadbclientpp ${mariadb_LIBRARY} pthread)
target_include_directories(mariadbclientpp PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

# install configuration
install(FILES ${mariadbclientpp_PUBLIC_HEADERS} DESTINATION include/mariadb++)
install(TARGETS mariadbclientpp
	ARCHIVE DESTINATION lib
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
)

# tests
add_subdirectory(test)

# doxygen
include(Doxygen)
if (DOXYGEN_FOUND)
	setup_doxygen(mariadbpp_doc ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in)
endif()