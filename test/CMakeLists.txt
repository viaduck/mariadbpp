#          Copyright The ViaDuck Project 2016 - 2018.
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
    file(GLOB mariadbpp_tests_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/*.h)
    file(GLOB mariadbpp_tests_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

    ### coverage
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0")
    if (NOT WIN32)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage")
    endif()

    include_directories(${GTEST_INCLUDE_DIRS})
    include_directories(${CMAKE_CURRENT_BINARY_DIR})

    add_executable(mariadbpp_tests ${mariadbclientpp_SOURCES} ${mariadbclientpp_PUBLIC_HEADERS}
                                        ${mariadbpp_tests_HEADERS} ${mariadbpp_tests_SOURCES})

    target_link_libraries(mariadbpp_tests mariadbclientpp gtest_main gtest)

    if (NOT WIN32 AND NOT ANDROID)
        include(CodeCoverage)

        if (LCOV_PATH)
            setup_target_for_coverage(mariadbpp_coverage mariadbpp_tests ${CMAKE_CURRENT_BINARY_DIR}/coverage)
        endif()
    endif()
endif()
