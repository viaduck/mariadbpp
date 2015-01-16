//
//  M A R I A D B + +
//
//	Author   : Sylvain Rochette Langlois
//	License  : Boost Software License (http://www.boost.org/users/license.html)
//

#include <algorithm>
#include <mariadb++/connection.hpp>
#include <mariadb++/save_point.hpp>
#include "private.hpp"

using namespace mariadb;

namespace
{
	handle g_save_point_id = 0;

	const char* g_save_point_commands[] = {
		"SAVEPOINT %s",
		"ROLLBACK TO SAVEPOINT %s",
		"RELEASE SAVEPOINT %s"
	};
}

//
// Constructor
//
save_point::save_point(transaction* transaction) :
	m_transaction(transaction)
{
	char command[32];

	snprintf(command, sizeof(command), "SP%llu", ++g_save_point_id);
	m_name = command;
	snprintf(command, sizeof(command), g_save_point_commands[0], m_name.c_str());

	m_transaction->m_connection->execute(command);
}

//
// Destructor
//
save_point::~save_point()
{
	if (!m_transaction)
		return;

	char command[32];
	snprintf(command, sizeof(command), g_save_point_commands[1], m_name.c_str());

	m_transaction->remove_save_point(this);
	m_transaction->m_connection->execute(command);
}

//
// Commit the change
//
void save_point::commit()
{
	if (!m_transaction)
		return;

	char command[32];
	snprintf(command, sizeof(command), g_save_point_commands[2], m_name.c_str());

	m_transaction->remove_save_point(this);
	m_transaction->m_connection->execute(command);
	m_transaction = NULL;
}
