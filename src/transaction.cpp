//
//  M A R I A D B + +
//
//	Author   : Sylvain Rochette Langlois
//	License  : Boost Software License (http://www.boost.org/users/license.html)
//

#include <mysql/mysql.h>
#include <mariadb++/connection.hpp>
#include <mariadb++/transaction.hpp>

using namespace mariadb;

namespace
{
	const char* g_isolation_level[] = {
		"SET TRANSACTION ISOLATION LEVEL REPEATABLE READ;",
		"SET TRANSACTION ISOLATION LEVEL READ COMMITTED;",
		"SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED;",
		"SET TRANSACTION ISOLATION LEVEL SERIALIZABLE;",
	};

	const char* g_consistent_snapshot[] = {
		"START TRANSACTION;",
		"START TRANSACTION WITH CONSISTENT SNAPSHOT;",
	};
}

//
// Constructor
//
transaction::transaction(connection* conn, isolation::level level, bool consistent_snapshot) :
	m_connection(conn)
{
	conn->execute(g_isolation_level[level]);
	conn->execute(g_consistent_snapshot[consistent_snapshot]);
}

//
// Destructor
//
transaction::~transaction()
{
	if (!m_connection)
		return;

	mysql_rollback(m_connection->m_mysql);
	cleanup();
}


//
// Cleanup transaction
//
void transaction::cleanup()
{
	for(save_point* save_point : m_save_points)
	{
		save_point->m_transaction = NULL;
	}
}

//
// Commit the change
//
void transaction::commit()
{
	if (!m_connection)
		return;

	mysql_commit(m_connection->m_mysql);
	cleanup();
	m_connection = NULL;
}

//
// Create save point
//
save_point_ref transaction::create_save_point()
{
	if (!m_connection)
		return save_point_ref();

	save_point* sp = new save_point(this);
	m_save_points.push_back(sp);
	return save_point_ref(sp);
}

//
// Remove save_point
//
void transaction::remove_save_point(save_point* sv_point)
{
	m_save_points.erase(std::remove(m_save_points.begin(), m_save_points.end(), sv_point));
}
