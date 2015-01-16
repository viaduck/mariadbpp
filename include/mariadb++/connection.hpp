//
//  M A R I A D B + +
//
//	Author   : Sylvain Rochette Langlois
//	License  : Boost Software License (http://www.boost.org/users/license.html)
//

#ifndef _MARIADB_CONNECTION_HPP_
#define _MARIADB_CONNECTION_HPP_

#include <string>
#include <mariadb++/account.hpp>
#include <mariadb++/statement.hpp>
#include <mariadb++/transaction.hpp>
#include <mariadb++/save_point.hpp>
#include <mariadb++/result_set.hpp>

struct st_mysql;
typedef struct st_mysql MYSQL;

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
		// Get / Set character set
		//
		const std::string& charset() const;
		bool set_charset(const std::string& value);

		//
		// Execute a query with or without statement
		//
		u64 execute(const std::string& query);
		u64 insert(const std::string& query);
		result_set_ref query(const std::string& query);

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
		std::string m_charset;
		account_ref m_account;
	};
}

#endif
