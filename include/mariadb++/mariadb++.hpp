//
//  M A R I A D B + +
//
//	Author   : Sylvain Rochette Langlois
//	License  : Boost Software License (http://www.boost.org/users/license.html)
//

#ifndef _MARIADBPP_HPP_
#define _MARIADBPP_HPP_

#include <mariadb++/connection.hpp>
#include <mariadb++/concurrency.hpp>
#include <mariadb++/exceptions.hpp>

namespace mariadb
{
	extern void set_log_error(bool log_error = false);

	/**
	 * MySQL client c connector library needs to be initialized and uninitialized.
	 * Initialization (mysql_library_init) is being done automatically via any calling
	 * mysql_init, but it not thread safe. This guard initializes library and
	 * deinitializes it when it is destroyed. It should be created at beginning of main loop.
	 *
	 * More info in documentation of mysql_library_init.
	 */
	class mariodb_guard {
	public:
		mariodb_guard();
		~mariodb_guard();
	};
}

#endif
