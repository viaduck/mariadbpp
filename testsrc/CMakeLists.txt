add_executable(mariadbpp_tests SkeletonTest.h SelectTest.cpp SelectTest.h GeneralTest.cpp GeneralTest.h)
target_link_libraries(mariadbpp_tests mariadbclientpp gtest_main)
