//
//  M A R I A D B + +
//
//          Copyright Sylvain Rochette Langlois 2013,
//                    Frantisek Boranek 2015,
//                    The ViaDuck Project 2016 - 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "worker.hpp"
#include <mariadb++/concurrency.hpp>

using namespace mariadb;
using namespace mariadb::concurrency;

//
// Constructors
//
worker::worker()
    : m_keep_handle(false),
      m_handle(0),
      m_status(status::removed),
      m_command(command::query),
      m_result(0) {}

worker::worker(account_ref& account, handle handle, bool keep_handle, command::type command,
               const std::string& query)
    : m_keep_handle(keep_handle),
      m_handle(handle),
      m_status(handle > 0 ? status::waiting : status::removed),
      m_command(command),
      m_result(0),
      m_query(query),
      m_account(account) {}

worker::worker(account_ref& account, handle handle, bool keep_handle, command::type command,
               statement_ref& statement)
    : m_keep_handle(keep_handle),
      m_handle(handle),
      m_status(handle > 0 ? status::waiting : status::removed),
      m_command(command),
      m_result(0),
      m_account(account),
      m_statement(statement) {}

//
// Get informations
//
status::type worker::status() const { return m_status; }

bool worker::keep_handle() const { return m_keep_handle; }

mariadb::handle worker::get_handle() const { return m_handle; }

//
// Get result / result_set
//
u64 worker::result() const { return m_result; }

result_set_ref worker::result_set() const { return m_result_set; }

//
// Do the actual job
//
void worker::execute() {
    m_status = status::executing;

    try {
        connection_ref connection =
            m_statement ? m_statement->m_connection : connection::create(m_account);

        //
        // Make sure auto commit mode is on before continuing...
        //
        connection->set_auto_commit(true);
        connection->connect();

        switch (m_command) {
            case command::execute:
                if (m_statement)
                    m_result = m_statement->execute();
                else
                    m_result = connection->execute(m_query.c_str());
                break;

            case command::insert:
                if (m_statement)
                    m_result = m_statement->insert();
                else
                    m_result = connection->insert(m_query.c_str());
                break;

            case command::query:
                if (m_statement)
                    m_result_set = m_statement->query();
                else
                    m_result_set = connection->query(m_query.c_str());
                break;
        }

        m_status = status::succeed;
    } catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
        m_status = status::failed;
    }
}
