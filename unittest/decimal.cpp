//
//  M A R I A D B + +
//
//	Author   : Sylvain Rochette Langlois
//	License  : Boost Software License (http://www.boost.org/users/license.html)
//

#include "test.hpp"

using namespace mariadb;
using namespace unittest;

//
// Test decimal class, no data involved
//
bool test::test_decimal()
{
	decimal d("24.1234");
	decimal d2(24.1234f, 4);

	fail_if(d != d2, "mariadb::decimal string constructor fail!");

	d -= d2;
	fail_if(d.unbiased(), "mariadb::decimal operator -= fail!");

	d += d2;
	fail_if(d != d2, "mariadb::decimal operator += fail!");
	fail_if(d.str() != "24.1234", "mariadb::decimal operator += fail!");

	return true;
}
