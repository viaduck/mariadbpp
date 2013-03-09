//
//  M A R I A D B + +
//
//	Author   : Sylvain Rochette Langlois
//	License  : Boost Software License (http://www.boost.org/users/license.html)
//

#include "private.hpp"

namespace mariadb
{
	bool g_throw_exception = true;

#ifdef DEBUG
	bool g_log_error = true;
#else
	bool g_log_error = false;
#endif

	void set_log_error(bool log_error)
	{
		g_log_error = log_error;
	}

	void set_throw_exception(bool throw_exception = true)
	{
		g_throw_exception = throw_exception;
	}
}
