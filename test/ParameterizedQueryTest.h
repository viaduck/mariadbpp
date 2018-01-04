//
// Created by jonas on 04.07.16.
//

#ifndef MARIADBCLIENTPP_PARAMETERIZEDQUERYTEST_H
#define MARIADBCLIENTPP_PARAMETERIZEDQUERYTEST_H


#include "SkeletonTest.h"

class ParameterizedQueryTest : public SkeletonTest{
	virtual void CreateTestTable() override {
		m_con->execute(("CREATE TABLE " + m_table_name + " (id INT AUTO_INCREMENT, preis INT, str VARCHAR(30), b BOOL, tim DATETIME, d DECIMAL(3,2), dd DOUBLE, nul INT, PRIMARY KEY (id));").c_str());
		m_con->execute("INSERT INTO " + m_table_name + " (id, preis) VALUES (1, 150);");
	}
};


#endif //MARIADBCLIENTPP_PARAMETERIZEDQUERYTEST_H
