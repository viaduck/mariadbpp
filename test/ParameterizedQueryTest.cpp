//
//  M A R I A D B + +
//
//          Copyright The ViaDuck Project 2016 - 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "ParameterizedQueryTest.h"

TEST_F(ParameterizedQueryTest, bindNormal) {
    mariadb::statement_ref selectQuery =
        m_con->create_statement("SELECT * FROM " + m_table_name + " WHERE 1=?;");
    selectQuery->set_unsigned32(0, 1);

    mariadb::result_set_ref queryResult = selectQuery->query();

    ASSERT_TRUE(!!queryResult);
    ASSERT_TRUE(queryResult->next());
}

TEST_F(ParameterizedQueryTest, emptyBind) {
    mariadb::statement_ref emptyQuery =
        m_con->create_statement("SELECT * FROM " + m_table_name + " WHERE 1=?;");

    mariadb::result_set_ref queryResult = emptyQuery->query();
    ASSERT_TRUE(!!emptyQuery);
    ASSERT_TRUE(!!queryResult);
    ASSERT_FALSE(queryResult->next());
}

TEST_F(ParameterizedQueryTest, emptyBindQuery) {
    // fixme: Invalid query object instead of exception maybe?
    EXPECT_ANY_THROW(mariadb::statement_ref emptyQuery = m_con->create_statement(""));
}

TEST_F(ParameterizedQueryTest, bindAfterQuery) {
    mariadb::statement_ref errorQuery =
        m_con->create_statement("SELECT * FROM " + m_table_name + " WHERE id = ?;");
    mariadb::result_set_ref queryResult = errorQuery->query();

    EXPECT_NO_THROW(errorQuery->set_unsigned32(0, 1));
}

TEST_F(ParameterizedQueryTest, bindAnyDataType) {
    mariadb::statement_ref errorQuery;
    mariadb::statement_ref testQuery;
    mariadb::result_set_ref queryResult;

#define ParamTest_TEST(call, call2, name, value)                                                  \
    errorQuery = m_con->create_statement("UPDATE " + m_table_name + " SET " + name + "= ?;");     \
    call;                                                                                         \
    errorQuery->execute();                                                                        \
    testQuery = m_con->create_statement("SELECT " + std::string(name) + " FROM " + m_table_name + \
                                        " WHERE id = 1;");                                        \
    queryResult = testQuery->query();                                                             \
    ASSERT_TRUE(queryResult->next());                                                             \
    ASSERT_EQ(call2, value);

    decimal d = decimal("0.02");
    mariadb::date_time t(mariadb::date_time::now());
    mariadb::time ti(mariadb::time::now());

    ParamTest_TEST(errorQuery->set_unsigned32(0, (unsigned int)299), queryResult->get_unsigned32(0),
                   "preis", 299);
    ParamTest_TEST(errorQuery->set_string(0, "TESTSTRING"), queryResult->get_string(0), "str",
                   "TESTSTRING");
    ParamTest_TEST(errorQuery->set_boolean(0, true), queryResult->get_boolean(0), "b", true);
    ParamTest_TEST(errorQuery->set_double(0, 0.03), queryResult->get_double(0), "dd", 0.03);
    ParamTest_TEST(errorQuery->set_double(0, -0.03), queryResult->get_double(0), "dd", -0.03);
    ParamTest_TEST(errorQuery->set_signed32(0, 100), queryResult->get_signed32(0), "preis", 100);
    ParamTest_TEST(errorQuery->set_date_time(0, t), queryResult->get_date_time(0), "tim", t);
    ParamTest_TEST(errorQuery->set_time(0, ti), queryResult->get_time(0), "tiim", ti);
    ParamTest_TEST(errorQuery->set_decimal(0, d), queryResult->get_decimal(0).str(), "d", "0.02");
    ParamTest_TEST(errorQuery->set_null(0), queryResult->get_is_null(0), "nul", true);
}

TEST_F(ParameterizedQueryTest, bindExecute) {
    mariadb::statement_ref crashQuery =
        m_con->create_statement("INSERT INTO " + m_table_name + " (id, preis) VALUES (2, ?);");
    crashQuery->set_unsigned32(0, 1);

    crashQuery->query();
}

