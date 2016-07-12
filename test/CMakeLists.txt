set(TEST_HOSTNAME "localhost" CACHE STRING "Hostname for mariadbpp tests")
set(TEST_PORT 3306 CACHE INTEGER "Port for mariadbpp tests")
set(TEST_UNIXSOCKET "" CACHE STRING "Unix socket for mariadbpp tests")
set(TEST_USERNAME "testuser" CACHE STRING "Database username for mariadbpp tests")
set(TEST_PASSWORD "bockwurst" CACHE STRING "Database username's password for mariadbpp tests")
set(TEST_DATABASE "mariadbpptest" CACHE STRING "Database name for mariadbpp test")

configure_file(test_config.h.in test_config.h)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

### coverage
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 -fprofile-arcs -ftest-coverage")


add_executable(mariadbpp_tests ${mariadbclientpp_SOURCES} ${mariadbclientpp_PUBLIC_HEADERS} SkeletonTest.h SelectTest.cpp
                SelectTest.h GeneralTest.cpp GeneralTest.h AlterTableTest.cpp AlterTableTest.h ParameterizedQueryTest.cpp
                ParameterizedQueryTest.h StructureTest.cpp StructureTest.h RollbackTest.cpp RollbackTest.h test_config.h
                TimeTest.cpp TimeTest.h)

target_link_libraries(mariadbpp_tests mariadbclientpp gtest_main)

## code coverage
include(CodeCoverage)
if (LCOV_PATH)
    setup_target_for_coverage(mariadbpp_coverage mariadbpp_tests ${CMAKE_CURRENT_BINARY_DIR}/coverage)
endif()
