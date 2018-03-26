//
//  M A R I A D B + +
//
//          Copyright Sylvain Rochette Langlois 2013,
//                    Frantisek Boranek 2015,
//                    The ViaDuck Project 2016 - 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef _MARIADB_WORKER_HPP_
#define _MARIADB_WORKER_HPP_

#include <mariadb++/connection.hpp>
#include <mariadb++/concurrency.hpp>

namespace mariadb {
using namespace concurrency;

namespace command {
enum type { execute, insert, query };
}

//
// Worker entity used by concurrency namespace
//
class worker {
   public:
    //
    // Constructor
    //
    worker();
    worker(account_ref& account, handle hnd, bool keep_handle, command::type command,
           const std::string& query);
    worker(account_ref& account, handle hnd, bool keep_handle, command::type command,
           statement_ref& statement);

    //
    // Get informations
    //
    status::type status() const;
    handle get_handle() const;
    bool keep_handle() const;

    //
    // Get result / result_set
    //
    u64 result() const;
    result_set_ref result_set() const;

    //
    // Do the actual job
    //
    void execute();

   private:
    bool m_keep_handle;
    handle m_handle;
    status::type m_status;
    command::type m_command;
    u64 m_result;
    std::string m_query;
    account_ref m_account;
    result_set_ref m_result_set;
    statement_ref m_statement;
};
}

#endif
