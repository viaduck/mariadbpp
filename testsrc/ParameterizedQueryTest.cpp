//
// Created by jonas on 04.07.16.
//

#include "ParameterizedQueryTest.h"

TEST_F(ParameterizedQueryTest, bindNormal)
{

	std::string query = "SELECT * FROM " + m_table_name + " WHERE preis>=?;";

	mariadb::statement_ref  selectQuery = m_con->create_statement(("SELECT * FROM " + m_table_name + " WHERE 1=?;").c_str());
	selectQuery->set_unsigned32(0, 1);

	mariadb::result_set_ref queryResult = selectQuery->query();


	ASSERT_TRUE(!!queryResult);
	ASSERT_TRUE(queryResult->next());
}

TEST_F(ParameterizedQueryTest, emptyBind)
{
	// Currently this test does fail because mariadbclient does not distinguish between a normal query with ? and a parameterized query...

	mariadb::statement_ref emptyQuery = m_con->create_statement(("SELECT * FROM " + m_table_name + " WHERE 1=?;").c_str());

	mariadb::result_set_ref queryResult = emptyQuery->query();
	ASSERT_FALSE(!!emptyQuery);
}

TEST_F(ParameterizedQueryTest, emptyBindQuery)
{
	// fixme: Invalid query object instead of exception maybe?
	EXPECT_ANY_THROW(mariadb::statement_ref emptyQuery = m_con->create_statement(""));
}

TEST_F(ParameterizedQueryTest, bindAfterQuery)
{
	mariadb::statement_ref errorQuery = m_con->create_statement(("SELECT * FROM " + m_table_name + " WHERE id = ?;").c_str());
	mariadb::result_set_ref queryResult = errorQuery->query();

	EXPECT_NO_THROW(errorQuery->set_unsigned32(0, 1));
}


TEST_F(ParameterizedQueryTest, bindTHISdataType)
{
	mariadb::statement_ref nonerrorQuery = m_con->create_statement("UPDATE ParameterizedQueryTest_bindAnyDataType SET preis = 299;");
	u64 rows = nonerrorQuery->execute();


	mariadb::statement_ref testQuery = m_con->create_statement("SELECT preis FROM ParameterizedQueryTest_bindAnyDataType;");
	mariadb::result_set_ref testResult = testQuery->query();
	ASSERT_TRUE(testResult->next());

	ASSERT_EQ(testResult->get_unsigned32((unsigned int) 0), 299);
}

TEST_F(ParameterizedQueryTest, bindAnyDataType)
{
	mariadb::statement_ref errorQuery;
	mariadb::statement_ref testQuery;
	mariadb::result_set_ref queryResult;

#define ParamTest_TEST(call, call2, name, value)  errorQuery = m_con->create_statement(("UPDATE " + m_table_name + " SET "+ std::string(name) +"= ?;").c_str()); \
	call; \
	errorQuery->execute(); \
	testQuery = m_con->create_statement(("SELECT " + std::string(name) + " FROM " + m_table_name + " WHERE id = 1;").c_str()); \
	queryResult = testQuery->query(); \
	ASSERT_TRUE(queryResult->next()); \
	ASSERT_EQ(call2, value);

	decimal d = decimal("0.02");
	mariadb::date_time t(mariadb::date_time::now());

	ParamTest_TEST(errorQuery->set_unsigned32(0, (unsigned int) 299), queryResult->get_unsigned32((unsigned int)0), "preis", 299);
	//ParamTest_TEST(errorQuery->set_string(0, "TESTSTRING"), queryResult->get_string((unsigned int)0), "str", "TESTSTRING");
	ParamTest_TEST(errorQuery->set_boolean(0, true), queryResult->get_boolean((unsigned int)0), "b", true);
	ParamTest_TEST(errorQuery->set_double(0, 0.03), queryResult->get_double((unsigned int)0), "dd", 0.03);
	ParamTest_TEST(errorQuery->set_signed32(0, 100), queryResult->get_signed32((unsigned int)0), "preis", 100);
	//ParamTest_TEST(errorQuery->set_date(0, t), queryResult->get_date((unsigned int)0), "tim", t);
	ParamTest_TEST(errorQuery->set_decimal(0, d), queryResult->get_decimal((unsigned int)0), "d", d);
	ParamTest_TEST(errorQuery->set_null(0), queryResult->is_null((unsigned int)0), "nul", true);
}

TEST_F(ParameterizedQueryTest, bindExecute)
{
	mariadb::statement_ref crashQuery = m_con->create_statement(("INSERT INTO " + m_table_name + " (id, preis) VALUES (2, ?);").c_str());
	crashQuery->set_unsigned32(0, 1);

	// Should not crash
	crashQuery->query();
}

TEST_F(ParameterizedQueryTest, bindDataBlob)
{
	mariadb::statement_ref errorQuery = m_con->create_statement(("SELECT * FROM " + m_table_name + " WHERE id = ?;").c_str());

	const char* c = reinterpret_cast<const char*>(new char[400]);
	errorQuery->set_data(0, mariadb::data_ref(new mariadb::data<char>(c, 400)));

	mariadb::result_set_ref queryResult = errorQuery->query();
	delete[] c;

	ASSERT_TRUE(!!queryResult);
	ASSERT_FALSE(queryResult->next());
}


TEST_F(ParameterizedQueryTest, bindDataBlobNullPtr)
{
	// Basically should not crash. Please.
	mariadb::statement_ref errorQuery = m_con->create_statement(("SELECT * FROM " + m_table_name + " WHERE id = ?;").c_str());
	errorQuery->set_data(0, nullptr);
}

TEST_F(ParameterizedQueryTest, bindWithoutParameters)
{
	mariadb::statement_ref errorQuery = m_con->create_statement("SELECT 1;");

	// Or Exception -> Basic range checking maybe
	EXPECT_ANY_THROW(errorQuery->set_unsigned32(1, 100));
}
