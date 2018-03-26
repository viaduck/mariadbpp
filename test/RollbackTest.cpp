//
//  M A R I A D B + +
//
//          Copyright The ViaDuck Project 2016 - 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "RollbackTest.h"

TEST_F(RollbackTest, testTransactCommit) {
    transaction_ref trx = m_con->create_transaction();

    u64 id = m_con->insert("INSERT INTO " + m_table_name + "(str) VALUES('test');");
    EXPECT_NE(0, id);
    trx->commit();

    result_set_ref rs = m_con->query("SELECT COUNT(*) FROM " + m_table_name + ";");
    EXPECT_TRUE(!!rs);
    EXPECT_TRUE(rs->next());
    EXPECT_EQ(1, rs->get_unsigned64(0));
}

TEST_F(RollbackTest, testTransactionRollback) {
    // force transaction out of scope to destruct it and trigger automatic rollback
    {
        transaction_ref trx = m_con->create_transaction();
        m_con->insert("INSERT INTO " + m_table_name + "(str) VALUES('test2');");
    }

    result_set_ref rs = m_con->query("SELECT COUNT(*) FROM " + m_table_name + ";");
    EXPECT_TRUE(!!rs);
    EXPECT_TRUE(rs->next());
    EXPECT_EQ(0, rs->get_unsigned64(0));
}

TEST_F(RollbackTest, testSavePointCommit) {
    transaction_ref trx = m_con->create_transaction();
    {
        save_point_ref sp = trx->create_save_point();
        u64 id = m_con->insert("INSERT INTO " + m_table_name + "(str) VALUES('test');");
        EXPECT_NE(0, id);
        sp->commit();
    }
    trx->commit();

    result_set_ref rs = m_con->query("SELECT COUNT(*) FROM " + m_table_name + ";");
    EXPECT_TRUE(!!rs);
    EXPECT_TRUE(rs->next());
    EXPECT_EQ(1, rs->get_unsigned64(0));
}

TEST_F(RollbackTest, testSavePointNoCommit) {
    transaction_ref trx = m_con->create_transaction();
    {
        save_point_ref sp = trx->create_save_point();
        u64 id = m_con->insert("INSERT INTO " + m_table_name + "(str) VALUES('test');");
        EXPECT_NE(0, id);
    }
    trx->commit();

    result_set_ref rs = m_con->query("SELECT COUNT(*) FROM " + m_table_name + ";");
    EXPECT_TRUE(!!rs);
    EXPECT_TRUE(rs->next());
    EXPECT_EQ(0, rs->get_unsigned64(0));
}

TEST_F(RollbackTest, testMultiInsertIntegration) {
    std::string queries[] = {
        "INSERT INTO " + m_table_name + " (data) VALUES(?);",
        "INSERT INTO " + m_table_name + " (data, str) VALUES(?, ?);",
        "INSERT INTO " + m_table_name + " (data, str, dt) VALUES(?, ?, ?);",
        "INSERT INTO " + m_table_name + " (data, str, dt, t) VALUES(?, ?, ?, ?);",
        "INSERT INTO " + m_table_name + " (data, str, dt, t, value) VALUES(?, ?, ?, ?, ?);",
        "INSERT INTO " + m_table_name +
            " (data, str, dt, t, value, deci) VALUES(?, ?, ?, ?, ?, ?);",
        ""};

    const char *content =
        "01234567890123456789012345678901234567890123456789"
        "01234567890123456789012345678901234567890123456789";

    u32 index = 0;

    while (queries[index] != "") {
        statement_ref sta = m_con->create_statement(queries[index]);
        data_ref data = data_ref(new mariadb::data<char>(content, 100));

        sta->set_data(0, data);

        if (index >= 1) sta->set_string(1, "test");
        if (index >= 2) sta->set_date_time(2, mariadb::date_time(2000, 1, 2, 3, 4, 5));
        if (index >= 3) sta->set_time(3, mariadb::time(11, 22, 33));
        if (index >= 4) sta->set_signed32(4, 666);
        if (index >= 5) sta->set_decimal(5, decimal("1.1234"));
        EXPECT_NE(0, sta->insert());

        index++;
        result_set_ref rs = m_con->query("SELECT COUNT(*) FROM " + m_table_name + ";");
        EXPECT_TRUE(!!rs);
        EXPECT_TRUE(rs->next());
        EXPECT_EQ(index, rs->get_unsigned64(0));
    }

    //
    // Validate the inserted value from last test
    //

    result_set_ref rs = m_con->query("SELECT data, str, dt, t, value, deci FROM " + m_table_name +
                                     " ORDER BY id DESC LIMIT 1;");

    EXPECT_TRUE(!!rs);
    EXPECT_TRUE(rs->next());

    data_ref data = rs->get_data("data");
    EXPECT_TRUE(!!data);
    EXPECT_EQ(100, data->size());

    EXPECT_EQ("test", rs->get_string("str"));
    EXPECT_EQ(666, rs->get_signed32("value"));
    EXPECT_EQ("1.1234", rs->get_decimal("deci").str());
    EXPECT_EQ("2000-01-02 03:04:05", rs->get_date_time("dt").str());
    EXPECT_EQ("11:22:33", rs->get_time("t").str_time());
}