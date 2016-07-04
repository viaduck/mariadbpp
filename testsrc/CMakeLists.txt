add_executable(mariadbpp_tests SkeletonTest.h SelectTest.cpp SelectTest.h AlterTableTest.cpp AlterTableTest.h)
target_link_libraries(mariadbpp_tests mariadbclientpp gtest_main)
