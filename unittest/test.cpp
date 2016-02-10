//
//  M A R I A D B + +
//
//	Author   : Sylvain Rochette Langlois
//	License  : Boost Software License (http://www.boost.org/users/license.html)
//

#include "test.hpp"

using namespace mariadb;
using namespace unittest;

//
// Constructor
//
test::test(const char* host_name, const char* user_name, const char* password, const char* schema, u32 port, const char* unix_socket)
{
	m_account_setup = account::create(host_name, user_name, password, NULL, port, unix_socket);
	m_account_setup->set_auto_commit(true);

	m_account_auto_commit = account::create(host_name, user_name, password, schema, port, unix_socket);
	m_account_auto_commit->set_auto_commit(true);

	m_account_transaction = account::create(host_name, user_name, password, schema, port, unix_socket);
	m_account_transaction->set_auto_commit(false);
}

//
// Destructor
//
test::~test()
{
}

//
// Run
//
int test::run()
{
	if (!create_test_db())
	{
		std::cout << "Unable to create test table in schema " << m_account_auto_commit->schema() << "!\n";
		return 0;
	}

	try
	{
		if (test_date_time() &&
			test_time() &&
			test_time_span() &&
			test_decimal() && // TODO: fix this SH$"!/!IT
			test_transaction() &&
			test_save_point() &&
			test_statement() &&
			test_result_set() &&
			test_concurrency())
		{
			std::cout << "test succeed!\n";
			drop_test_db();
		}
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << '\n';
	}

	return 0;
}

bool test::create_test_db()
{
	try
	{
		connection_ref con = connection::create(m_account_setup);
		statement_ref sta = con->create_statement("SELECT 1 FROM INFORMATION_SCHEMA.SCHEMATA WHERE SCHEMA_NAME = ?;");
		sta->set_string(0, m_account_auto_commit->schema().c_str());
		result_set_ref rs = sta->query();

		if (!rs ||
			!rs->next())
			con->execute(("CREATE DATABASE " + m_account_auto_commit->schema()).c_str());
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << '\n';
		return false;
	}

	return execute("DROP TABLE IF EXISTS test; CREATE TABLE test(id INT PRIMARY KEY AUTO_INCREMENT, data TINYBLOB NULL, str VARCHAR(50) NULL, dt DATETIME NULL, t TIME NULL, value INT NULL, deci DECIMAL(8,4) DEFAULT 0.0);");
}

void test::drop_test_db()
{
	execute("DROP TABLE IF EXISTS test;");
}

//
// Execute a query
//
int test::execute(const char* query)
{
	auto con = connection::create(m_account_auto_commit);

	try
	{
		return con->execute(query) || 1;
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << '\n';
		return 0;
	}

	return 1;
}
