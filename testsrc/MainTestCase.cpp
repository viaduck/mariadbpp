//
// Created by pcpcpc on 05.02.2016.
//

#include <mariadb++/statement.hpp>
#include <mariadb++/connection.hpp>
#include <mariadb++/concurrency.hpp>
#include <thread>
#include "MainTestCase.h"

using namespace mariadb;

int main() {
    MainTestCase css;
    return css.run() + css.run_stress();
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

    data_ref gd = rs2->get_data(0u);
    if(!gd)
        return 3;

    if(gd->size() != 20)
        return 4;

    for (int i = 0; i < 20; i++) {
        if(gd->get()[i] != (char)i)
            return 5;
    }

    std::cout << std::to_string(rs2->get_signed32(1)) << std::endl;

    return 0;
}

int MainTestCase::run_stress() {
    account_ref m_account_setup;
    uint32_t insert_test_count = 2000;

    m_account_setup = account::create("127.0.0.1", "test", "test", "test", 3306, NULL);
    concurrency::set_account(m_account_setup);

    char binary_pid[20];
    for (int i = 0; i < 20; ++i) {
        binary_pid[i] = (char)i;
    }

    data_ref my_data(new data<char>(binary_pid, 20));

    statement_ref sta = concurrency::create_statement("INSERT INTO messages (binary_pid, mid) SELECT ? as binary_pid, IFNULL(MAX(mid),-1)+1 as mid FROM messages WHERE binary_pid=?;");
    for (int j = 0; j < insert_test_count; ++j) {
        sta->set_data(0, my_data);
        sta->set_data(1, my_data);
        concurrency::insert(sta);
    }

    std::this_thread::sleep_for(std::chrono::seconds(10));

    return 0;
}
