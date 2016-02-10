//
// Created by pcpcpc on 05.02.2016.
//

#include <mariadb++/statement.hpp>
#include <mariadb++/connection.hpp>
#include "MainTestCase.h"

using namespace mariadb;

int main() {
    MainTestCase css;
    return css.run();
}

int MainTestCase::run() {
    account_ref m_account_setup;

    m_account_setup = account::create("127.0.0.1", "testuser", "abc", "secchatd", 3306, NULL);
    m_account_setup->set_auto_commit(true);

    connection_ref con = connection::create(m_account_setup);
    statement_ref sta = con->create_statement("SELECT ?, ?;");
    sta->set_string(0, "a");
    sta->set_string(1, "b");

    result_set_ref rs = sta->query();
    if(!rs || !rs->next() || strcmp(rs->get_string((u32)0), "a") != 0 || strcmp(rs->get_string((u32)1), "b") != 0)
        return -1;

    return 0;
}
