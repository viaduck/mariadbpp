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
#include "private.hpp"

using namespace mariadb;

#define MYSQL_ERROR_DISCONNECT(mysql)                 \
    {                                                 \
        m_last_error_no = mysql_errno(mysql);         \
        m_last_error = mysql_error(mysql);            \
        disconnect();                                 \
        MARIADB_ERROR(m_last_error_no, m_last_error); \
    }

connection::connection(const account_ref& account)
    : m_mysql(NULL), m_auto_commit(true), m_account(account) {}

connection_ref connection::create(const account_ref& account) {
    return connection_ref(new connection(account));
}

connection::~connection() { disconnect(); }

const std::string& connection::schema() const { return m_schema; }

bool connection::set_schema(const std::string& schema) {
    if (!connect()) return false;

    if (mysql_select_db(m_mysql, schema.c_str())) MYSQL_ERROR(m_mysql);

    m_schema = schema;
    return true;
}

const std::string& connection::charset() const { return m_charset; }

bool connection::set_charset(const std::string& value) {
    if (!connect()) return false;

    if (mysql_set_character_set(m_mysql, value.c_str())) MYSQL_ERROR(m_mysql);

    m_charset = value;
    return true;
}

bool connection::connected() const {
    if (m_mysql == nullptr)
        return false;
    else
        return !mysql_ping(m_mysql);
}

account_ref connection::account() const { return m_account; }

bool connection::auto_commit() const { return m_auto_commit; }

bool connection::set_auto_commit(bool auto_commit) {
    if (m_auto_commit == auto_commit) return true;

    if (!connect()) return false;

    if (mysql_autocommit(m_mysql, auto_commit)) MYSQL_ERROR(m_mysql);

    m_auto_commit = auto_commit;
    return true;
}

bool connection::connect() {
    if (connected()) return true;

    if (m_mysql == nullptr) {
        m_mysql = mysql_init(nullptr);

        if (!m_mysql) {
            MARIADB_ERROR(0, "Cannot create MYSQL object.");
        }
    }

    if (!m_account->ssl_key().empty()) {
        if (mysql_ssl_set(m_mysql, m_account->ssl_key().c_str(),
                          m_account->ssl_certificate().c_str(), m_account->ssl_ca().c_str(),
                          m_account->ssl_ca_path().c_str(), m_account->ssl_cipher().c_str()))
            MYSQL_ERROR(m_mysql);
    }

    //
    // set connect options
    //
    for (auto& pair : m_account->connect_options()) {
        if (0 != mysql_options(m_mysql, pair.first, pair.second->value()))
            MYSQL_ERROR_DISCONNECT(m_mysql);
    }

    if (!mysql_real_connect(
            m_mysql, m_account->host_name().c_str(), m_account->user_name().c_str(),
            m_account->password().c_str(), nullptr, m_account->port(),
            m_account->unix_socket().empty() ? nullptr : m_account->unix_socket().c_str(),
            CLIENT_MULTI_STATEMENTS))
        MYSQL_ERROR(m_mysql);

    if (!set_auto_commit(m_account->auto_commit())) MYSQL_ERROR_DISCONNECT(m_mysql);

    if (!m_account->schema().empty()) {
        if (!set_schema(m_account->schema().c_str())) MYSQL_ERROR_DISCONNECT(m_mysql);
    }

    //
    // Set options
    //
    for (auto& pair : m_account->options()) {
        if (1 != execute("SET OPTION " + pair.first + "=" + pair.second))
            MYSQL_ERROR_DISCONNECT(m_mysql);
    }

    return true;
}

void connection::disconnect() {
    if (!m_mysql) return;

    mysql_close(m_mysql);
    mysql_thread_end();  // mysql_init() call mysql_thread_init therefor it needed to clear memory
                         // when closed msql handle
    m_mysql = nullptr;
}

result_set_ref connection::query(const std::string& query) {
    result_set_ref rs;

    if (!connect()) return rs;

    if (mysql_real_query(m_mysql, query.c_str(), query.size())) {
        MYSQL_ERROR_NO_BRAKET(m_mysql);
    }

    rs.reset(new result_set(this));
    return rs;
}

u64 connection::execute(const std::string& query) {
    if (!connect()) return 0;

    u64 affected_rows = 0;

    if (mysql_real_query(m_mysql, query.c_str(), query.size())) {
        MYSQL_ERROR_NO_BRAKET(m_mysql);
    }

    int status;
    do {
        MYSQL_RES* result = mysql_store_result(m_mysql);

        if (result)
            mysql_free_result(result);
        else if (mysql_field_count(m_mysql) == 0)
            affected_rows += mysql_affected_rows(m_mysql);
        else {
            MYSQL_ERROR_NO_BRAKET(m_mysql);
        }

        status = mysql_next_result(m_mysql);
        if (status > 0) {
            MYSQL_ERROR_NO_BRAKET(m_mysql);
        }
    } while (status == 0);

    return affected_rows;
}

u64 connection::insert(const std::string& query) {
    if (!connect()) return 0;

    if (mysql_real_query(m_mysql, query.c_str(), query.size())) {
        MYSQL_ERROR_NO_BRAKET(m_mysql);
    }

    return mysql_insert_id(m_mysql);
}

statement_ref connection::create_statement(const std::string& query) {
    if (!connect()) return statement_ref();

    return statement_ref(new statement(this, query));
}

transaction_ref connection::create_transaction(isolation::level level, bool consistent_snapshot) {
    if (!connect()) return transaction_ref();

    return transaction_ref(new transaction(this, level, consistent_snapshot));
}
