//
//  M A R I A D B + +
//
//          Copyright Sylvain Rochette Langlois 2013,
//                    The ViaDuck Project 2016 - 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <mariadb++/time_span.hpp>

using namespace mariadb;

time_span::time_span(u32 days, u8 hours, u8 minutes, u8 seconds, u16 milliseconds, bool negative) {
    set(days, hours, minutes, seconds, milliseconds, negative);
}

time_span::time_span(const time_span& dur) {
    set(dur.days(), dur.hours(), dur.minutes(), dur.seconds(), dur.milliseconds(), dur.negative());
}

int time_span::compare(const time_span& ts) const {
    if (negative() && !ts.negative()) return -1;

    if (!negative() && ts.negative()) return 1;

    if (zero() && ts.zero()) return 0;

    if (days() < ts.days()) return -1;

    if (days() > ts.days()) return 1;

    if (hours() < ts.hours()) return -1;

    if (hours() > ts.hours()) return 1;

    if (minutes() < ts.minutes()) return -1;

    if (minutes() > ts.minutes()) return 1;

    if (seconds() < ts.seconds()) return -1;

    if (seconds() > ts.seconds()) return 1;

    if (milliseconds() < ts.milliseconds()) return -1;

    return milliseconds() == ts.milliseconds() ? 0 : 1;
}

time_span& time_span::operator=(const time_span& ts) {
    set(ts.days(), ts.hours(), ts.minutes(), ts.seconds(), ts.milliseconds(), ts.negative());

    return *this;
}

bool time_span::operator==(const time_span& ts) const { return compare(ts) == 0; }

bool time_span::operator!=(const time_span& ts) const { return compare(ts) != 0; }

bool time_span::operator<(const time_span& ts) const { return compare(ts) < 0; }

bool time_span::operator<=(const time_span& ts) const { return compare(ts) <= 0; }

bool time_span::operator>(const time_span& ts) const { return compare(ts) > 0; }

bool time_span::operator>=(const time_span& ts) const { return compare(ts) >= 0; }

void time_span::set(u32 _days, u8 _hours, u8 _minutes, u8 _seconds, u16 _milliseconds,
                    bool _negative) {
    negative(_negative);
    days(_days);
    hours(_hours);
    minutes(_minutes);
    seconds(_seconds);
    milliseconds(_milliseconds);
}

bool time_span::zero() const {
    return m_days == 0 && m_hours == 0 && m_minutes == 0 && m_seconds == 0 && m_milliseconds == 0;
}

u32 time_span::days() const { return m_days; }

u32 time_span::days(u32 days) { return (m_days = days); }

u8 time_span::hours() const { return m_hours; }

u8 time_span::hours(u8 hours) {
    if (hours > 23) throw std::invalid_argument("Hours must be < 24");

    return (m_hours = hours);
}

u8 time_span::minutes() const { return m_minutes; }

u8 time_span::minutes(u8 minutes) {
    if (minutes > 59) throw std::invalid_argument("Hours must be < 60");

    return (m_minutes = minutes);
}

u8 time_span::seconds() const { return m_seconds; }

u8 time_span::seconds(u8 seconds) {
    if (seconds > 60) throw std::invalid_argument("Hours must be < 61");

    return (m_seconds = seconds);
}

u16 time_span::milliseconds() const { return m_milliseconds; }

u16 time_span::milliseconds(u16 milliseconds) {
    if (milliseconds > 999) throw std::invalid_argument("Hours must be < 1000");

    return (m_milliseconds = milliseconds);
}

bool time_span::negative() const { return m_negative; }

bool time_span::negative(bool negative) { return (m_negative = negative); }

u64 time_span::total_hours() const { return m_days * 24u + m_hours; }

u64 time_span::total_minutes() const { return total_hours() * 60u + m_minutes; }

u64 time_span::total_seconds() const { return total_minutes() * 60u + m_seconds; }

u64 time_span::total_milliseconds() const { return total_seconds() * 1000u + m_milliseconds; }

std::ostream& mariadb::operator<<(std::ostream& os, const time_span& ts) {
    if (ts.negative()) os << "negative ";

    os << ts.days() << " days, " << +ts.hours() << " hours, " << +ts.minutes() << " minutes, "
       << +ts.seconds() << " seconds, " << ts.milliseconds() << " milliseconds";

    return os;
}
