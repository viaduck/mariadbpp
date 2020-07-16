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
#include <mariadb++/time.hpp>
#include <mariadb++/conversion_helper.hpp>
#include <iomanip>
#include <chrono>
#include "private.hpp"

#define MS_PER_SEC 1000
#define MS_PER_MIN (MS_PER_SEC * 60)
#define MS_PER_HOUR (MS_PER_MIN * 60)
#define MS_PER_DAY (MS_PER_HOUR * 24)

mariadb::time::time(u8 hour, u8 minute, u8 second, u16 millisecond) {
    set(hour, minute, second, millisecond);
}

mariadb::time::time(const time& t) { set(t.hour(), t.minute(), t.second(), t.millisecond()); }

mariadb::time::time(const tm& t) { set(t.tm_hour, t.tm_min, t.tm_sec, 0); }

mariadb::time::time(const time_t& t) {
    tm ts;
    localtime_safe(&ts, &t);

    set(ts.tm_hour, ts.tm_min, ts.tm_sec, 0);
}

mariadb::time::time(const MYSQL_TIME& t) { set(t.hour, t.minute, t.second, t.second_part / 1000); }

mariadb::time::time(const std::string& t) { set(t); }

int mariadb::time::compare(const time& t) const {
    if (hour() < t.hour()) return -1;

    if (hour() > t.hour()) return 1;

    if (minute() < t.minute()) return -1;

    if (minute() > t.minute()) return 1;

    if (second() < t.second()) return -1;

    if (second() > t.second()) return 1;

    if (millisecond() < t.millisecond()) return -1;

    return millisecond() == t.millisecond() ? 0 : 1;
}

mariadb::time& mariadb::time::operator=(const time& t) {
    set(t.hour(), t.minute(), t.second(), t.millisecond());
    return *this;
}

bool mariadb::time::operator==(const time& t) const { return compare(t) == 0; }

bool mariadb::time::operator!=(const time& t) const { return compare(t) != 0; }

bool mariadb::time::operator<(const time& t) const { return compare(t) < 0; }

bool mariadb::time::operator<=(const time& t) const { return compare(t) <= 0; }

bool mariadb::time::operator>(const time& t) const { return compare(t) > 0; }

bool mariadb::time::operator>=(const time& t) const { return compare(t) >= 0; }

bool mariadb::time::set(u8 hour, u8 minute, u8 second, u16 millisecond) {
    m_hour = hour;
    m_minute = minute;
    m_second = second;
    m_millisecond = millisecond;
    return true;
}

mariadb::u8 mariadb::time::hour() const { return m_hour; }

mariadb::u8 mariadb::time::hour(u8 hour) {
    if (hour > 23)
        MARIADB_ERROR_THROW_TIME(hour, minute(), second(), millisecond());

    m_hour = hour;

    return m_hour;
}

mariadb::u8 mariadb::time::minute() const { return m_minute; }

mariadb::u8 mariadb::time::minute(u8 minute) {
    if (minute > 59)
        MARIADB_ERROR_THROW_TIME(hour(), minute, second(), millisecond());

    m_minute = minute;

    return m_minute;
}

mariadb::u8 mariadb::time::second() const { return m_second; }

mariadb::u8 mariadb::time::second(u8 second) {
    if (second > 61)
        MARIADB_ERROR_THROW_TIME(hour(), minute(), second, millisecond());

    m_second = second;

    return m_second;
}

mariadb::u16 mariadb::time::millisecond() const { return m_millisecond; }

mariadb::u16 mariadb::time::millisecond(u16 millisecond) {
    if (millisecond > 999)
        MARIADB_ERROR_THROW_TIME(hour(), minute(), second(), millisecond);

    m_millisecond = millisecond;

    return m_millisecond;
}

mariadb::time mariadb::time::add_hours(s32 hours) const {
    mariadb::time tmp = *this;

    if (hours == 0) return tmp;

    // hour overflow does not matter, as we dont care about days
    hours = (hours + hour() + 24) % 24;
    tmp.hour(hours);
    return tmp;
}

mariadb::time mariadb::time::add_minutes(s32 minutes) const {
    mariadb::time tmp = *this;

    if (minutes == 0) return tmp;

    s32 hours = minutes / 60;
    minutes = (minutes % 60) + minute();

    if (minutes >= 60)
        ++hours;
    else if (minutes < 0)
        --hours;

    if (hours != 0) tmp = tmp.add_hours(hours);

    tmp.minute((minutes + 60) % 60);
    return tmp;
}

mariadb::time mariadb::time::add_seconds(s32 seconds) const {
    mariadb::time tmp = *this;

    if (seconds == 0) return tmp;

    s32 minutes = seconds / 60;
    seconds = (seconds % 60) + second();

    if (seconds >= 60)
        ++minutes;
    else if (seconds < 0)
        --minutes;

    if (minutes != 0) tmp = tmp.add_minutes(minutes);

    tmp.second((seconds + 60) % 60);
    return tmp;
}

mariadb::time mariadb::time::add_milliseconds(s32 milliseconds) const {
    mariadb::time tmp = *this;

    if (milliseconds == 0) return tmp;

    s32 seconds = milliseconds / 1000;
    milliseconds = (milliseconds % 1000) + millisecond();

    if (milliseconds > 999)
        ++seconds;
    else if (milliseconds < 0)
        --seconds;

    if (seconds != 0) tmp = tmp.add_seconds(seconds);

    tmp.millisecond((milliseconds + 1000) % 1000);
    return tmp;
}

mariadb::time mariadb::time::subtract(const time_span& dur) const {
    // copy and negate timespan
    time_span tmp = dur;
    tmp.negative(!dur.negative());

    // add negated
    return add(tmp);
}

