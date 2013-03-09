//
//  M A R I A D B + +
//
//	Author   : Sylvain Rochette Langlois
//	License  : Boost Software License (http://www.boost.org/users/license.html)
//

#include <mariadb++/last_error.hpp>

using namespace mariadb;

//
// Constructor
//
last_error::last_error() :
	m_last_error_no(0)
{
}

//
// Get last error
//
u32 last_error::error_no() const
{
	return m_last_error_no;
}

const std::string& last_error::error() const
{
	return m_last_error;
}
