project(mariadbclientpp)

cmake_minimum_required(VERSION 2.8)

#####################################################################################
set(mariadb_LIBRARIES mysqlclient)

#####################################################################################
# dependencies

find_package(MySQL REQUIRED)

include_directories(
	"${CMAKE_CURRENT_SOURCE_DIR}/include"
	"${CMAKE_CURRENT_BINARY_DIR}/include"
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
target_link_libraries(mariadbclientpp ${mariadb_LIBRARIES} pthread)

install(TARGETS mariadbclientpp
	ARCHIVE DESTINATION lib
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
)
install(FILES ${mariadbclientpp_PUBLIC_HEADERS}
	DESTINATION include/mariadb++
)

add_subdirectory(unittest)
add_subdirectory(testsrc)
add_subdirectory(external/googletest)
