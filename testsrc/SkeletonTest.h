#ifndef MARIADBCLIENTPP_SKELETONTEST_H
#define MARIADBCLIENTPP_SKELETONTEST_H

#include <mariadb++/connection.hpp>
#include <gtest/gtest.h>

using namespace mariadb;

class SkeletonTest : public ::testing::Test {
    const char* m_hostname = "localhost";
    const char* m_unixsock = nullptr;
    const char* m_username = "test";
    const char* m_password = "test";
    const char* m_dbname = "test";

    uint32_t m_port = 3306;

public:
    virtual void SetUp() override {
        // get test names and concatenate them
        const ::testing::TestInfo* const test_info = ::testing::UnitTest::GetInstance()->current_test_info();
        m_table_name = std::string(test_info->test_case_name()) + "_" + std::string(test_info->name());

        // create user account
        m_account_setup = account::create(m_hostname, m_username, m_password, m_dbname, m_port, m_unixsock);
        ASSERT_TRUE(!!m_account_setup);
        m_account_setup->set_auto_commit(true);

        // create database connection
        m_con = connection::create(m_account_setup);
        ASSERT_TRUE(!!m_con);
        ASSERT_TRUE(m_con->connect());
        ASSERT_TRUE(m_con->connected());

        // drop table and call creation
        m_con->execute("DROP TABLE IF EXISTS " + m_table_name);
        CreateTestTable();
    }

    virtual void TearDown() override {
        m_con->disconnect();
    }

protected:
    virtual void CreateTestTable() = 0;

    account_ref m_account_setup;
    std::string m_table_name;

    connection_ref m_con;
};


#endif //MARIADBCLIENTPP_SKELETONTEST_H
