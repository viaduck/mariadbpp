//
//  M A R I A D B + +
//
//	Author   : Sylvain Rochette Langlois
//	License  : Boost Software License (http://www.boost.org/users/license.html)
//

#include <thread>
#include "test.hpp"

using namespace mariadb;
using namespace unittest;

//
// Test mariadb core class
//
bool test::test_transaction()
{
	auto con = connection::create(m_account_transaction);

	//
	// Test commit
	//
	{
		auto trx = con->create_transaction();
		auto id = con->insert("INSERT INTO test(str) VALUES('test');");
		trx->commit();
		fail_if(!id, "mariadb::connection::insert id is zero!");

		auto rs = con->query("SELECT COUNT(*) FROM test;");
		fail_if(!rs || !rs->next() || !rs->get_unsigned64((u32)0), "mariadb::connection::transaction cound not commit!");
	}

	//
	// Test rollback
	//
	{
		auto trx = con->create_transaction();
		con->insert("INSERT INTO test(str) VALUES('test2');");
	}

	auto rs = con->query("SELECT COUNT(*) FROM test;");
	fail_if(!rs || !rs->next() || 1 != rs->get_unsigned64((u32)0), "mariadb::connection::transaction cound not rollback!");

	execute("DELETE FROM test WHERE id < 10000;");
	return true;
}

bool test::test_save_point()
{
	auto con = connection::create(m_account_transaction);

	//
	// Test commit
	//
	{
		auto trx = con->create_transaction();
		auto sp = trx->create_save_point();
		auto id = con->insert("INSERT INTO test(str) VALUES('test');");
		sp->commit();
		trx->commit();
		fail_if(!id, "mariadb::connection::insert id is zero!");

		auto rs = con->query("SELECT COUNT(*) FROM test;");
		fail_if(!rs || !rs->next() || !rs->get_unsigned64((u32)0), "mariadb::connection::save_point cound not commit!");
	}
	{
		auto trx = con->create_transaction();
		auto sp = trx->create_save_point();
		auto id = con->insert("INSERT INTO test(str) VALUES('test');");
		trx->commit();
		fail_if(!id, "mariadb::connection::insert id is zero!");

		auto rs = con->query("SELECT COUNT(*) FROM test;");
		fail_if(!rs || !rs->next() || 1 == rs->get_unsigned64((u32)0), "mariadb::connection::save_point cound not commit!");
	}

	//
	// Test rollback
	//
	{
		auto trx = con->create_transaction();
		{
			auto sp = trx->create_save_point();
			con->insert("INSERT INTO test(str) VALUES('test2');");
		}
		trx->commit();
	}

	auto rs = con->query("SELECT COUNT(*) FROM test;");
	fail_if(!rs || !rs->next() || 2 != rs->get_unsigned64((u32)0), "mariadb::connection::save_point cound not rollback!");

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
	auto con = connection::create(m_account_auto_commit);

	while (queries[index])
	{
		auto sta = con->create_statement(queries[index]);
		auto data = data_ref(new mariadb::data<char>(100));

		sta->set_data(0, data);

		if (index >= 1) sta->set_string(1, "test");
		if (index >= 2) sta->set_date_time(2, date_time(2000, 1, 2, 3, 4, 5));
		if (index >= 3) sta->set_time(3, mariadb::time(11, 22, 33));
		if (index >= 4) sta->set_signed32(4, 666);
		if (index >= 5) sta->set_decimal(5, "1.1234");
		id = sta->insert();

		index++;
		auto rs = con->query("SELECT COUNT(*) FROM test;");
		fail_if(!rs || !rs->next() || index != rs->get_unsigned64((u32)0), "mariadb::connection::statement unable to insert a row at index " << (index - 1));
	}

	auto sta = con->create_statement("DELETE FROM test WHERE id < ?;");
	sta->set_unsigned32(0, id);
	sta->execute();

	return true;
}

bool test::test_result_set()
{
	//
	// Validate the inserted value from last test
	//
	auto con = connection::create(m_account_auto_commit);
	auto rs = con->query("SELECT data, str, dt, t, value, deci FROM test LIMIT 1;");

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
	concurrency::status::type status;
	concurrency::set_account(m_account_auto_commit);
	concurrency::execute("DROP PROCEDURE IF EXISTS insert_rows;");
	auto handle = concurrency::execute("\
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

	while ((status = concurrency::worker_status(handle)) < concurrency::status::succeed)
	{
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}

	concurrency::remove(handle);
	fail_if(status != concurrency::status::succeed, "mariadb::concurrency did not work!");

	handle = concurrency::execute("call insert_rows(); DELETE FROM test;", true);

	while ((status = concurrency::worker_status(handle)) < concurrency::status::succeed)
	{
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}

	concurrency::remove(handle);
	fail_if(status != concurrency::status::succeed, "mariadb::concurrency did not work!");

	return true;
}
