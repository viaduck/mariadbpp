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
// Test date time class, no data involved
//
bool test::test_date_time()
{
	date_time dt("2000-01-02 03:04:05.666");
	date_time dt2(2000, 1, 2, 3, 4, 5, 666);

	fail_if(dt != dt2, "mariadb::date_time string constructor fail!");
	fail_if(date_time::valid_date(2000, 0, 32), "mariadb::date_time::valid_date date should be invalid!");

	dt = dt.add_milliseconds(1000 * 60 * 60 * 24 * 10);
	dt2 = dt2.add_days(10);
	fail_if(dt != dt2, "mariadb::date_time::add_milliseconds fail!");
	fail_if(dt2.str(false) != "2000-01-12 03:04:05", "mariadb::date_time::str fail!");
	fail_if(dt2.str(true) != "2000-01-12 03:04:05.666", "mariadb::date_time::str fail!");

	return true;
}

//
// Test time class, no data involved
//
bool test::test_time()
{
	mariadb::time t("03:04:05.666");
	mariadb::time t2(3, 4, 5, 666);

	fail_if(t != t2, "mariadb::time string constructor fail!");

	t = t.add_milliseconds(1000 * 60 * 60 * 5);
	t2 = t2.add_hours(5);
	fail_if(t != t2, "mariadb::time::add_milliseconds fail!");
	fail_if(t2.str_time(false) != "08:04:05", "mariadb::time::str_time fail!");
	fail_if(t2.str_time(true) != "08:04:05.666", "mariadb::time::str_time fail!");

	return true;
}

bool test::test_time_span()
{
	date_time dt(2000, 1, 2, 3, 4, 5, 666);
	date_time dt2(2000, 1, 5, 5, 5, 5, 999);

	time_span ts = dt2.time_between(dt);
	fail_if(ts.days() != 3 || ts.hours() != 2 || ts.minutes() != 1 || ts.milliseconds() != 333, "mariadb::time_span::time_between fail!");

	return true;
}