mariadb::time mariadb::time::add(const time_span& dur) const {
    // negate if needed
    s32 negative = dur.negative() ? -1 : 1;
    time tmp = *this;

    tmp.add_hours(negative * dur.hours());
    tmp.add_minutes(negative * dur.minutes());
    tmp.add_seconds(negative * dur.seconds());
    tmp.add_milliseconds(negative * dur.milliseconds());
    return tmp;
}

mariadb::time_span mariadb::time::time_between(const time& t) const {
    // equal
    if (t == *this) return time_span(0, 0, 0, 0, 0);

    // recursive call with negation
    if (t > *this) {
        time_span dur = t.time_between(*this);
        dur.negative(true);
        return dur;
    }

    // calculate the ms representation of both
    s64 ms =
        (hour() * MS_PER_HOUR) + (minute() * MS_PER_MIN) + (second() * MS_PER_SEC) + millisecond();
    s64 t_ms = (t.hour() * MS_PER_HOUR) + (t.minute() * MS_PER_MIN) + (t.second() * MS_PER_SEC) +
               t.millisecond();
    s64 total_ms = 0;

    // subtract the lesser from greater // TODO: is this case ever reached since we have recusive
    // call on t > this?
    if (t_ms > ms)
        total_ms = MS_PER_DAY - (t_ms - ms);
    else
        total_ms = ms - t_ms;

    u32 hours = static_cast<u32>(total_ms / MS_PER_HOUR);
    total_ms = total_ms % MS_PER_HOUR;

    u32 minutes = static_cast<u32>(total_ms / MS_PER_MIN);
    total_ms = total_ms % MS_PER_MIN;

    u32 seconds = static_cast<u32>(total_ms / MS_PER_SEC);
    total_ms = total_ms % MS_PER_SEC;

    return time_span(0, hours, minutes, seconds, static_cast<u32>(total_ms), false);
}

time_t mariadb::time::mktime() const {
    tm time_struct;

    time_struct.tm_year = 1900;
    time_struct.tm_mon = 0;
    time_struct.tm_mday = 1;
    time_struct.tm_hour = hour();
    time_struct.tm_min = minute();
    time_struct.tm_sec = second();

    return ::mktime(&time_struct);
}

MYSQL_TIME mariadb::time::mysql_time() const {
    MYSQL_TIME t;

    t.year = 0;
    t.month = 0;
    t.day = 0;
    t.hour = hour();
    t.minute = minute();
    t.second = second();
    t.second_part = millisecond() * 1000u;
    t.neg = false;
    t.time_type = MYSQL_TIMESTAMP_TIME;
    return t;
}

double mariadb::time::diff_time(const time& t) const {
    time_t time_val = mktime();
    time_t t_time_val = t.mktime();

    return ::difftime(time_val, t_time_val);
}

bool mariadb::time::is_valid() const {
    return time::valid_time(hour(), minute(), second(), millisecond());
}

bool mariadb::time::valid_time(u8 hour, u8 minute, u8 second, u16 millisecond) {
    // seconds go up to 61 to allow for leap seconds
    return hour < 24 && minute < 60 && second <= 61 && millisecond < 1000;
}

mariadb::time mariadb::time::now() {
    using namespace std::chrono;
    auto now = system_clock::now();

    time_t local_time = system_clock::to_time_t(now);
    tm ts;
    localtime_safe(&ts, &local_time);

    auto millis = duration_cast<milliseconds>(now.time_since_epoch()).count() % 1000;
    return mariadb::time(ts).add_milliseconds(static_cast<s32>(millis));
}

mariadb::time mariadb::time::now_utc() {
    using namespace std::chrono;
    auto now = system_clock::now();

    time_t utc_time = system_clock::to_time_t(now);
    tm ts;
    gmtime_safe(&ts, &utc_time);

    auto millis = duration_cast<milliseconds>(now.time_since_epoch()).count() % 1000;
    return mariadb::time(ts).add_milliseconds(static_cast<s32>(millis));
}

bool mariadb::time::set(const std::string& t) {
    std::stringstream stream(t);

    u8 h, m, s;
    u16 s_h = 0, s_m = 0, s_s = 0, s_ms = 0;
    char delim;

    // read formatted hours, check overflow before cast
    if (stream >> s_h && s_h < 24) {
        h = static_cast<u8>(s_h);
        if (stream.eof()) return set(h, 0, 0, 0);

        // read formatted minutes, check overflow before cast
        if (stream >> delim && stream >> s_m && s_m < 60) {
            m = static_cast<u8>(s_m);
            if (stream.eof()) return set(h, m, 0, 0);

            // read formatted seconds, check overflow before cast
            if (stream >> delim && stream >> s_s && s_s < 62) {
                s = static_cast<u8>(s_s);
                if (stream.eof()) return set(h, m, s, 0);

                // read formatted millis
                if (stream >> delim && stream >> s_ms) return set(h, m, s, s_ms);
            }
        }
    }

    throw std::invalid_argument("invalid time format");
}

const std::string mariadb::time::str_time(bool with_millisecond) const {
    std::stringstream stream;
    stream.fill('0');

    /*
     * Note: the magic unary + forces conversion of unsigned char (u8) to a numeric printing type,
     * otherwise it will
     * be printed as char
     */
    stream << std::setw(2) << +hour() << ":" << std::setw(2) << +minute() << ":" << std::setw(2)
           << +second();

    if (with_millisecond) stream << "." << std::setw(3) << millisecond();

    return stream.str();
}

std::ostream& mariadb::operator<<(std::ostream& os, const time& t) {
    os << t.str_time(true);
    return os;
}
