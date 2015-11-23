//
//  M A R I A D B + +
//
//	Author   : Sylvain Rochette Langlois
//	License  : Boost Software License (http://www.boost.org/users/license.html)
//

#ifndef _MARIADB_TIME_SPAN_HPP_
#define _MARIADB_TIME_SPAN_HPP_

#include <sstream>
#include <mariadb++/types.hpp>

namespace mariadb
{
	class time_span
	{
	public:
		//
		// Constructor
		//
		time_span(u32 days = 0, u32 hours = 0, u32 minutes = 0, u32 seconds = 0, u32 milliseconds = 0, bool negative = false);
		time_span(const time_span& dur);

		//
		// Operators
		//
		int compare(const time_span& rhs) const;
		time_span& operator = (const time_span& rhs);
		bool operator == (const time_span& rhs) const;
		bool operator != (const time_span& rhs) const;
		bool operator < (const time_span& rhs) const;
		bool operator <= (const time_span& rhs) const;
		bool operator > (const time_span& rhs) const;
		bool operator >= (const time_span& rhs) const;

		//
		// Get / Set
		//
		void set(u32 days = 0, u32 hours = 0, u32 minutes = 0, u32 seconds = 0, u32 milliseconds = 0, bool negative = false);
		bool zero() const;
		bool negative() const;
		bool negative(bool negative);

		u32 days() const;
		u32 days(u32 day);
		u8 hours() const;
		u8 hours(u32 hour);
		u8 minutes() const;
		u8 minutes(u32 minute);
		u8 seconds() const;
		u8 seconds(u32 second);
		u16 milliseconds() const;
		u16 milliseconds(u32 millisecond);

		u64 total_hours() const;
		u64 total_minutes() const;
		u64 total_seconds() const;
		u64 total_milliseconds() const;

	private:
		bool m_negative;
		u32  m_days;
		u8   m_hours;
		u8   m_minutes;
		u8   m_seconds;
		u16  m_milliseconds;
	};

	std::ostream& operator << (std::ostream& os, const time_span& ts);
}

#endif
