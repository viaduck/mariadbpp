//
// Created by John Watson on 04.07.16.
//

#include "SelectTest.h"

TEST_F(SelectTest, selectEmptyTable) {
    result_set_ref res = m_con->query("SELECT * FROM " + m_table_name);

    ASSERT_TRUE(!!res);
    ASSERT_FALSE(res->next());
}