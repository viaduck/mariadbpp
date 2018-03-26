//
//  M A R I A D B + +
//
//          Copyright Sylvain Rochette Langlois 2013,
//                    The ViaDuck Project 2016 - 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef _MARIADB_TRANSACTION_HPP_
#define _MARIADB_TRANSACTION_HPP_

#include <vector>
#include <algorithm>
#include <mariadb++/save_point.hpp>
#include <mariadb++/types.hpp>

namespace mariadb {
class connection;
class save_point;

/**
 * Class representing a SQL transaction having automatic rollback functionality
 */
class transaction {
    friend class connection;
    friend class save_point;

   public:
    /**
     * Destructor initiates automatic rollback if changes were not committed
     */
    virtual ~transaction();

    /**
     * Commits the changes, releases all savepoints
     */
    void commit();

    /**
     * Create named savepoint
     * Note: only valid until the transaction is destroyed or committed
     *
     * @return Reference to a unique new savepoint
     */
    save_point_ref create_save_point();

   private:
    /**
     * Create a transaction with given isolation level and snapshot setting
     *
     * @param conn                  Connection to start transaction on
     * @param level                 Level of database isolation to use
     * @param consistent_snapshot   Controls whether the transaction needs a consistent snapshot on
     * creation
     */
    transaction(connection* conn, isolation::level level, bool consistent_snapshot);

    /**
     * Removes a savepoint from the list of savepoints
     *
     * @param sv_point savepoint to remove
     */
    void remove_save_point(save_point* sv_point);

    /**
     * Cleans up the transaction, releases all savepoints
     */
    void cleanup();

   private:
    // parent connection pointer
    connection* m_connection;
    // list of created savepoints for this transaction
    std::vector<save_point*> m_save_points;
};

typedef std::shared_ptr<transaction> transaction_ref;
}

#endif
