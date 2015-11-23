//
//  M A R I A D B + +
//
//	Author   : Sylvain Rochette Langlois
//	License  : Boost Software License (http://www.boost.org/users/license.html)
//

#ifndef _UNITTEST_TEST_HPP_
#define _UNITTEST_TEST_HPP_

#include <string>
#include <mariadb++/mariadb++.hpp>

namespace unittest
{
	using namespace mariadb;

	class test
	{
	public:
		//
		// Constructor
		//
		test(const char* host_name, const char* user_name, const char* password, const char* schema = NULL, u32 port = 0, const char* unix_socket = NULL);

		//
		// Destructor
		//
		virtual ~test();

		//
		// Run
		//
		int run();

	private:
		//
		// Create / Drop test database
		//
		bool create_test_db();
		void drop_test_db();

		//
		// Log fail message
		//
		#define fail_if(condition, message)\
		if (condition)\
		{\
			std::cout << message << "\nIn function: " << __FUNCTION__ << "\nIn file " << __FILE__ << "\nOn line " << __LINE__ << '\n';\
			return false;\
		}

		//
		// Execute a query
		//
		int execute(const char* query);

		//
		// Test date time class, no data involved
		//
		bool test_date_time();

		//
		// Test time class, no data involved
		//
		bool test_time();
		bool test_time_span();

		//
		// Test decimal class, no data involved
		//
		bool test_decimal();

		//
		// Test mariadb core class
		//
		bool test_transaction();
		bool test_statement();
		bool test_result_set();
		bool test_save_point();
		bool test_concurrency();

	private:
		account_ref m_account_setup;
		account_ref m_account_auto_commit;
		account_ref m_account_transaction;
	};
}

#endif
