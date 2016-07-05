//
// Created by John Watson on 04.07.16.
//

#ifndef MARIADBCLIENTPP_GENERALTEST_H
#define MARIADBCLIENTPP_GENERALTEST_H


#include "SkeletonTest.h"

class GeneralTest : public SkeletonTest {

protected:
    virtual void CreateTestTable() override {
        m_con->execute("CREATE TABLE " + m_table_name + " (id INT AUTO_INCREMENT, str VARCHAR(50) NULL, PRIMARY KEY(id));");
    }
};


#endif //MARIADBCLIENTPP_GENERALTEST_H
