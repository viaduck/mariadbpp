#ifndef MARIADBCLIENTPP_TRUNCATIONTEST_H
#define MARIADBCLIENTPP_TRUNCATIONTEST_H


#include "SkeletonTest.h"

class TruncationTest : public SkeletonTest {
protected:
    void CreateTestTable() override {
        m_con->execute("CREATE TABLE " + m_table_name + " (id INT UNSIGNED);");
    }
};


#endif //MARIADBCLIENTPP_TRUNCATIONTEST_H
