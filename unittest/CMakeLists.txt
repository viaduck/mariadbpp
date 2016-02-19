PROJECT(mariadbclientpp_tests)

SET(mariadbclientpp_tests_SOURCES
	main.cpp
	decimal.cpp
	date_time.cpp
	database.cpp
	test.cpp
)

ADD_EXECUTABLE (mariadbclientpp_tests ${mariadbclientpp_tests_SOURCES})
TARGET_LINK_LIBRARIES(mariadbclientpp_tests mariadbclientpp)
