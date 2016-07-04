//
// Created by John Watson on 04.07.16.
//

#include "GeneralTest.h"

TEST_F(GeneralTest, testCreateFail) {
    // intended syntax error
    ASSERT_ANY_THROW(m_con->execute("CREATE TAVBEL testtest ();"));
    ASSERT_ANY_THROW(m_con->execute("CREATE TABLE testtest (\");"));
    ASSERT_ANY_THROW(m_con->execute("CREATE TABLE testtest¸ ()"));
    ASSERT_ANY_THROW(m_con->execute("CREATE TABLE ()"));
}

TEST_F(GeneralTest, testPermissions) {
    ASSERT_ANY_THROW(m_con->execute("CREATE DATABASE testtest;"));
    ASSERT_ANY_THROW(m_con->execute("DROP DATABASE testtest;"));
}

TEST_F(GeneralTest, testMissingConnection) {
    // create connection without connecting
    account_ref no_acc = account::create("0.0.0.0", "", "");
    connection_ref no_conn = connection::create(no_acc);

    ASSERT_FALSE(no_conn->connected());
    EXPECT_ANY_THROW(no_conn->execute("CREATE TABLE asdf;"));
    EXPECT_ANY_THROW(no_conn->query("SELECT * FROM asdf;"));
    EXPECT_ANY_THROW(no_conn->insert("INSERT INTO asdf (a) VALUES ('a');"));

    EXPECT_ANY_THROW(no_conn->create_statement("SELECT * FROM asdf;"));
}

TEST_F(GeneralTest, testDuplicateTable) {
    EXPECT_ANY_THROW(m_con->execute("CREATE TABLE " + m_table_name + " (id INT AUTO_INCREMENT, PRIMARY KEY(id));"));
}