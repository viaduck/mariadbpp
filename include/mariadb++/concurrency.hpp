//
//  M A R I A D B + +
//
//          Copyright Sylvain Rochette Langlois 2013,
//                    Frantisek Boranek 2015,
//                    The ViaDuck Project 2016 - 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef _MARIADB_CONCURRENCY_HPP_
#define _MARIADB_CONCURRENCY_HPP_

#include <vector>
#include <mariadb++/account.hpp>
#include <mariadb++/statement.hpp>

namespace mariadb {
namespace concurrency {
    namespace status {
        enum type { waiting, executing, succeed, failed, removed };
    }

    //
    // Set account for connection
    //
    extern void set_account(account_ref& account);

    //
    // Query status
    //
    extern status::type worker_status(handle h);

    //
    // Execute a query
    // Note: the void overloads are needed because it was too easy to "forget" passing keep_handle
    // and getting an invalid handle, instead we now return void when no handle is needed
    //
    extern handle execute(const std::string& query, bool keep_handle);
    inline void execute(const std::string& squery) { execute(squery, false); }

    extern handle insert(const std::string& query, bool keep_handle);
    inline void insert(const std::string& squery) { insert(squery, false); }

    extern handle query(const std::string& query, bool keep_handle);
    inline void query(const std::string& squery) { query(squery, false); }

    //
    // Execute a query using a statement
    // Note: the void overloads are needed because it was too easy to "forget" passing keep_handle
    // and getting an invalid handle, instead we now return void when no handle is needed
    //
    extern statement_ref create_statement(const std::string& query);
    extern handle execute(statement_ref& statement, bool keep_handle);
    inline void execute(statement_ref& statement) { execute(statement, false); }

    extern handle insert(statement_ref& statement, bool keep_handle);
    inline void insert(statement_ref& statement) { insert(statement, false); }

    extern handle query(statement_ref& statement, bool keep_handle);
    inline void query(statement_ref& statement) { query(statement, false); }

    //
    // Query executed, result ready to be used
    //
    extern u64 get_execute_result(handle h);
    extern u64 get_insert_result(handle h);
    extern result_set_ref get_query_result(handle h);

    //
    // Remove a query
    //
    // Please note, if a result_set is used, it must be done after the result_set is used...
    //
    extern void release_handle(handle h);

    //
    // Wait for a handle to signal
    //
    extern bool wait_handle(handle h, u64 wait_time_ms = 100);
}
}

#endif
