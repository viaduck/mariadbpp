//
//  M A R I A D B + +
//
//          Copyright The ViaDuck Project 2016 - 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef MARIADBCLIENTPP_GENERALTEST_H
#define MARIADBCLIENTPP_GENERALTEST_H

#include "SkeletonTest.h"

class GeneralTest : public SkeletonTest {
   protected:
    virtual void CreateTestTable() override {
        m_con->execute("CREATE TABLE " + m_table_name + " (id INT AUTO_INCREMENT, str VARCHAR(50) NULL, PRIMARY KEY(id));");
    }
};

#endif  // MARIADBCLIENTPP_GENERALTEST_H