TEST_F(ParameterizedQueryTest, bindDataBlob) {
    mariadb::statement_ref errorQuery =
        m_con->create_statement("SELECT * FROM " + m_table_name + " WHERE id = ?;");

    const char* c =
        "0123456789012345678901234567890123456789"
        "0123456789012345678901234567890123456789"
        "0123456789012345678901234567890123456789"
        "0123456789012345678901234567890123456789"
        "0123456789012345678901234567890123456789"
        "0123456789012345678901234567890123456789"
        "0123456789012345678901234567890123456789"
        "0123456789012345678901234567890123456789"
        "0123456789012345678901234567890123456789"
        "0123456789012345678901234567890123456789";

    errorQuery->set_data(0, mariadb::data_ref(new mariadb::data<char>(c, 400)));

    mariadb::result_set_ref queryResult = errorQuery->query();

    ASSERT_TRUE(!!queryResult);
    ASSERT_FALSE(queryResult->next());
}

TEST_F(ParameterizedQueryTest, bindDataBlobNullPtr) {
    mariadb::statement_ref errorQuery =
        m_con->create_statement("SELECT * FROM " + m_table_name + " WHERE id = ?;");
    errorQuery->set_data(0, nullptr);
}

TEST_F(ParameterizedQueryTest, bindWithoutParameters) {
    mariadb::statement_ref errorQuery = m_con->create_statement("SELECT 1;");

    EXPECT_ANY_THROW(errorQuery->set_unsigned32(1, 100));
}

TEST_F(ParameterizedQueryTest, bindReuseSimple) {
    mariadb::statement_ref insertQuery = m_con->create_statement("INSERT INTO " + m_table_name + "(preis) VALUES(?);");

    // bind 1
    insertQuery->set_unsigned32(0, 177);
    u64 row1 = insertQuery->insert();

    // bind 2
    insertQuery->set_unsigned32(0, 1337);
    u64 row2 = insertQuery->insert();

    // bind 3 - reuse
    u64 row3 = insertQuery->insert();

    mariadb::statement_ref selectQuery = m_con->create_statement("SELECT preis FROM " + m_table_name + " WHERE id = ?");

    // test 1
    selectQuery->set_unsigned64(0, row1);
    mariadb::result_set_ref result = selectQuery->query();
    ASSERT_TRUE(!!result);
    ASSERT_TRUE(result->next());
    EXPECT_EQ(177u, result->get_unsigned32(0));

    // test2
    selectQuery->set_unsigned64(0, row2);
    mariadb::result_set_ref result2 = selectQuery->query();
    ASSERT_TRUE(!!result2);
    ASSERT_TRUE(result2->next());
    EXPECT_EQ(1337u, result2->get_unsigned32(0));

    // test3
    selectQuery->set_unsigned64(0, row3);
    mariadb::result_set_ref result3 = selectQuery->query();
    ASSERT_TRUE(!!result3);
    ASSERT_TRUE(result3->next());
    EXPECT_EQ(1337u, result3->get_unsigned32(0));
}

TEST_F(ParameterizedQueryTest, bindReuseString) {
    mariadb::statement_ref insertQuery = m_con->create_statement("INSERT INTO " + m_table_name + "(str) VALUES(?);");

    // bind 1
    insertQuery->set_string(0, "asdf");
    u64 row1 = insertQuery->insert();

    // bind 2
    insertQuery->set_string(0, "qwertz");
    u64 row2 = insertQuery->insert();

    // bind 3
    insertQuery->set_string(0, "");
    u64 row3 = insertQuery->insert();

    mariadb::statement_ref selectQuery = m_con->create_statement("SELECT str FROM " + m_table_name + " WHERE id = ?;");

    // test 1
    selectQuery->set_unsigned64(0, row1);
    mariadb::result_set_ref result = selectQuery->query();
    ASSERT_TRUE(!!result);
    ASSERT_TRUE(result->next());
    EXPECT_EQ("asdf", result->get_string(0));

    // test2
    selectQuery->set_unsigned64(0, row2);
    mariadb::result_set_ref result2 = selectQuery->query();
    ASSERT_TRUE(!!result2);
    ASSERT_TRUE(result2->next());
    EXPECT_EQ("qwertz", result2->get_string(0));

    // test3
    selectQuery->set_unsigned64(0, row3);
    mariadb::result_set_ref result3 = selectQuery->query();
    ASSERT_TRUE(!!result3);
    ASSERT_TRUE(result3->next());
    EXPECT_EQ("", result3->get_string(0));
}