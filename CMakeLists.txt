project(mariadbclientpp)

cmake_minimum_required(VERSION 2.8)

#####################################################################################
# set(CMAKE_INSTALL_PREFIX "${CMAKE_CURRENT_BINARY_DIR}/install")
# set(MARIADB_WITHOUT_CPP11 ON)
# set(BOOST_ROOT d:/Workspace/git.mailserver.Work/bin/libs_x64/common/external)
# set(mariadb_LIBRARIES d:/Workspace/git.mailserver.Work/bin/libs_x64/private/external/lib/libmariadb.lib)
# set(mariadb_INCLUDE_DIRS d:/Workspace/git.mailserver.Work/bin/libs_x64/private/external/include/mariadb)

#####################################################################################
# dependencies

# Please set BOOST_ROOT to the root directory containing Boost or BOOST_INCLUDEDIR to the directory containing  Boost's headers.
find_package(Boost REQUIRED)

# Please set mariadb_LIBRARIES and mariadb_INCLUDE_DIRS

#####################################################################################

include_directories(
	"${CMAKE_CURRENT_SOURCE_DIR}/include"
	"${CMAKE_CURRENT_BINARY_DIR}/include"
	"${Boost_INCLUDE_DIRS}"
	"${mariadb_INCLUDE_DIRS}"
)

# generate config.h
if(MARIADB_WITHOUT_CPP11)
	set(VAL_MARIADB_WITHOUT_CPP11 "#define MARIADB_WITHOUT_CPP11")
endif()

if(MARIADB_USES_EXCEPTION)
	set(VAL_MARIADB_USES_EXCEPTION "#define MARIADB_USES_EXCEPTION")
endif()

configure_file(
	"${CMAKE_CURRENT_SOURCE_DIR}/src/config.h.in"
	"${CMAKE_CURRENT_BINARY_DIR}/include/mariadb++/config.h"
)

file(GLOB mariadbclientpp_PUBLIC_HEADERS include/mariadb++/*)
list(APPEND mariadbclientpp_PUBLIC_HEADERS "${CMAKE_CURRENT_BINARY_DIR}/include/mariadb++/config.h")
source_group("Public Header Files" FILES ${mariadbclientpp_PUBLIC_HEADERS})

file(GLOB mariadbclientpp_PRIVATE_HEADERS src/*.hpp)
source_group("Internal Header Files" FILES ${mariadbclientpp_PRIVATE_HEADERS})

file(GLOB mariadbclientpp_SOURCES src/*.cpp)
source_group("Source Files" FILES ${mariadbclientpp_SOURCES})

add_library(mariadbclientpp STATIC ${mariadbclientpp_SOURCES} ${mariadbclientpp_PUBLIC_HEADERS} ${mariadbclientpp_PRIVATE_HEADERS})
target_link_libraries(mariadbclientpp mariadb)

install(TARGETS mariadbclientpp
	ARCHIVE DESTINATION lib
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
)
install(FILES ${mariadbclientpp_PUBLIC_HEADERS}
	DESTINATION include/mariadb++
)

add_subdirectory(unittest)

