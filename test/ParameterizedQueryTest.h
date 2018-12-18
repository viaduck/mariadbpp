//
//  M A R I A D B + +
//
//          Copyright The ViaDuck Project 2016 - 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef MARIADBCLIENTPP_PARAMETERIZEDQUERYTEST_H
#define MARIADBCLIENTPP_PARAMETERIZEDQUERYTEST_H

#include "SkeletonTest.h"

class ParameterizedQueryTest : public SkeletonTest {
    virtual void CreateTestTable() override {
        m_con->execute("CREATE TABLE " + m_table_name +
                       " (id INT AUTO_INCREMENT, preis INT, str VARCHAR(30), b BOOL, tim "
                       "DATETIME(6), tiim TIME(6), d DECIMAL(3,2), dd DOUBLE, nul INT, PRIMARY KEY (id));");
        m_con->execute("INSERT INTO " + m_table_name + " (id, preis) VALUES (1, 150);");
    }
};

#endif  // MARIADBCLIENTPP_PARAMETERIZEDQUERYTEST_H
