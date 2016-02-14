//
//  M A R I A D B + +
//
//	Author   : Sylvain Rochette Langlois
//	License  : Boost Software License (http://www.boost.org/users/license.html)
//

#include <thread>
#include <set>
#include "test.hpp"

using namespace mariadb;
using namespace unittest;

//
// Test mariadb core class
//
bool test::test_transaction()
{
	connection_ref con = connection::create(m_account_transaction);

	//
	// Test commit
	//
	{
		transaction_ref trx = con->create_transaction();
		u64 id = con->insert("INSERT INTO test(str) VALUES('test');");
		trx->commit();
		fail_if(!id, "mariadb::connection::insert id is zero!");

		result_set_ref rs = con->query("SELECT COUNT(*) FROM test;");
		fail_if(!rs || !rs->next() || !rs->get_unsigned64((u32)0), "mariadb::connection::transaction could not commit!");
	}

	//
	// Test rollback
	//
	{
		transaction_ref trx = con->create_transaction();
		con->insert("INSERT INTO test(str) VALUES('test2');");
	}

	result_set_ref rs = con->query("SELECT COUNT(*) FROM test;");
	fail_if(!rs || !rs->next() || 1 != rs->get_unsigned64((u32)0), "mariadb::connection::transaction could not rollback!");

	execute("DELETE FROM test WHERE id < 10000;");
	return true;
}

bool test::test_save_point()
{
	connection_ref con = connection::create(m_account_transaction);

	//
	// Test commit
	//
	{
		transaction_ref trx = con->create_transaction();
		save_point_ref sp = trx->create_save_point();
		u64 id = con->insert("INSERT INTO test(str) VALUES('test');");
		sp->commit();
		trx->commit();
		fail_if(!id, "mariadb::connection::insert id is zero!");

		result_set_ref rs = con->query("SELECT COUNT(*) FROM test;");
		fail_if(!rs || !rs->next() || !rs->get_unsigned64((u32)0), "mariadb::connection::save_point could not commit!");
	}
	{
		transaction_ref trx = con->create_transaction();
		save_point_ref sp = trx->create_save_point();
		u64 id = con->insert("INSERT INTO test(str) VALUES('test');");
		trx->commit();
		fail_if(!id, "mariadb::connection::insert id is zero!");

		result_set_ref rs = con->query("SELECT COUNT(*) FROM test;");
		fail_if(!rs || !rs->next() || 1 == rs->get_unsigned64((u32)0), "mariadb::connection::save_point could not commit!");
	}

	//
	// Test rollback
	//
	{
		transaction_ref trx = con->create_transaction();
		{
			save_point_ref sp = trx->create_save_point();
			con->insert("INSERT INTO test(str) VALUES('test2');");
		}
		trx->commit();
	}

	result_set_ref rs = con->query("SELECT COUNT(*) FROM test;");
	fail_if(!rs || !rs->next() || 2 != rs->get_unsigned64((u32)0), "mariadb::connection::save_point could not rollback!");

	execute("DELETE FROM test WHERE id < 10000;");
	return true;
}

bool test::test_statement()
{
	const char* queries[] = {
		"INSERT INTO test (data) VALUES(?);",
		"INSERT INTO test (data, str) VALUES(?, ?);",
		"INSERT INTO test (data, str, dt) VALUES(?, ?, ?);",
		"INSERT INTO test (data, str, dt, t) VALUES(?, ?, ?, ?);",
		"INSERT INTO test (data, str, dt, t, value) VALUES(?, ?, ?, ?, ?);",
		"INSERT INTO test (data, str, dt, t, value, deci) VALUES(?, ?, ?, ?, ?, ?);",
		NULL
	};

	u32 index = 0;
	u32 id = 0;
	connection_ref con = connection::create(m_account_auto_commit);

	while (queries[index])
	{
		statement_ref sta = con->create_statement(queries[index]);
		data_ref data = data_ref(new mariadb::data<char>(100));

		sta->set_data(0, data);

		if (index >= 1) sta->set_string(1, "test");
		if (index >= 2) sta->set_date_time(2, date_time(2000, 1, 2, 3, 4, 5));
		if (index >= 3) sta->set_time(3, mariadb::time(11, 22, 33));
		if (index >= 4) sta->set_signed32(4, 666);
		if (index >= 5) sta->set_decimal(5, "1.1234");
		id = sta->insert();

		index++;
		result_set_ref rs = con->query("SELECT COUNT(*) FROM test;");
		fail_if(!rs || !rs->next() || index != rs->get_unsigned64((u32)0), "mariadb::connection::statement unable to insert a row at index " << (index - 1));
	}

	statement_ref sta = con->create_statement("DELETE FROM test WHERE id < ?;");
	sta->set_unsigned32(0, id);
	sta->execute();

	return true;
}

