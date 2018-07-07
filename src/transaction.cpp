//
//  M A R I A D B + +
//
//          Copyright Sylvain Rochette Langlois 2013,
//                    Frantisek Boranek 2015,
//                    The ViaDuck Project 2016 - 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <mysql.h>
#include <mariadb++/connection.hpp>
#include <mariadb++/transaction.hpp>

using namespace mariadb;

namespace {
const char* g_isolation_level[] = {
    "SET TRANSACTION ISOLATION LEVEL REPEATABLE READ;",
    "SET TRANSACTION ISOLATION LEVEL READ COMMITTED;",
    "SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED;",
    "SET TRANSACTION ISOLATION LEVEL SERIALIZABLE;",
};

const char* g_consistent_snapshot[] = {
    "START TRANSACTION;", "START TRANSACTION WITH CONSISTENT SNAPSHOT;",
};
}

transaction::transaction(connection* conn, isolation::level level, bool consistent_snapshot)
    : m_connection(conn) {
    conn->execute(g_isolation_level[level]);
    conn->execute(g_consistent_snapshot[consistent_snapshot]);
}

transaction::~transaction() {
    if (!m_connection) return;

    mysql_rollback(m_connection->m_mysql);
    cleanup();
}

void transaction::cleanup() {
    for (save_point* save_point : m_save_points) save_point->m_transaction = nullptr;

    m_save_points.clear();
}

void transaction::commit() {
    if (!m_connection) return;

    mysql_commit(m_connection->m_mysql);
    cleanup();
    m_connection = nullptr;
}

save_point_ref transaction::create_save_point() {
    if (!m_connection) return save_point_ref();

    save_point* sp = new save_point(this);
    m_save_points.push_back(sp);
    return save_point_ref(sp);
}

void transaction::remove_save_point(save_point* sv_point) {
    m_save_points.erase(std::remove(m_save_points.begin(), m_save_points.end(), sv_point));
}
