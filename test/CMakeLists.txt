#          Copyright The ViaDuck Project 2016 - 2024.
# Distributed under the Boost Software License, Version 1.0.
#    (See accompanying file LICENSE or copy at
#          http://www.boost.org/LICENSE_1_0.txt)

# variables for running tests
set(TEST_HOSTNAME "localhost" CACHE STRING "Hostname for mariadbpp tests")
set(TEST_PORT 3306 CACHE STRING "Port for mariadbpp tests")
set(TEST_UNIXSOCKET "" CACHE STRING "Unix socket for mariadbpp tests")
set(TEST_USERNAME "testuser" CACHE STRING "Database username for mariadbpp tests")
set(TEST_PASSWORD "bockwurst" CACHE STRING "Database username's password for mariadbpp tests")
set(TEST_DATABASE "mariadbpptest" CACHE STRING "Database name for mariadbpp test")

# write config to header in binary dir
configure_file(test_config.h.in test_config.h)

include(FindOrBuildGTest)
if (GTEST_FOUND)
    # collect test files
    file(GLOB_RECURSE MARIADBPP_TEST_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.h ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

    macro(make_viaduck_test_target name)
        # create test target as base for the san
        add_executable(${name} ${MARIADBPP_FILES} ${MARIADBPP_TEST_FILES})
        # includes
        target_include_directories(${name} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})
        # links
        target_link_libraries(${name} PUBLIC mariadbclientpp ${GTEST_TARGET})
    endmacro()
    make_viaduck_test_target(mariadbpp_test)

    include(EnableSanitizers)
    enable_sanitizers_for_target(mariadbpp_test)

    include(EnableCoverage)
    enable_coverage_for_target(mariadbpp_test ${CMAKE_CURRENT_SOURCE_DIR}/../
        "test/*" "external/*" "${CMAKE_BINARY_DIR}/*" "${GTEST_SRC_DIR}/*")
endif()
