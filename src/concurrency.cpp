//
//  M A R I A D B + +
//
//	Author   : Sylvain Rochette Langlois
//	License  : Boost Software License (http://www.boost.org/users/license.html)
//

#include <atomic>
#include <thread>
#include <mutex>
#include "worker.hpp"
#include "private.hpp"

using namespace mariadb;
using namespace mariadb::concurrency;

namespace
{
	handle                    g_next_handle(0);
	account_ref               g_account;
	std::mutex                g_mutex;
	std::vector<worker*>      g_querys_in;
	std::map<handle, worker*> g_querys_out;
	std::atomic<bool>         g_thread_running(false);

	//
	// Worker thread
	//
	void worker_thread()
	{
		g_thread_running = true;
		mysql_thread_init();

		while (g_querys_in.size() > 0)
		{
			worker* w = NULL;
			{
				std::lock_guard<std::mutex> lock(g_mutex);
				w = *g_querys_in.begin();
				g_querys_in.erase(g_querys_in.begin());
			}

			w->execute();

			if (!w->keep_handle())
				delete w;

			std::lock_guard<std::mutex> lock(g_mutex);
		}

		g_thread_running = false;
		mysql_thread_end();
	}

	//
	// Start thread if no thread is running
	//
	void start_thread()
	{
		if (!g_thread_running)
		{
			std::thread t(worker_thread);
			t.detach();
		}
	}

	//
	// Get worker from handle
	//
	const worker& get_worker(handle handle)
	{
		auto w = g_querys_out.find(handle);

		if (w != g_querys_out.end())
			return *w->second;

		static worker removed;
		return removed;
	}

	//
	// Add / remove a new query / command to the thread
	//
	handle add(const char* query, command::type command, bool keep_handle)
	{
		worker* w = new worker(g_account, ++g_next_handle, keep_handle, command, query);
		{
			std::lock_guard<std::mutex> lock(g_mutex);
			g_querys_in.push_back(w);

			if (keep_handle)
				g_querys_out[g_next_handle] = w;
		}

		start_thread();

		return g_next_handle;
	}

	handle add(statement_ref& statement, command::type command, bool keep_handle)
	{
		worker* w = new worker(g_account, ++g_next_handle, keep_handle, command, statement);
		{
			std::lock_guard<std::mutex> lock(g_mutex);
			g_querys_in.push_back(w);

			if (keep_handle)
				g_querys_out[g_next_handle] = w;
		}

		start_thread();

		return g_next_handle;
	}
}

//
// Set account for connection
//
void concurrency::set_account(account_ref& account)
{
	g_account = account;
}

//
// Query status
//
status::type concurrency::worker_status(handle handle)
{
	const worker& w = get_worker(handle);
	return w.status();
}

//
// Query executed, result ready to be used
//
s32 concurrency::execute(handle handle)
{
	const worker& w = get_worker(handle);
	return w.status() == status::removed ? 0 : w.result();
}

u32 concurrency::insert(handle handle)
{
	const worker& w = get_worker(handle);
	return w.status() == status::removed ? 0 : w.result();
}

result_set_ref concurrency::query(handle handle)
{
	const worker& w = get_worker(handle);
	return w.status() == status::removed ? result_set_ref() : w.result_set();
}

//
// Execute a query
//
handle concurrency::execute(const char* query, bool keep_handle)
{
	return add(query, command::execute, keep_handle);
}

handle concurrency::insert(const char* query, bool keep_handle)
{
	return add(query, command::insert, keep_handle);
}

handle concurrency::query(const char* query, bool keep_handle)
{
	return add(query, command::query, keep_handle);
}

//
// Execute a query using a statement
//
statement_ref concurrency::create_statement(const char* query)
{
	auto connection = connection::create(g_account);
	auto statement = connection->create_statement(query);
	statement->set_connection(connection);
	return statement;
}

handle concurrency::execute(statement_ref& statement, bool keep_handle)
{
	return add(statement, command::execute, keep_handle);
}

handle concurrency::insert(statement_ref& statement, bool keep_handle)
{
	return add(statement, command::insert, keep_handle);
}

handle concurrency::query(statement_ref& statement, bool keep_handle)
{
	return add(statement, command::query, keep_handle);
}

//
// Remove a query
//
// Please note, if a result_set is used, it must be done after the result_set is used...
//
void concurrency::remove(handle handle)
{
	auto w = g_querys_out.find(handle);

	if (w == g_querys_out.end())
		return;

	delete w->second;
	g_querys_out.erase(w);
}
