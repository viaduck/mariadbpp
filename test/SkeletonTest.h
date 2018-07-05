//
//  M A R I A D B + +
//
//          Copyright The ViaDuck Project 2016 - 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef MARIADBCLIENTPP_SKELETONTEST_H
#define MARIADBCLIENTPP_SKELETONTEST_H

#include <mariadb++/connection.hpp>
#include <gtest/gtest.h>

#include "test_config.h"

using namespace mariadb;

class SkeletonTest : public ::testing::Test {
   public:
    virtual void SetUp() override {
        // get test names and concatenate them
        const ::testing::TestInfo* const test_info =
            ::testing::UnitTest::GetInstance()->current_test_info();
        m_table_name =
            std::string(test_info->test_case_name()) + "_" + std::string(test_info->name());

        // create user account
        using TestConfig = mariadb::testing::TestConfig;
        m_account_setup = account::create(TestConfig::Hostname, TestConfig::User, TestConfig::Password,
                TestConfig::Database, TestConfig::Port, TestConfig::UnixSocket);
        ASSERT_TRUE(!!m_account_setup);
        m_account_setup->set_auto_commit(true);
        m_account_setup->set_connect_option(MYSQL_OPT_CONNECT_TIMEOUT, 10);
        ASSERT_EQ(1u, m_account_setup->connect_options().size());

        // create database connection
        m_con = connection::create(m_account_setup);
        ASSERT_TRUE(!!m_con);
        ASSERT_TRUE(m_con->connect());
        ASSERT_TRUE(m_con->connected());

        // drop table and call creation
        m_con->execute("DROP TABLE IF EXISTS " + m_table_name);
        CreateTestTable();
    }

    virtual void TearDown() override { m_con->disconnect(); }

   protected:
    virtual void CreateTestTable() = 0;

    account_ref m_account_setup;
    std::string m_table_name;

    connection_ref m_con;
};

#endif  // MARIADBCLIENTPP_SKELETONTEST_H
