//
//  M A R I A D B + +
//
//	Author   : Sylvain Rochette Langlois
//	License  : Boost Software License (http://www.boost.org/users/license.html)
//

#ifndef _MARIADB_CONCURRENCY_HPP_
#define _MARIADB_CONCURRENCY_HPP_

#include <vector>
#include <mariadb++/statement.hpp>

namespace mariadb
{
	namespace concurrency
	{
		namespace status
		{
			enum type
			{
				waiting,
				executing,
				succeed,
				failed,
				removed
			};
		}

		//
		// Set account for connection
		//
		extern void set_account(account_ref& account);

		//
		// Query status
		//
		extern status::type worker_status(handle handle);

		//
		// Execute a query
		//
		extern handle execute(const char* query, bool keep_handle = false);
		extern handle insert(const char* query, bool keep_handle = false);
		extern handle query(const char* query, bool keep_handle = true);

		//
		// Execute a query using a statement
		//
		extern statement_ref create_statement(const char* query);
		extern handle execute(statement_ref& statement, bool keep_handle = false);
		extern handle insert(statement_ref& statement, bool keep_handle = false);
		extern handle query(statement_ref& statement, bool keep_handle = true);

		//
		// Query executed, result ready to be used
		//
		extern u64 execute(handle handle);
		extern u64 insert(handle handle);
		extern result_set_ref query(handle handle);

		//
		// Remove a query
		//
		// Please note, if a result_set is used, it must be done after the result_set is used...
		//
		extern void remove(handle handle);
	}
}

#endif
