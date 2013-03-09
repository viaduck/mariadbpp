//
//  M A R I A D B + +
//
//	Author   : Sylvain Rochette Langlois
//	License  : Boost Software License (http://www.boost.org/users/license.html)
//

#ifndef _MARIADBPP_HPP_
#define _MARIADBPP_HPP_

#include "connection.hpp"
#include "concurrency.hpp"
#include "exceptions.hpp"

namespace mariadb
{
	extern void set_log_error(bool log_error = false);
	extern void set_throw_exception(bool throw_exception = true);
}

#endif
