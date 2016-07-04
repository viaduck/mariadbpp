add_executable(mariadbpp_tests SkeletonTest.h SelectTest.cpp SelectTest.h GeneralTest.cpp GeneralTest.h
                AlterTableTest.cpp AlterTableTest.h ParameterizedQueryTest.cpp ParameterizedQueryTest.h)

target_link_libraries(mariadbpp_tests mariadbclientpp gtest_main)
