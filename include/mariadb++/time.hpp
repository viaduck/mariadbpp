//
//  M A R I A D B + +
//
//	Author   : Sylvain Rochette Langlois
//	License  : Boost Software License (http://www.boost.org/users/license.html)
//

#ifndef _MARIADB_TIME_HPP_
#define _MARIADB_TIME_HPP_

#include <iostream>
#include <mariadb++/time_span.hpp>

struct st_mysql_time;
typedef struct st_mysql_time MYSQL_TIME;

namespace mariadb
{
	class time
	{
	public:
		//
		// Constructor
		//
		time(u8 hour = 0, u8 minute = 0, u8 second = 0, u16 millisecond = 0);
		time(const time& t);
		time(const tm& time_struct);
		time(const time_t& time);
		time(const MYSQL_TIME& time);
		time(const std::string& t);

		//
		// Operators
		//
		int compare(const time& t) const;
		time& operator = (const time& t);
		bool operator == (const time& t) const;
		bool operator != (const time& t) const;
		bool operator < (const time& t) const;
		bool operator <= (const time& t) const;
		bool operator > (const time& t) const;
		bool operator >= (const time& t) const;

		//
		// Get / Set time part
		//
		u8 hour() const;
		u8 hour(u8 hour);
		u8 minute() const;
		u8 minute(u8 minute);
		u8 second() const;
		u8 second(u8 second);
		u16 millisecond() const;
		u16 millisecond(u16 millisecond);

		//
		// Set date / time
		//
		bool set(const std::string& t);
		bool set(u8 hour, u8 minute, u8 second, u16 millisecond);

		//
		// Date operations
		//
		time add_hours(s32 hours) const;
		time add_minutes(s32 minutes) const;
		time add_seconds(s32 seconds) const;
		time add_milliseconds(s32 milliseconds) const;

		time subtract(const time_span& dur) const;
		time add(const time_span& dur) const;
		time_span time_between(const time& t) const;

		//
		// Time conversions
		//
		time_t mktime() const;
		double diff_time(const time& t) const;
		MYSQL_TIME mysql_time() const;
		static time now();
		static time now_utc();
		const std::string str_time(bool with_millisecond = false) const;

	protected:
		u8  m_hour;
		u8  m_minute;
		u8  m_second;
		u16 m_millisecond;
	};

	std::ostream& operator << (std::ostream& os, const time& t);
}

#endif
