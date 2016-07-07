project(mariadbclientpp)

cmake_minimum_required(VERSION 2.8)

# add additional cmake modules
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/external/cmake-modules")


#####################################################################################
set(mariadb_LIBRARIES mysqlclient)

#####################################################################################
# dependencies

#find_package(MySQL REQUIRED)

include_directories(
	"${CMAKE_CURRENT_SOURCE_DIR}/include"
	"${CMAKE_CURRENT_BINARY_DIR}/include"
	"${CMAKE_CURRENT_SOURCE_DIR}/external/googletest/include"
)

set(MARIADB_USES_EXCEPTION ON CACHED)

if(MARIADB_USES_EXCEPTION)
	add_definitions(-DMARIADB_USES_EXCEPTION)
endif()


if (CMAKE_VERSION VERSION_LESS "3.1")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
else()
	set(CMAKE_CXX_STANDARD 11)
endif()

file(GLOB mariadbclientpp_PUBLIC_HEADERS include/mariadb++/*)
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

add_subdirectory(external/googletest)
add_subdirectory(testsrc)
