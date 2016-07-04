//
//  M A R I A D B + +
//
//	Author   : Sylvain Rochette Langlois
//	License  : Boost Software License (http://www.boost.org/users/license.html)
//

#ifndef _MARIADB_PRIVATE_HPP_
#define _MARIADB_PRIVATE_HPP_

#include <mariadb++/exceptions.hpp>
#include <time.h>

namespace mariadb
{
	extern bool g_log_error;

#if _WIN32
	inline int localtime_safe(struct tm * _tm, const time_t * _time)
	{
		return localtime_s(_tm, _time);
	}

	inline int gmtime_safe(struct tm * _tm, const time_t * _time)
	{
		return gmtime_s(_tm, _time);
	}
#else
	inline int localtime_safe(struct tm * _tm, const time_t * _time)
	{
		return localtime_r(_time, _tm) ? 0 : -1;
	}

	inline int gmtime_safe(struct tm * _tm, const time_t * _time)
	{
		return gmtime_r(_time, _tm) ? 0 : -1;
	}
#endif
}
#if _WIN32

#if __MINGW32__
// this is needed because on mingw sprintf_s is broken (due to msvcrt not following c99 standard with sprintf...)
#define snprintf(buffer, szbuffer, format, ...) __mingw_sprintf(buffer, format, ##__VA_ARGS__)
#else
#define snprintf sprintf_s
#endif

#endif

#ifdef MARIADB_USES_EXCEPTION
#define MARIADB_ERROR_THROW_CONNECTION(error_id, error) throw exception::connection(error_id, error);
#define MARIADB_ERROR_THROW_DATE(_year, _month, _day, _hour, _minute, _second, _millisecond) throw exception::date_time(_year, _month, _day, _hour, _minute, _second, _millisecond);
#define MARIADB_ERROR_THROW_TIME(_hour, _minute, _second, _millisecond) throw exception::time(_hour, _minute, _second, _millisecond);
#else
#warning MariaDB++ without exceptions is not implemented yet

#define MARIADB_ERROR_THROW_CONNECTION(error_id, error)
#define MARIADB_ERROR_THROW_DATE(_year, _month, _day, _hour, _minute, _second, _millisecond)
#define MARIADB_ERROR_THROW_TIME(_hour, _minute, _second, _millisecond)
#endif


#define MARIADB_ERROR(error_id, error)\
	if (mariadb::g_log_error)\
		std::cerr << "MariaDB Error(" <<  error_id << "): " << error << "\nIn function: " << __FUNCTION__ << "\nIn file " << __FILE__ << "\nOn line " << __LINE__ << '\n';\
	MARIADB_ERROR_THROW_CONNECTION(error_id, error)

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
