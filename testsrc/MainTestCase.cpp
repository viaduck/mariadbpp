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

    m_account_setup = account::create("127.0.0.1", "test", "test", "test", 3306, NULL);
    m_account_setup->set_auto_commit(true);

    connection_ref con = connection::create(m_account_setup);
    statement_ref sta = con->create_statement("INSERT INTO messages (binary_pid, mid) SELECT ? as binary_pid, IFNULL(MAX(mid),-1)+1 as mid FROM messages WHERE binary_pid=?;");

    char binary_pid[20];
    for (int i = 0; i < 20; ++i) {
        binary_pid[i] = (char)i;
    }

    sta->set_data(0, data_ref(new data<char>(binary_pid, 20)));
    sta->set_data(1, data_ref(new data<char>(binary_pid, 20)));

    u64 rs = sta->insert();
    if (rs == 0)
        return 1;

    result_set_ref rs2 = con->query("SELECT binary_pid, mid FROM messages WHERE iid=" + std::to_string(rs));
    if(!rs2 || !rs2->next())
        return 2;

    data_ref gd = rs2->get_data((u32)0);
    if(!gd)
        return 3;

    if(gd->size() != 20)
        return 4;

    for (int i = 0; i < 20; i++) {
        if(gd->get()[i] != (char)i)
            return 5;
    }

    std::cout << std::to_string(rs2->get_signed32((u32)1)) << std::endl;

    return 0;
}
