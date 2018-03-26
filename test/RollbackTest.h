//
//  M A R I A D B + +
//
//          Copyright The ViaDuck Project 2016 - 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef MARIADBCLIENTPP_ROLLBACKTEST_H
#define MARIADBCLIENTPP_ROLLBACKTEST_H

#include "SkeletonTest.h"

class RollbackTest : public SkeletonTest {
   protected:
    virtual void CreateTestTable() override {
        m_con->execute("CREATE TABLE " + m_table_name +
                       "(id INT PRIMARY KEY AUTO_INCREMENT, "
                       "data TINYBLOB NULL, "
                       "str VARCHAR(50) NULL, "
                       "dt DATETIME NULL, "
                       "t TIME NULL, "
                       "value INT NULL, "
                       "deci DECIMAL(8,4) DEFAULT 0.0);");
    }
};

#endif  // MARIADBCLIENTPP_ROLLBACKTEST_H
