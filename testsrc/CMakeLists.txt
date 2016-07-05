set(TEST_HOSTNAME "localhost" CACHE STRING "Hostname for mariadbpp tests")
set(TEST_PORT 3306 CACHE INTEGER "Port for mariadbpp tests")
set(TEST_UNIXSOCKET "" CACHE STRING "Unix socket for mariadbpp tests")
set(TEST_USERNAME "testuser" CACHE STRING "Database username for mariadbpp tests")
set(TEST_PASSWORD "bockwurst" CACHE STRING "Database username's password for mariadbpp tests")
set(TEST_DATABASE "mariadbpptest" CACHE STRING "Database name for mariadbpp test")

configure_file(test_config.h.in test_config.h)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_executable(mariadbpp_tests SkeletonTest.h SelectTest.cpp SelectTest.h GeneralTest.cpp GeneralTest.h
                AlterTableTest.cpp AlterTableTest.h ParameterizedQueryTest.cpp ParameterizedQueryTest.h
                StructureTest.cpp StructureTest.h RollbackTest.cpp RollbackTest.h)

target_link_libraries(mariadbpp_tests mariadbclientpp gtest_main)
