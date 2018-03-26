//
//  M A R I A D B + +
//
//          Copyright The ViaDuck Project 2016 - 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "TruncationTest.h"

TEST_F(TruncationTest, testUInt) {
    uint32_t max = std::numeric_limits<uint32_t>::max();

    // insert max uint
    m_con->execute("INSERT INTO " + m_table_name + " VALUES (" + std::to_string(max) + ");");

    // query directly
    result_set_ref res = m_con->query("SELECT * FROM " + m_table_name + ";");

    ASSERT_TRUE(!!res);
    ASSERT_TRUE(res->next());
    ASSERT_EQ(max, res->get_unsigned32(0));

    // query as statement
    statement_ref stmt = m_con->create_statement("SELECT * FROM " + m_table_name + ";");
    result_set_ref stmt_res = stmt->query();

    ASSERT_TRUE(!!stmt_res);
    ASSERT_TRUE(stmt_res->next());
    ASSERT_EQ(max, stmt_res->get_unsigned32(0));
}