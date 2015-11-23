//
//  M A R I A D B + +
//
//	Author   : Sylvain Rochette Langlois
//	License  : Boost Software License (http://www.boost.org/users/license.html)
//

#ifndef _MARIADB_DATE_TIME_HPP_
#define _MARIADB_DATE_TIME_HPP_

#include <iostream>
#include <mariadb++/time.hpp>

namespace mariadb
{
	class date_time : public time
	{
	public:
		//
		// Constructor
		//
		date_time(u16 year = 1970, u8 month = 1, u8 day = 1, u8 hour = 0, u8 minute = 0, u8 second = 0, u16 millisecond = 0);
		date_time(const date_time& dt);
		date_time(const time& t);
		date_time(const tm& time_struct);
		date_time(const time_t& time);
		date_time(const MYSQL_TIME& time);
		date_time(const std::string& dt);

		//
		// Operators
		//
		int compare(const date_time& dt) const;
		date_time& operator = (const date_time& dt);
		bool operator == (const date_time& dt) const;
		bool operator != (const date_time& dt) const;
		bool operator < (const date_time& dt) const;
		bool operator <= (const date_time& dt) const;
		bool operator > (const date_time& dt) const;
		bool operator >= (const date_time& dt) const;

		//
		// Get / Set date part
		//
		u16 year() const;
		u16 year(u16 year);
		u8 month() const;
		u8 month(u8 month);
		u8 day() const;
		u8 day(u8 day);
		u16 day_of_year() const;
		u16 day_of_year(u16 day_of_year);

		//
		// Set date / time
		//
		bool set(u16 year, u8 month, u8 day);
		bool set(u16 year, u8 month, u8 day, u8 hour, u8 minute, u8 second, u16 millisecond);
		bool set(const std::string& dt);

		//
		// Date operations
		//
		date_time add_years(s32 years) const;
		date_time add_months(s32 months) const;
		date_time add_days(s32 days) const;
		date_time add_hours(s32 hours) const;
		date_time add_minutes(s32 minutes) const;
		date_time add_seconds(s32 seconds) const;
		date_time add_milliseconds(s32 milliseconds) const;
		date_time add(const time_span& dur) const;
		date_time subtract(const time_span& dur) const;
		date_time add(const time& t) const;
		date_time substract(const time& t) const;
		time_span time_between(const date_time& dt) const;
		static bool is_leap_year(u16 year);
		static bool valid_date(u16 year, u8 month, u8 day);
		static u16 days_in_year(u16 year);
		static u8  days_in_month(u16 year, u8 month);
		static u16 day_of_year(u16 year, u8 month, u8 day);

		//
		// Date conversions
		//
		static date_time reverse_day_of_year(u16 year, u16 day_of_year);
		time_t mktime() const;
		double diff_time(const date_time& dt) const;
		date_time date() const;
		MYSQL_TIME mysql_time() const;
		static date_time now();
		static date_time now_utc();
		const std::string str(bool with_millisecond = false) const;
		const std::string str_date() const;

	private:
		u16 m_year;
		u8  m_month;
		u8  m_day;
	};

	std::ostream& operator << (std::ostream& os, const date_time& ddt);
}

#endif