bool test::test_result_set()
{
	//
	// Validate the inserted value from last test
	//
	connection_ref con = connection::create(m_account_auto_commit);
	result_set_ref rs = con->query("SELECT data, str, dt, t, value, deci FROM test LIMIT 1;");

	fail_if(!rs || !rs->next(), "mariadb::connection::result_set no row found!");

	data_ref data = rs->get_data("data");
	fail_if(data->size() != 100, "mariadb::connection::result_set invalid blob!");

	std::string str = rs->get_string("str");
	fail_if(str != "test", "mariadb::connection::result_set invalid string!");

	s32 value = rs->get_signed32("value");
	fail_if(value != 666, "mariadb::connection::result_set invalid signed32!");

	decimal dec = rs->get_decimal("deci");
	fail_if(dec.str() != "1.1234", "mariadb::connection::result_set invalid decimal!");

	date_time dt = rs->get_date_time("dt");
	fail_if(dt.str() != "2000-01-02 03:04:05", "mariadb::connection::result_set invalid date_time!");

	mariadb::time t = rs->get_time("t");
	fail_if(t.str_time() != "11:22:33", "mariadb::connection::result_set invalid time!");

	execute("DELETE FROM test WHERE id < 10000;");
	return true;
}

bool test::test_concurrency()
{
	bool success;
	concurrency::set_account(m_account_auto_commit);

	auto haendl = concurrency::execute("DROP PROCEDURE IF EXISTS insert_rows;", true);
	success = concurrency::wait_handle(haendl);
	concurrency::release_handle(haendl);

	fail_if(!success, "mariadb::concurrency did not work!");

	haendl = concurrency::execute("\
CREATE PROCEDURE insert_rows()\
BEGIN\
	DECLARE counter INT DEFAULT 100;\
	test_loop: LOOP\
		IF counter = 0 THEN\
			LEAVE test_loop;\
		END IF;\
		INSERT INTO test(str) VALUES('test');\
		SET counter = counter - 1;\
	END LOOP;\
END", true);

	success = concurrency::wait_handle(haendl, 200);
	concurrency::release_handle(haendl);

	fail_if(!success, "mariadb::concurrency did not work!");

	haendl = concurrency::execute("call insert_rows(); DELETE FROM test;", true);
	success = concurrency::wait_handle(haendl);
	concurrency::release_handle(haendl);

	fail_if(!success, "mariadb::concurrency did not work!");

	return true;
}

bool test::test_real_concurrency() {
	constexpr int num_results = 100;

	std::vector<handle> handles;
	std::set<u64> results;

	concurrency::set_account(m_account_auto_commit);

	// launch all queries
	for(int i = 0; i < num_results; i++) {
		auto heandl = concurrency::insert("INSERT INTO test(str) VALUES('teest');", true);
		handles.push_back(heandl);
	}

	// wait for all queries
	for(auto h : handles)
		fail_if(!concurrency::wait_handle(h), "mariadb::concurrency did not work");

	// get all results
	for(auto h : handles) {
		u64 res = concurrency::get_execute_result(h);
		auto set_result = results.insert(res);

		// fail if this result already existed (insert returns false as second)
		fail_if(!set_result.second, "maraidb::concurrency did not work");
	}

	// release all handles
	for(auto h : handles)
		concurrency::release_handle(h);

	// clear table for tests later on
	auto hn = concurrency::execute("DELETE FROM test;", true);
	fail_if(!concurrency::wait_handle(hn), "mariadb::concurrency did not work");
	concurrency::release_handle(hn);

	fail_if(results.size() != num_results, "mariadb::concurrency did not work, results missing");

	return true;
}