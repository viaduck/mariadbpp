//
//  M A R I A D B + +
//
//	Author   : Sylvain Rochette Langlois
//	License  : Boost Software License (http://www.boost.org/users/license.html)
//

#ifndef _MARIADB_CONNECTION_HPP_
#define _MARIADB_CONNECTION_HPP_

#include <string>
#include "account.hpp"
#include "statement.hpp"
#include "transaction.hpp"
#include "save_point.hpp"
#include "result_set.hpp"

//
// C++ wrapper over mariadb-native-client API
//

namespace mariadb
{
	class connection : public last_error
	{
		friend class result_set;
		friend class statement;
		friend class transaction;
		friend class save_point;

	public:
		//
		// Destructor
		//
		virtual ~connection();

		//
		// Connect to the database
		//
		bool connect();

		//
		// Disconnect from the database
		//
		void disconnect();

		//
		// Tell if a connection is currently active to the database
		//
		bool connected() const;

		//
		// Get account
		//
		account_ref account() const;

		//
		// Get / Set schema (database)
		//
		const char* schema() const;
		bool set_schema(const char* schema);

		//
		// Execute a query with or without statement
		//
		s32 execute(const char* query);
		u32 insert(const char* query);
		result_set_ref query(const char* query);

		//
		// Set auto commit mode
		//
		bool auto_commit() const;
		bool set_auto_commit(bool auto_commit);

	public:
		//
		// Create statement
		//
		statement_ref create_statement(const char* query);

		//
		// Commit / rollback support
		//
		transaction_ref create_transaction(isolation::level level = isolation::repeatable_read, bool consistent_snapshot = true);

	public:
		//
		// Create a new connection
		//
		static connection_ref create(account_ref& account);

	private:
		//
		// Constructor
		//
		connection(account_ref& account);

	private:
		bool        m_auto_commit;
		MYSQL*      m_mysql;
		std::string m_schema;
		account_ref m_account;
	};
}

#endif
