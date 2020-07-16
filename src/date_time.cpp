//
//  M A R I A D B + +
//
//          Copyright Sylvain Rochette Langlois 2013,
//                    Frantisek Boranek 2015,
//                    The ViaDuck Project 2016 - 2020.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <mysql.h>
#include <sstream>
#include <mariadb++/exceptions.hpp>
#include <mariadb++/date_time.hpp>
#include <mariadb++/conversion_helper.hpp>
#include <iomanip>
#include <chrono>
#include "private.hpp"

using namespace mariadb;

namespace {
const u8 g_month_lengths[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
}

#define MS_PER_SEC 1000
#define MS_PER_MIN (MS_PER_SEC * 60)
#define MS_PER_HOUR (MS_PER_MIN * 60)
#define MS_PER_DAY (MS_PER_HOUR * 24)

date_time::date_time(u16 year, u8 month, u8 day, u8 hour, u8 minute, u8 second, u16 millisecond) : time() {
    set(year, month, day, hour, minute, second, millisecond);
}

date_time::date_time(const date_time& dt) : time() {
    set(dt.year(), dt.month(), dt.day(), dt.hour(), dt.minute(), dt.second(), dt.millisecond());
}

date_time::date_time(const time& t) : time() {
    set(1900, 1, 1, t.hour(), t.minute(), t.second(), t.millisecond());
}

date_time::date_time(const tm& t) : time() {
    set(t.tm_year + 1900, t.tm_mon + 1, t.tm_mday, t.tm_hour, t.tm_min, t.tm_sec, 0);
}

date_time::date_time(const time_t& time) : mariadb::time() {
    tm t;
    localtime_safe(&t, &time);

    set(t.tm_year + 1900, t.tm_mon + 1, t.tm_mday, t.tm_hour, t.tm_min, t.tm_sec, 0);
}

date_time::date_time(const MYSQL_TIME& t) : time() {
    set(t.year, t.month, t.day, t.hour, t.minute, t.second, t.second_part / 1000);
}

date_time::date_time(const std::string& dt) : time() { set(dt); }

int date_time::compare(const date_time& dt) const {
    if (year() < dt.year()) return -1;

    if (year() > dt.year()) return 1;

    if (month() < dt.month()) return -1;

    if (month() > dt.month()) return 1;

    if (day() < dt.day()) return -1;

    if (day() > dt.day()) return 1;

    return time::compare(dt);
}

date_time& date_time::operator=(const date_time& dt) {
    set(dt.year(), dt.month(), dt.day(), dt.hour(), dt.minute(), dt.second(), dt.millisecond());
    return *this;
}

bool date_time::operator==(const date_time& dt) const { return compare(dt) == 0; }

bool date_time::operator!=(const date_time& dt) const { return compare(dt) != 0; }

bool date_time::operator<(const date_time& dt) const { return compare(dt) < 0; }

bool date_time::operator<=(const date_time& dt) const { return compare(dt) <= 0; }

bool date_time::operator>(const date_time& dt) const { return compare(dt) > 0; }

bool date_time::operator>=(const date_time& dt) const { return compare(dt) >= 0; }

bool date_time::set(u16 year, u8 month, u8 day) {
    m_year = year;
    m_month = month;
    m_day = day;
    return true;
}

bool date_time::set(u16 year, u8 month, u8 day, u8 hour, u8 minute, u8 second, u16 millisecond) {
    m_year = year;
    m_month = month;
    m_day = day;

    return time::set(hour, minute, second, millisecond);
}

u16 date_time::year() const { return m_year; }

u16 date_time::year(u16 year) {
    if (year == 0)
        MARIADB_ERROR_THROW_DATE(year, month(), day(), hour(), minute(), second(), millisecond());

    m_year = year;

    if (!valid_date(year, month(), day())) {
        m_month = 1;
        m_day = 1;
    }

    return m_year;
}

u8 date_time::month() const { return m_month; }

u8 date_time::month(u8 month) {
    if (month == 0 || month > 12)
        MARIADB_ERROR_THROW_DATE(year(), month, day(), hour(), minute(), second(), millisecond());

    m_month = month;

    u8 days_inmonth = days_in_month(year(), month);

    if (day() > days_inmonth) m_day = 1;

    return m_month;
}

u8 date_time::day() const { return m_day; }

u8 date_time::day(u8 day) {
    u8 days_inmonth = days_in_month(year(), month());

    if (day == 0 || day > days_inmonth)
        MARIADB_ERROR_THROW_DATE(year(), month(), day, hour(), minute(), second(), millisecond());

    m_day = day;

    return m_day;
}

u16 date_time::day_of_year() const { return day_of_year(year(), month(), day()); }

u16 date_time::day_of_year(u16 day_of_year) {
    date_time new_date = reverse_day_of_year(year(), day_of_year);

    month(new_date.month());
    day(new_date.day());

    return day_of_year;
}

date_time date_time::add_years(s32 years) const {
    date_time tmp = *this;

    if (!years) return tmp;

    tmp.year(m_year + years);
    return tmp;
}

date_time date_time::add_months(s32 months) const {
    date_time tmp = *this;

    if (!months) return tmp;

    s32 years = months / 12;

    months = (months % 12) + month();

    if (months >= 12) {
        ++years;
        months %= 12;
    } else if (months < 0) {
        --years;
        months = 12 - months;
    }

    if (years) tmp = tmp.add_years(years);

    tmp.month(months);
    return tmp;
}

date_time date_time::add_days(s32 days) const {
    date_time tmp = *this;

    if (!days) return tmp;

    s32 dir = days > 0 ? 1 : -1;
    u16 y = year();
    s32 days_in = days_in_year(y);
    s32 day_of = day_of_year() + days;

    while (day_of > days_in || day_of < 0) {
        day_of -= days_in * dir;
        y += dir;
        days_in = days_in_year(y);
    }

    tmp.year(y);
    tmp.day_of_year(day_of);
    return tmp;
}

date_time date_time::add_hours(s32 hours) const {
    date_time tmp = *this;

    if (!hours) return tmp;

    s32 days = hours / 24;

    hours = (hours % 24) + hour();

    if (hours >= 24) {
        ++days;
        hours %= 24;
    } else if (hours < 0) {
        --days;
        hours = 24 - hours;
    }

    if (days) tmp = tmp.add_days(days);

    tmp.hour(hours);
    return tmp;
}

date_time date_time::add_minutes(s32 minutes) const {
    date_time tmp = *this;

    if (!minutes) return tmp;

    s32 hours = minutes / 60;

    minutes = (minutes % 60) + minute();

    if (minutes >= 60) {
        ++hours;
        minutes %= 60;
    } else if (minutes < 0) {
        --hours;
        minutes = 60 - minutes;
    }

    if (hours > 0) tmp = tmp.add_hours(hours);

    tmp.minute(minutes);
    return tmp;
}

date_time date_time::add_seconds(s32 seconds) const {
    date_time tmp = *this;

    if (!seconds) return tmp;

    s32 minutes = seconds / 60;

    seconds = (seconds % 60) + second();

    if (seconds >= 60) {
        ++minutes;
        seconds %= 60;
    } else if (seconds < 0) {
        --minutes;
        seconds = 60 - seconds;
    }

    if (minutes > 0) tmp = tmp.add_minutes(minutes);

    tmp.second(seconds);
    return tmp;
}

date_time date_time::add_milliseconds(s32 milliseconds) const {
    date_time tmp = *this;

    if (!milliseconds) return tmp;

    s32 seconds = milliseconds / 1000;

    milliseconds = (milliseconds % 1000) + millisecond();

    if (milliseconds >= 1000) {
        ++seconds;
        milliseconds %= 1000;
    } else if (milliseconds < 0) {
        --seconds;
        milliseconds = 1000 - milliseconds;
    }

    if (seconds > 0) tmp = tmp.add_seconds(seconds);

    tmp.millisecond(milliseconds);
    return tmp;
}

date_time date_time::subtract(const time_span& dur) const {
    bool negative = !dur.negative();

    time_span tmp = dur;
    tmp.negative(negative);
    return add(tmp);
}

date_time date_time::add(const time_span& dur) const {
    s32 negate = dur.negative() ? -1 : 1;
    date_time tmp = *this;

    tmp = tmp.add_days(negate * dur.days());
    tmp = tmp.add_hours(negate * dur.hours());
    tmp = tmp.add_minutes(negate * dur.minutes());
    tmp = tmp.add_seconds(negate * dur.seconds());
    tmp = tmp.add_milliseconds(negate * dur.milliseconds());
    return tmp;
}

date_time date_time::add(const time& t) const {
    date_time tmp = *this;

    tmp = tmp.add_hours(t.hour());
    tmp = tmp.add_minutes(t.minute());
    tmp = tmp.add_seconds(t.second());
    tmp = tmp.add_milliseconds(t.millisecond());
    return tmp;
}

date_time date_time::substract(const time& t) const {
    date_time tmp = *this;

    tmp = tmp.add_hours(-1 * t.hour());
    tmp = tmp.add_minutes(-1 * t.minute());
    tmp = tmp.add_seconds(-1 * t.second());
    tmp = tmp.add_milliseconds(-1 * t.millisecond());
    return tmp;
}

time_span date_time::time_between(const date_time& dt) const {
    if (dt == *this) return time_span();

    if (dt > *this) {
        time_span dur = dt.time_between(*this);
        dur.negative(true);
        return dur;
    }

    s64 ms =
        (hour() * MS_PER_HOUR) + (minute() * MS_PER_MIN) + (second() * MS_PER_SEC) + millisecond();
    s64 dt_ms = (dt.hour() * MS_PER_HOUR) + (dt.minute() * MS_PER_MIN) +
                (dt.second() * MS_PER_SEC) + dt.millisecond();
    s64 total_ms = 0;
    date_time stop_date_time;

    //
    // Need to stop one day after in this case
    //
    if (dt_ms > ms) {
        stop_date_time = dt.add_days(1);
        total_ms = MS_PER_DAY - (dt_ms - ms);
    } else {
        stop_date_time = dt;
        total_ms = ms - dt_ms;
    }

    u32 hours = static_cast<u32>(total_ms / MS_PER_HOUR);
    total_ms = total_ms % MS_PER_HOUR;

    u32 minutes = static_cast<u32>(total_ms / MS_PER_MIN);
    total_ms = total_ms % MS_PER_MIN;

    u32 seconds = static_cast<u32>(total_ms / MS_PER_SEC);
    total_ms = total_ms % MS_PER_SEC;

    u32 milliseconds = static_cast<u32>(total_ms);

    u32 totaldays = 0;
    u16 currentyear = year();

    while (currentyear > stop_date_time.year()) {
        if (currentyear == year())
            totaldays += day_of_year();
        else
            totaldays += days_in_year(currentyear);

        currentyear--;
    }

    if (year() == stop_date_time.year())
        totaldays += day_of_year() - stop_date_time.day_of_year();
    else
        totaldays += days_in_year(currentyear) - stop_date_time.day_of_year();

    /*while (currentday > stop_date_time.day_of_year())
    {
            totaldays++;
            currentday--;
    }*/

    return time_span(totaldays, hours, minutes, seconds, milliseconds, false);
}

bool date_time::is_valid() const {
    return valid_date(year(), month(), day()) && time::is_valid();
}

bool date_time::is_leap_year(u16 year) {
    /*
     * Leap year rule:
     * every year divisible by 400 is a leap year
     * every year divisible by 4 is a leap year if its not divisible by 100
     */
    return (year % 400 == 0) || (year % 4 == 0 && year % 100 != 0);
}

bool date_time::valid_date(u16 year, u8 month, u8 day) {
    if (year == 0 || month == 0 || day == 0) return false;

    if (month > 12) return false;

    return day <= days_in_month(year, month);
}

u16 date_time::days_in_year(u16 year) { return is_leap_year(year) ? u16(366) : u16(365); }

u8 date_time::days_in_month(u16 year, u8 month) {
    u8 days_in_month = g_month_lengths[month - 1];

    if (month == 2 && is_leap_year(year)) days_in_month++;

    return days_in_month;
}

u16 date_time::day_of_year(u16 year, u8 _month, u8 day) {
    u16 day_of_year = 0;

    for (u8 month = 1; month < _month; month++)
        day_of_year += date_time::days_in_month(year, month);

    day_of_year += day;

    return day_of_year;
}

date_time date_time::reverse_day_of_year(u16 year, u16 day_of_year) {
    u8 month;

    for (month = 1; month < 12; month++) {
        u32 days = date_time::days_in_month(year, month);

        if (days >= day_of_year) break;

        day_of_year -= days;
    }

    return date_time(year, month, static_cast<u8>(day_of_year));
}

time_t date_time::mktime() const {
    tm t;

    t.tm_year = year() - 1900;
    t.tm_mon = month() - 1;
    t.tm_mday = day();
    t.tm_hour = hour();
    t.tm_min = minute();
    t.tm_sec = second();

    return ::mktime(&t);
}

MYSQL_TIME date_time::mysql_time() const {
    MYSQL_TIME t;

    t.year = year();
    t.month = month();
    t.day = day();
    t.hour = hour();
    t.minute = minute();
    t.second = second();
    t.second_part = millisecond() * 1000u;
    t.neg = false;
    t.time_type =
        !t.hour && !t.minute && !t.second ? MYSQL_TIMESTAMP_DATE : MYSQL_TIMESTAMP_DATETIME;

    return t;
}

double date_time::diff_time(const date_time& dt) const {
    time_t time_val = mktime();
    time_t dt_time_val = dt.mktime();

    return ::difftime(time_val, dt_time_val);
}

date_time date_time::date() const { return date_time(year(), month(), day()); }

date_time date_time::now() {
    using namespace std::chrono;
    auto now = system_clock::now();

    time_t local_time = system_clock::to_time_t(now);
    tm ts;
    localtime_safe(&ts, &local_time);

    auto millis = duration_cast<milliseconds>(now.time_since_epoch()).count() % 1000;
    return date_time(ts).add_milliseconds(static_cast<s32>(millis));
}

date_time date_time::now_utc() {
    using namespace std::chrono;
    auto now = system_clock::now();

    time_t utc_time = system_clock::to_time_t(now);
    tm ts;
    gmtime_safe(&ts, &utc_time);

    auto millis = duration_cast<milliseconds>(now.time_since_epoch()).count() % 1000;
    return date_time(ts).add_milliseconds(static_cast<s32>(millis));
}

bool date_time::set(const std::string& dt) {
    std::stringstream stream(dt);

    u8 m, d;
    u16 s_y = 0, s_m = 0, s_d = 0;
    char delim;

    // read formatted years, check overflow before cast
    if (stream >> s_y) {
        if (stream.eof()) return set(s_y, 0, 0);

        // read formatted months, check overflow before cast
        if (stream >> delim && stream >> s_m && s_m < 13) {
            m = static_cast<u8>(s_m);
            if (stream.eof()) return set(s_y, m, 0);

            // read formatted days, check overflow before cast
            if (stream >> delim && stream >> s_d && s_d < 32) {
                d = static_cast<u8>(s_d);
                if (stream.eof()) return set(s_y, m, d);

                // extract remaining string
                std::string rest;
                std::getline(stream, rest);

                // set time from string
                return set(s_y, m, d) && time::set(rest);
            }
        }
    }

    throw std::invalid_argument("invalid date format");
}

const std::string date_time::str(bool with_millisecond) const {
    std::stringstream stream;
    stream.fill('0');

    stream << str_date() << " " << time::str_time(with_millisecond);
    return stream.str();
}

const std::string date_time::str_date() const {
    std::stringstream stream;
    stream.fill('0');

    /*
    * Note: the magic unary + forces conversion of unsigned char (u8) to a numeric printing type,
    * otherwise it will
    * be printed as char
    */
    stream << std::setw(4) << year() << "-" << std::setw(2) << +month() << "-" << std::setw(2)
           << +day();
    return stream.str();
}

std::ostream& mariadb::operator<<(std::ostream& os, const date_time& dt) {
    os << dt.str(true);
    return os;
}
