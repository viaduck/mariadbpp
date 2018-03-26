//
//  M A R I A D B + +
//
//          Copyright The ViaDuck Project 2016 - 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef MARIADBCLIENTPP_TRUNCATIONTEST_H
#define MARIADBCLIENTPP_TRUNCATIONTEST_H

#include "SkeletonTest.h"

class TruncationTest : public SkeletonTest {
   protected:
    void CreateTestTable() override {
        m_con->execute("CREATE TABLE " + m_table_name + " (id INT UNSIGNED);");
    }
};

#endif  // MARIADBCLIENTPP_TRUNCATIONTEST_H
