//
//  M A R I A D B + +
//
//	Author   : Sylvain Rochette Langlois
//	License  : Boost Software License (http://www.boost.org/users/license.html)
//

#include <mysql.h>
#include <sstream>
#include <boost/lexical_cast.hpp>
#include <mariadb++/exceptions.hpp>
#include <mariadb++/date_time.hpp>
#include "private.hpp"

using namespace mariadb;
using namespace std;

namespace
{
	const u8 g_month_lengths[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
}

#define MS_PER_SEC 1000
#define MS_PER_MIN (MS_PER_SEC * 60)
#define MS_PER_HOUR (MS_PER_MIN * 60)
#define MS_PER_DAY (MS_PER_HOUR * 24)

#define THROW_EXCEPTION(_year, _month, _day, _hour, _minute, _second, _millisecond)\
{\
	if (g_log_error)\
		std::cerr << "MariaDB Invalid date time: year - " << _year << ", month - " << _month << ", day - " << _day\
			<< ", hour - " << _hour << ", minute - " << _minute << ", second - " << _second << ", millisecond - " << _millisecond\
			<< "\nIn function: " << __FUNCTION__ << '\n';\
	MARIADB_ERROR_THROW_DATE(_year, _month, _day, _hour, _minute, _second, _millisecond)\
	return false;\
}

//
// Constructors
//
date_time::date_time(u16 year, u8 month, u8 day, u8 hour, u8 minute, u8 second, u16 millisecond)
{
	set(year, month, day, hour, minute, second, millisecond);
}

date_time::date_time(const date_time& dt)
{
	set(dt.year(), dt.month(), dt.day(), dt.hour(), dt.minute(), dt.second(), dt.millisecond());
}

date_time::date_time(const time& t)
{
	set(1900, 1, 1, t.hour(), t.minute(), t.second(), t.millisecond());
}

date_time::date_time(const tm& t)
{
	set(t.tm_year + 1900, t.tm_mon + 1, t.tm_mday, t.tm_hour, t.tm_min, t.tm_sec, 0);
}

date_time::date_time(const time_t& time)
{
	tm t;
	localtime_safe(&t, &time);

	set(t.tm_year + 1900, t.tm_mon + 1, t.tm_mday, t.tm_hour, t.tm_min, t.tm_sec, 0);
}

date_time::date_time(const MYSQL_TIME& t)
{
	set(t.year, t.month, t.day, t.hour, t.minute, t.second, 0);
}

date_time::date_time(const std::string& dt)
{
	set(dt);
}

//
// Operators
//
int date_time::compare(const date_time& dt) const
{
	if (year() < dt.year())
		return -1;

	if (year() > dt.year())
		return 1;

	if (month() < dt.month())
		return -1;

	if (month() > dt.month())
		return 1;

	if (day() < dt.day())
		return -1;

	if (day() > dt.day())
		return 1;

	return time::compare(dt);
}

date_time& date_time::operator = (const date_time& dt)
{
	set(dt.year(), dt.month(), dt.day(), dt.hour(), dt.minute(), dt.second(), dt.millisecond());
	return *this;
}

bool date_time::operator == (const date_time& dt) const
{
	return compare(dt) == 0;
}

bool date_time::operator != (const date_time& dt) const
{
	return compare(dt) != 0;
}

bool date_time::operator < (const date_time& dt) const
{
	return compare(dt) < 0;
}

bool date_time::operator <= (const date_time& dt) const
{
	return compare(dt) <= 0;
}

bool date_time::operator > (const date_time& dt) const
{
	return compare(dt) > 0;
}

bool date_time::operator >= (const date_time& dt) const
{
	return compare(dt) >= 0;
}

bool date_time::set(u16 year, u8 month, u8 day)
{
	if (!valid_date(year, month, day))
		THROW_EXCEPTION(year, month, day, 0, 0, 0, 0);

	m_year = year;
	m_month = month;
	m_day = day;
	return true;
}

bool date_time::set(u16 year, u8 month, u8 day, u8 hour, u8 minute, u8 second, u16 millisecond)
{
	if (!valid_date(year, month, day) || hour >= 24 || minute >= 60 || second >= 60 || millisecond >= 1000)
		THROW_EXCEPTION(year, month, day, hour, minute, second, millisecond);

	m_year = year;
	m_month = month;
	m_day = day;

	return time::set(hour, minute, second, millisecond);
}

u16 date_time::year() const
{
	return m_year;
}

u16 date_time::year(u16 year)
{
	if (year == 0)
		THROW_EXCEPTION(year, month(), day(), hour(), minute(), second(), millisecond());

	m_year = year;

	if (!valid_date(year, month(), day()))
	{
		m_month = 1;
		m_day = 1;
	}

	return m_year;
}

u8 date_time::month() const
{
	return m_month;
}

u8 date_time::month(u8 month)
{
	if (month == 0 || month > 12)
		THROW_EXCEPTION(year(), month, day(), hour(), minute(), second(), millisecond());

	m_month = month;

	u8 days_inmonth = days_in_month(year(), month);

	if (day() > days_inmonth)
		m_day = 1;

	return m_month;
}

u8 date_time::day() const
{
	return m_day;
}

u8 date_time::day(u8 day)
{
	u8 days_inmonth = days_in_month(year(), month());

	if (day == 0 || day > days_inmonth)
		THROW_EXCEPTION(year(), month(), day, hour(), minute(), second(), millisecond());

	m_day = day;

	return m_day;
}

u16 date_time::day_of_year() const
{
	return day_of_year(year(), month(), day());
}

u16 date_time::day_of_year(u16 day_of_year)
{
	date_time new_date = reverse_day_of_year(year(), day_of_year);

	month(new_date.month());
	day(new_date.day());

	return day_of_year;
}

date_time date_time::add_years(s32 years) const
{
	date_time tmp = *this;

	if (!years)
		return tmp;

	tmp.year(m_year + years);
	return tmp;
}

date_time date_time::add_months(s32 months) const
{
	date_time tmp = *this;

	if (!months)
		return tmp;

	s32 years = months / 12;

	months = (months % 12) + month();

	if (months >= 12)
	{
		++years;
		months %= 12;
	}
	else if (months < 0)
	{
		--years;
		months = 12 - months;
	}

	if (years)
		tmp = tmp.add_years(years);

	tmp.month(months);
	return tmp;
}

date_time date_time::add_days(s32 days) const
{
	date_time tmp = *this;

	if (!days)
		return tmp;

	s32 dir = days > 0 ? 1 : -1;
	s32 y = year();
	s32 days_in = days_in_year(y);
	s32 day_of = day_of_year() + days;

	while (day_of > days_in || day_of < 0)
	{
		day_of -= days_in * dir;
		y += dir;
		days_in = days_in_year(y);
	}

	tmp.year(y);
	tmp.day_of_year(day_of);
	return tmp;
}

date_time date_time::add_hours(s32 hours) const
{
	date_time tmp = *this;

	if (!hours)
		return tmp;

	s32 days = hours / 24;

	hours = (hours % 24) + hour();

	if (hours >= 24)
	{
		++days;
		hours %= 24;
	}
	else if (hours < 0)
	{
		--days;
		hours = 24 - hours;
	}

	if (days)
		tmp = tmp.add_days(days);

	tmp.hour(hours);
	return tmp;
}

date_time date_time::add_minutes(s32 minutes) const
{
	date_time tmp = *this;

	if (!minutes)
		return tmp;

	s32 hours = minutes / 60;

	minutes = (minutes % 60) + minute();

	if (minutes >= 60)
	{
		++hours;
		minutes %= 60;
	}
	else if (minutes < 0)
	{
		--hours;
		minutes = 60 - minutes;
	}

	if (hours > 0)
		tmp = tmp.add_hours(hours);

	tmp.minute(minutes);
	return tmp;
}

date_time date_time::add_seconds(s32 seconds) const
{
	date_time tmp = *this;

	if (!seconds)
		return tmp;

	s32 minutes = seconds / 60;

	seconds = (seconds % 60) + second();

	if (seconds >= 60)
	{
		++minutes;
		seconds %= 60;
	}
	else if (seconds < 0)
	{
		--minutes;
		seconds = 60 - seconds;
	}

	if (minutes > 0)
		tmp = tmp.add_minutes(minutes);

	tmp.second(seconds);
	return tmp;
}

date_time date_time::add_milliseconds(s32 milliseconds) const
{
	date_time tmp = *this;

	if (!milliseconds)
		return tmp;

	s32 seconds = milliseconds / 1000;

	milliseconds = (milliseconds % 1000) + millisecond();

	if (milliseconds >= 1000)
	{
		++seconds;
		milliseconds %= 1000;
	}
	else if (milliseconds < 0)
	{
		--seconds;
		milliseconds = 1000 - milliseconds;
	}

	if (seconds > 0)
		tmp = tmp.add_seconds(seconds);

	tmp.millisecond(milliseconds);
	return tmp;
}

date_time date_time::subtract(const time_span& dur) const
{
	bool negative = !dur.negative();

	time_span tmp = dur;
	tmp.negative(negative);
	return add(tmp);
}

date_time date_time::add(const time_span& dur) const
{
	s32 negate = dur.negative() ? -1 : 1;
	date_time tmp = *this;

	tmp = tmp.add_days(negate * dur.days());
	tmp = tmp.add_hours(negate * dur.hours());
	tmp = tmp.add_minutes(negate * dur.minutes());
	tmp = tmp.add_seconds(negate * dur.seconds());
	tmp = tmp.add_milliseconds(negate * dur.milliseconds());
	return tmp;
}

date_time date_time::add(const time& t) const
{
	date_time tmp = *this;

	tmp = tmp.add_hours(t.hour());
	tmp = tmp.add_minutes(t.minute());
	tmp = tmp.add_seconds(t.second());
	tmp = tmp.add_milliseconds(t.millisecond());
	return tmp;
}

date_time date_time::substract(const time& t) const
{
	date_time tmp = *this;

	tmp = tmp.add_hours(-1 * t.hour());
	tmp = tmp.add_minutes(-1 * t.minute());
	tmp = tmp.add_seconds(-1 * t.second());
	tmp = tmp.add_milliseconds(-1 * t.millisecond());
	return tmp;
}

time_span date_time::time_between(const date_time& dt) const
{
	if (dt == *this)
		return time_span(0, 0, 0, 0, 0);

	if (dt > *this)
	{
		time_span dur = dt.time_between(*this);
		dur.negative(true);
		return dur;
	}

	bool negative = false;
	s64 ms = (hour() * MS_PER_HOUR) + (minute() * MS_PER_MIN) + (second() * MS_PER_SEC) + millisecond();
	s64 dt_ms = (dt.hour() * MS_PER_HOUR) + (dt.minute() * MS_PER_MIN) + (dt.second() * MS_PER_SEC) + dt.millisecond();
	s64 total_ms = 0;
	date_time stop_date_time;

	//
	// Need to stop one day after in this case
	//
	if (dt_ms > ms)
	{
		stop_date_time = dt.add_days(1);
		total_ms = MS_PER_DAY - (dt_ms - ms);
	}
	else
	{
		stop_date_time = dt;
		total_ms = ms - dt_ms;
	}

	u32 hours = boost::lexical_cast<u32>(total_ms / MS_PER_HOUR);
	total_ms = total_ms % MS_PER_HOUR;

	u32 minutes = boost::lexical_cast<u32>(total_ms / MS_PER_MIN);
	total_ms = total_ms % MS_PER_MIN;

	u32 seconds = boost::lexical_cast<u32>(total_ms / MS_PER_SEC);
	total_ms = total_ms % MS_PER_SEC;

	u32 milliseconds = boost::lexical_cast<u32>(total_ms);
	u32 totaldays = 0;
	u32 currentyear = year();
	u32 currentday = day_of_year();

	while (currentyear > stop_date_time.year())
	{
		if (currentyear == year())
			totaldays += day_of_year();
		else
			totaldays += days_in_year(currentyear);

		currentyear--;
		currentday = day_of_year();
	}

	while (currentday > stop_date_time.day_of_year())
	{
		totaldays++;
		currentday--;
	}

	return time_span(totaldays, hours, minutes, seconds, milliseconds, negative);
}

bool date_time::is_leap_year(u16 year)
{
	return
		!(year % 400) ||
		(!(year % 4) && (year % 100));
}

bool date_time::valid_date(u16 year, u8 month, u8 day)
{
	if (year == 0 || month == 0 || day == 0)
		return false;

	if (month > 12)
		return false;

	return day <= days_in_month(year, month);
}

u16 date_time::days_in_year(u16 year)
{
	return is_leap_year(year) ? 366 : 365;
}

u8 date_time::days_in_month(u16 year, u8 month)
{
	u8 days_in_month = g_month_lengths[month - 1];

	if (month == 2 && is_leap_year(year))
		days_in_month++;

	return days_in_month;
}

u16 date_time::day_of_year(u16 year, u8 _month, u8 day)
{
	u16 day_of_year = 0;

	for (u8 month = 1; month < _month; month++)
		day_of_year += date_time::days_in_month(year, month);

	day_of_year += day;

	return day_of_year;
}

date_time date_time::reverse_day_of_year(u16 year, u16 day_of_year)
{
	u8 month;

	for (month = 1; month < 12; month++)
	{
		u32 days = date_time::days_in_month(year, month);

		if (days >= day_of_year)
			break;

		day_of_year -= days;
	}

	return date_time(year, month, boost::lexical_cast<u8>(day_of_year));
}

time_t date_time::mktime() const
{
	tm t;

	t.tm_year = year() - 1900;
	t.tm_mon = month() - 1;
	t.tm_mday = day();
	t.tm_hour = hour();
	t.tm_min = minute();
	t.tm_sec = second();

	return ::mktime(&t);
}

MYSQL_TIME date_time::mysql_time() const
{
	MYSQL_TIME t;

	t.year = year();
	t.month = month();
	t.day = day();
	t.hour = hour();
	t.minute = minute();
	t.second = second();
	t.neg = false;
	t.second_part = 0;
	t.time_type = !t.hour && !t.minute && !t.second ? MYSQL_TIMESTAMP_DATE : MYSQL_TIMESTAMP_DATETIME;

	return t;
}

double date_time::diff_time(const date_time& dt) const
{
	time_t time_val = mktime();
	time_t dt_time_val = dt.mktime();

	return ::difftime(time_val, dt_time_val);
}

date_time date_time::now()
{
	time_t local_time = ::time(NULL);
	tm time_struct;
	localtime_safe(&time_struct, &local_time);

	return date_time(time_struct);
}

date_time date_time::date() const
{
	return date_time(year(), month(), day());
}

date_time date_time::now_utc()
{
	time_t utc_time = ::time(NULL);
	tm ts;
	gmtime_safe(&ts, &utc_time);

	return date_time(ts);
}

bool date_time::set(const std::string& dt)
{
	if (dt.empty() ||
		dt.length() < 10)
		return false;

	u16 y = boost::lexical_cast<u16>(dt.substr(0, 4).c_str());
	u8 m = boost::lexical_cast<u8>(dt.substr(5, 2).c_str());
	u8 d = boost::lexical_cast<u8>(dt.substr(8, 2).c_str());

	if (dt.length() >= 13)
		time::set(dt.substr(11));

	return set(y, m, d, hour(), minute(), second(), millisecond());
}

const std::string date_time::str(bool with_millisecond) const
{
    char buffer[32];

	if (with_millisecond && millisecond())
		snprintf(buffer, sizeof(buffer), "%04i-%02i-%02i %02i:%02i:%02i.%03i", year(), month(), day(), hour(), minute(), second(), millisecond());
	else
		snprintf(buffer, sizeof(buffer), "%04i-%02i-%02i %02i:%02i:%02i", year(), month(), day(), hour(), minute(), second());

	return std::string(buffer);
}

const std::string date_time::str_date() const
{
    char buffer[32];

	snprintf(buffer, sizeof(buffer), "%04i-%02i-%02i", year(), month(), day());

	return std::string(buffer);
}

ostream& mariadb::operator << (ostream& os, const date_time& dt)
{
	os << dt.str();
	return os;
}
