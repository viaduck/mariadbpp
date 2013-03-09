//
//  M A R I A D B + +
//
//	Author   : Sylvain Rochette Langlois
//	License  : Boost Software License (http://www.boost.org/users/license.html)
//

#ifndef _MARIADB_PRIVATE_HPP_
#define _MARIADB_PRIVATE_HPP_

#include <mariadb++/exceptions.hpp>

namespace mariadb
{
	extern bool g_throw_exception;
	extern bool g_log_error;
}

#define MARIADB_ERROR(error_id, error)\
	if (mariadb::g_log_error)\
		std::cerr << "MariaDB Error(" <<  error_id << "): " << error << "\nIn function: " << __PRETTY_FUNCTION__ << "\nIn file " << __FILE__ << "\nOn line " << __LINE__ << '\n';\
	if (mariadb::g_throw_exception)\
		throw exception::connection(error_id, error);

#define MYSQL_ERROR_NO_BRAKET(mysql)\
	m_last_error_no = mysql_errno(mysql);\
	m_last_error = mysql_error(mysql);\
	MARIADB_ERROR(m_last_error_no, m_last_error);

#define MYSQL_ERROR(mysql)\
{\
	MYSQL_ERROR_NO_BRAKET(mysql)\
}

#define MYSQL_ERROR_RETURN_FALSE(mysql)\
{\
	MYSQL_ERROR_NO_BRAKET(mysql)\
	return false;\
}

#define STMT_ERROR_NO_BRAKET(statement)\
	m_last_error_no = mysql_stmt_errno(statement);\
	m_last_error = mysql_stmt_error(statement);\
	MARIADB_ERROR(m_last_error_no, m_last_error);

#define STMT_ERROR(statement)\
{\
	STMT_ERROR_NO_BRAKET(statement)\
}

#define STMT_ERROR_RETURN_FALSE(statement)\
{\
	STMT_ERROR_NO_BRAKET(statement)\
	return false;\
}

#endif
