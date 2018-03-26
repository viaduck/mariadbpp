//
//  M A R I A D B + +
//
//          Copyright The ViaDuck Project 2016 - 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "AlterTableTest.h"

void AlterTableTest::CreateTestTable() {
    m_con->execute("CREATE TABLE " + m_table_name +
                   " (id INT AUTO_INCREMENT, PRIMARY KEY (`id`));");
}

TEST_F(AlterTableTest, AlterAddAndDrop_existing) {
    m_con->execute("ALTER TABLE " + m_table_name + " ADD new INT;");
    m_con->execute("ALTER TABLE " + m_table_name + " DROP COLUMN new;");
}

TEST_F(AlterTableTest, AlterAddAndModify_existing) {
    m_con->execute("ALTER TABLE " + m_table_name + " ADD new INT;");
    m_con->execute("ALTER TABLE " + m_table_name + " MODIFY new DOUBLE NOT NULL;");
}

TEST_F(AlterTableTest, AlterAdd_non_existing_table) {
    EXPECT_ANY_THROW(m_con->execute("ALTER TABLE doesntexist ADD new INT;"));
}

TEST_F(AlterTableTest, AlterDrop_non_existing_table) {
    EXPECT_ANY_THROW(m_con->execute("ALTER TABLE doesntexist DROP COLUMN new;"));
}

TEST_F(AlterTableTest, AlterModify_non_existing_table) {
    EXPECT_ANY_THROW(m_con->execute("ALTER TABLE doesntexist MODIFY new DOUBLE;"));
}

TEST_F(AlterTableTest, AlterModify_non_existing_column) {
    m_con->execute("ALTER TABLE " + m_table_name + " ADD new INT;");
    EXPECT_ANY_THROW(
        m_con->execute("ALTER TABLE " + m_table_name + " MODIFY new_doesntexist DOUBLE;"));
}

TEST_F(AlterTableTest, AlterAddAndDrop_no_connect) {
    account_ref no_acc = account::create("256.256.256.256", "", "");
    connection_ref no_conn = connection::create(no_acc);

    EXPECT_ANY_THROW(no_conn->execute("ALTER TABLE " + m_table_name + " ADD new INT;"));
    EXPECT_ANY_THROW(no_conn->execute("ALTER TABLE " + m_table_name + " DROP COLUMN new;"));
}
