//
//  M A R I A D B + +
//
//	Author   : Sylvain Rochette Langlois
//	License  : Boost Software License (http://www.boost.org/users/license.html)
//

#include <mariadb++/time_span.hpp>

using namespace mariadb;
using namespace std;

//
// Constructors
//
time_span::time_span(u32 days, u32 hours, u32 minutes, u32 seconds, u32 milliseconds, bool negative) :
	m_days(0),
	m_hours(0),
	m_minutes(0),
	m_seconds(0),
	m_milliseconds(0)
{
	set(days, hours, minutes, seconds, milliseconds, negative);
}

time_span::time_span(const time_span& dur) :
	m_days(0),
	m_hours(0),
	m_minutes(0),
	m_seconds(0),
	m_milliseconds(0)
{
	set(dur.days(), dur.hours(), dur.minutes(), dur.seconds(), dur.milliseconds(), dur.negative());
}

//
// Operators
//
int time_span::compare(const time_span& ts) const
{
	if (zero() && ts.zero())
		return 0;

	if (days() < ts.days())
		return -1;

	if (days() > ts.days())
		return 1;

	if (hours() < ts.hours())
		return -1;

	if (hours() > ts.hours())
		return 1;

	if (minutes() < ts.minutes())
		return -1;

	if (minutes() > ts.minutes())
		return 1;

	if (seconds() < ts.seconds())
		return -1;

	if (seconds() > ts.seconds())
		return 1;

	if (milliseconds() < ts.milliseconds())
		return -1;

	return milliseconds() == ts.milliseconds() ? 0 : 1;
}

time_span& time_span::operator = (const time_span& ts)
{
	set(ts.days(), ts.hours(), ts.minutes(), ts.seconds(), ts.milliseconds(), ts.negative());

	return *this;
}

bool time_span::operator == (const time_span& ts) const
{
	return compare(ts) == 0;
}

bool time_span::operator != (const time_span& ts) const
{
	return compare(ts) != 0;
}

bool time_span::operator < (const time_span& ts) const
{
	return compare(ts) < 0;
}

bool time_span::operator <= (const time_span& ts) const
{
	return compare(ts) <= 0;
}

bool time_span::operator > (const time_span& ts) const
{
	return compare(ts) > 0;
}

bool time_span::operator >= (const time_span& ts) const
{
	return compare(ts) >= 0;
}

void time_span::set(u32 _days, u32 _hours, u32 _minutes, u32 _seconds, u32 _milliseconds, bool _negative)
{
	negative(_negative);
	days(_days);
	hours(_hours);
	minutes(_minutes);
	seconds(_seconds);
	milliseconds(_milliseconds);
}

bool time_span::zero() const
{
	return m_days == 0
		&& m_hours == 0
		&& m_minutes == 0
		&& m_seconds == 0
		&& m_milliseconds == 0;
}

u32 time_span::days() const
{
	return m_days;
}

u32 time_span::days(u32 days)
{
	m_days = days;
	return m_days;
}

u8 time_span::hours() const
{
	return m_hours;
}

u8 time_span::hours(u32 hours)
{
	m_days += (hours / 24);
	m_hours = hours % 24;
	return m_hours;
}

u8 time_span::minutes() const
{
	return m_minutes;
}

u8 time_span::minutes(u32 minutes)
{
	while (minutes >= 60)
	{
		hours(++m_hours);
		minutes -= 60;
	}

	m_minutes = minutes;
	return m_minutes;
}

u8 time_span::seconds() const
{
	return m_seconds;
}

u8 time_span::seconds(u32 seconds)
{
	while (seconds >= 60)
	{
		minutes(++m_minutes);
		seconds -= 60;
	}

	m_seconds = seconds;
	return m_seconds;
}

u16 time_span::milliseconds() const
{
	return m_milliseconds;
}

u16 time_span::milliseconds(u32 milliseconds)
{
	while (milliseconds >= 1000)
	{
		seconds(++m_seconds);
		milliseconds -= 1000;
	}

	m_milliseconds = milliseconds;
	return m_milliseconds;
}

bool time_span::negative() const
{
	return m_negative;
}

bool time_span::negative(bool negative)
{
	m_negative = negative;

	return m_negative;
}

u64 time_span::total_hours() const
{
	return m_days * 24 + m_hours;
}

u64 time_span::total_minutes() const
{
	return total_hours() * 60 + m_minutes;
}

u64 time_span::total_seconds() const
{
	return total_minutes() * 60 + m_seconds;
}

u64 time_span::total_milliseconds() const
{
	return total_seconds() * 1000 + m_milliseconds;
}

ostream& mariadb::operator << (ostream& os, const time_span& ts)
{
	if (ts.negative())
		os << "negative ";

	os << ts.days() << " days, " << (s32)ts.hours() << " hours, " << (s32)ts.minutes() << " minutes, " << (s32)ts.seconds() << " seconds, " << ts.milliseconds() << " milliseconds";

	return os;
}
