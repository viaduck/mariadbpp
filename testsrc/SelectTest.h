//
// Created by John Watson on 04.07.16.
//

#ifndef MARIADBCLIENTPP_SELECTTEST_H
#define MARIADBCLIENTPP_SELECTTEST_H


#include "SkeletonTest.h"

class SelectTest : public SkeletonTest {
    virtual void CreateTestTable() override {
        m_con->execute("CREATE TABLE " + m_table_name + " (id INT AUTO_INCREMENT, PRIMARY KEY (`id`));");
    }
};


#endif //MARIADBCLIENTPP_SELECTTEST_H
