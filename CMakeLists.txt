project(mariadbclientpp)

cmake_minimum_required(VERSION 2.8)

#####################################################################################
# set(CMAKE_INSTALL_PREFIX "${CMAKE_CURRENT_BINARY_DIR}/install")
#set(mariadb_LIBRARIES ${CMAKE_CURRENT_SOURCE_DIR}/mariadb/libmariadb.a ${CMAKE_CURRENT_SOURCE_DIR}/mariadb/libmariadb.dll)
set(mariadb_LIBRARIES mysqlclient)
#set(mariadb_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/mariadb/include/)
set(mariadb_INCLUDE_DIRS /usr/include/mysql/)

#####################################################################################
# dependencies

# Please set mariadb_LIBRARIES and mariadb_INCLUDE_DIRS

#####################################################################################

include_directories(
	"${CMAKE_CURRENT_SOURCE_DIR}/include"
	"${CMAKE_CURRENT_BINARY_DIR}/include"
	"${mariadb_INCLUDE_DIRS}"
)

# generate config.h
set(MARIADB_USES_EXCEPTION ON)

if(MARIADB_USES_EXCEPTION)
	set(VAL_MARIADB_USES_EXCEPTION "#define MARIADB_USES_EXCEPTION")
endif()

configure_file(
	"${CMAKE_CURRENT_SOURCE_DIR}/src/config.h.in"
	"${CMAKE_CURRENT_BINARY_DIR}/include/mariadb++/config.h"
)

set(CMAKE_CXX_STANDARD 11)

file(GLOB mariadbclientpp_PUBLIC_HEADERS include/mariadb++/*)
list(APPEND mariadbclientpp_PUBLIC_HEADERS "${CMAKE_CURRENT_BINARY_DIR}/include/mariadb++/config.h")
source_group("Public Header Files" FILES ${mariadbclientpp_PUBLIC_HEADERS})

file(GLOB mariadbclientpp_PRIVATE_HEADERS src/*.hpp)
source_group("Internal Header Files" FILES ${mariadbclientpp_PRIVATE_HEADERS})

file(GLOB mariadbclientpp_SOURCES src/*.cpp)
source_group("Source Files" FILES ${mariadbclientpp_SOURCES})

add_library(mariadbclientpp STATIC ${mariadbclientpp_SOURCES} ${mariadbclientpp_PUBLIC_HEADERS} ${mariadbclientpp_PRIVATE_HEADERS})
target_link_libraries(mariadbclientpp ${mariadb_LIBRARIES})

install(TARGETS mariadbclientpp
	ARCHIVE DESTINATION lib
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
)
install(FILES ${mariadbclientpp_PUBLIC_HEADERS}
	DESTINATION include/mariadb++
)

set(BUILD_TESTS 1)
add_subdirectory(unittest)
add_subdirectory(testsrc)
