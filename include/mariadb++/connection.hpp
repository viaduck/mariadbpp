//
//  M A R I A D B + +
//
//          Copyright Sylvain Rochette Langlois 2013,
//                    Frantisek Boranek 2015,
//                    The ViaDuck Project 2016 - 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef _MARIADB_CONNECTION_HPP_
#define _MARIADB_CONNECTION_HPP_

#include <string>
#include <mariadb++/account.hpp>
#include <mariadb++/statement.hpp>
#include <mariadb++/transaction.hpp>
#include <mariadb++/save_point.hpp>
#include <mariadb++/result_set.hpp>

#include <mysql.h>

namespace mariadb {
/**
 * Wraps a Database connection.
 */
class connection : public last_error {
    friend class result_set;
    friend class statement;
    friend class transaction;
    friend class save_point;

   public:
    /**
     * Destroys connection and automatically disconnects
     */
    virtual ~connection();

    /**
     * Actually connects to the database using given account, sets SSL and additional options as
     * well as auto commit
     *
     * @return True on success
     */
    bool connect();

    /**
     * Disconnects from the database
     */
    void disconnect();

    /**
     * Indicates whether the connection is active. Also detects stale connections
     *
     * @return True on active connection
     */
    bool connected() const;

    /**
     * Gets the account associated with this connection
     *
     * @return Reference to the account
     */
    account_ref account() const;

    /**
     * Gets the schema (database name)
     *
     * @return String containing the schema
     */
    const std::string& schema() const;

    /**
     * Sets the schema (database name).
     * The connection needs to be already established
     *
     * @param schema The new schema name
     * @return True on success
     */
    bool set_schema(const std::string& schema);

    /**
     * Gets the charset associated with this connection
     *
     * @return String containg the charset (see documentation of MariaDB for possible values)
     */
    const std::string& charset() const;

    /**
     * Sets the charset.
     * The connection needs to be already established
     *
     * @param value The new charset
     * @return True on success
     */
    bool set_charset(const std::string& value);

    /**
     * Execute a query without interest in a result.
     * The connection needs to be established.
     *
     * @param query SQL query to execute
     * @return The number of actually affected rows. 0 on error
     */
    u64 execute(const std::string& query);

    /**
     * Execute a query (usually, but not limited to INSERT) with interest for the last row id.
     * The connection needs to be established.
     *
     * @param query SQL query to execute
     * @return Last row id of the inserted row. 0 on error
     */
    u64 insert(const std::string& query);

    /**
     * Execute a query with an result (if no result is returned, the result_set will be empty).
     * The connection needs to be established.
     * Note: The result is only valid as long as the connection is valid.
     *
     * @param query SQL query to execute
     * @return Result of the query as result_set.
     */
    result_set_ref query(const std::string& query);

    /**
     * Gets the status of the auto_commit setting.
     *
     * @return True if auto_commit is enabled
     */
    bool auto_commit() const;

    /**
     * Sets the auto_commit setting. Default MariaDB setting is TRUE.
     * The connection needs to be established.
     * This setting controls the default behavior of holding all changes back until a COMMIT is
     * issued.
     * See MariaDB documentation for further information on this setting.
     *
     * @return True on success
     */
    bool set_auto_commit(bool auto_commit);

    /**
     * Create a prepared statement. Use "?" to issue bindings in the query which can then be filled
     * in.
     * For information on how to properly use prepared statements please refer to the MariaDB
     * manual.
     * The connection needs to be established.
     * Note that "?" bindings can only be established at certain locations in a SQL statement.
     * A misplaced "?" will result in an error when executing the statement.
     * The statement is only valid as long as the connection is valid.
     *
     * @return Reference to the created statement.
     */
    statement_ref create_statement(const std::string& query);

    /**
     * Create a transaction. Any change to the database will be held back until you COMMIT the
     * transaction.
     * A not committed transaction automatically rolls all changes back on destruction.
     * Note: the transaction is only valid as long as this connection is valid.
     *
     * @param level The level of isolation to set while using this transaction. Defaults to
     * repeatable read
     * @param consistent_snapshot Indicates whether to require a consistent snapshot before entering
     * the transaction.
     * Note: refer to MariaDB manual for further information
     *
     * @return Reference to the created transaction.
     */
    transaction_ref create_transaction(isolation::level level = isolation::repeatable_read,
                                       bool consistent_snapshot = true);

    /**
     * Creates a new connection using the given account
     *
     * @param account The account used to provide the connection information
     * @return Reference to the newly created connection
     */
    static connection_ref create(const account_ref& account);

   private:
    /**
     * Private constructor used to create a connection with the given account
     */
    connection(const account_ref& account);

   private:
    // internal database connection pointer
    MYSQL* m_mysql;

    // state of auto_commit setting
    bool m_auto_commit;
    // name of current schema
    std::string m_schema;
    // current charset
    std::string m_charset;
    // currently used account
    account_ref m_account;
};
}

#endif
