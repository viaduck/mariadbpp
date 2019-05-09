//
//  M A R I A D B + +
//
//          Copyright Sylvain Rochette Langlois 2013,
//                    Frantisek Boranek 2015,
//                    The ViaDuck Project 2016 - 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef _MARIADB_TIME_HPP_
#define _MARIADB_TIME_HPP_

#include <iostream>
#include <mariadb++/time_span.hpp>

#include <mysql.h>

namespace mariadb {
/**
 * Class representing SQL time
 */
class time {
   public:
    /**
     * Construct time using given values
     *
     * @param hour Hours 0-23
     * @param minute Minutes 0-59
     * @param second Seconds 0-61 (for leap seconds)
     * @param millisecond Milliseconds 0-999
     */
    time(u8 hour = 0, u8 minute = 0, u8 second = 0, u16 millisecond = 0);

    /**
     * Copy constructor
     *
     * @param t Time to copy from
     */
    time(const time& t);

    /**
     * Construct time from time.h functions
     *
     * @param time_struct Timestruct to copy from
     */
    time(const tm& time_struct);

    /**
     * Construct time from time.h functions
     *
     * @param time Timestruct to copy from
     */
    time(const time_t& time);

    /**
     * Construct time from SQL time
     *
     * @param time SQL time to copy from
     */
    time(const MYSQL_TIME& time);

    /**
     * Construct time from string
     * The format needs to be hh[:mm][:ss][:nnn]
     * where less digits are possible and the delimiter may be any non digit
     *
     * @param t String containing time representation
     */
    time(const std::string& t);

    /**
     * Allow proper destruction in derived classes
     */
    virtual ~time() = default;

    /**
     * Compare this instance to given instance
     *
     * @param t Time to compare to
     * @return 1 if this instance is greater, -1 if t is greater, 0 on equality
     */
    int compare(const time& t) const;

    /**
     * Assigns a value to this instance
     */
    time& operator=(const time& t);

    /**
     * Checks for equality
     */
    bool operator==(const time& t) const;

    /**
     * Checks for unequality
     */
    bool operator!=(const time& t) const;

    /**
     * Checks if this instance is lesser than t
     */
    bool operator<(const time& t) const;

    /**
     * Checks if this instance is lesser or equal to t
     */
    bool operator<=(const time& t) const;

    /**
     * Checks if this instance is greater than t
     */
    bool operator>(const time& t) const;

    /**
     * Checks if this instance is greater or equal to t
     */
    bool operator>=(const time& t) const;

    /**
     * Get the current hour 0-23
     */
    u8 hour() const;

    /**
     * Set the current hour 0-23
     */
    u8 hour(u8 hour);

    /**
     * Get the current minute 0-59
     */
    u8 minute() const;

    /**
     * Set the current minute 0-59
     */
    u8 minute(u8 minute);

    /**
     * Get the current second 0-61 (leap second possible)
     */
    u8 second() const;

    /**
     * Set the current second 0-61 (leap second possible)
     */
    u8 second(u8 second);

    /**
     * Get the current millisecond 0-999
     */
    u16 millisecond() const;

    /**
     * Set the current millisecond 0-999
     */
    u16 millisecond(u16 millisecond);

    /**
     * Set the time from string
     * The format needs to be hh[:mm][:ss][.nnn]
     * where less digits are possible and the delimiter may be any non digit
     *
     * Examples:
     * h
     * h:mm.s?n
     * hh,mm!ss-n
     *
     * @return True on success
     */
    virtual bool set(const std::string& t);

    /**
     * Set the time from given values
     *
     * @param hour Hour to set
     * @param minute Minute to set
     * @param second Second to set
     * @param millisecond Milliseconds to set
     * @return True on success
     */
    bool set(u8 hour, u8 minute, u8 second, u16 millisecond);

    /**
     * Adds a certain amount of hours to the current time. Negative values subtract hours
     *
     * @param hours Number of hours to add
     * @return Time containing sum
     */
    time add_hours(s32 hours) const;

    /**
     * Adds a certain amount of minutes to the current time. Negative values subtract minutes
     *
     * @param minutes Number of minutes to add
     * @return Time containing sum
     */
    time add_minutes(s32 minutes) const;

    /**
     * Adds a certain amount of seconds to the current time. Negative values subtract seconds
     *
     * @param minutes Number of seconds to add
     * @return Time containing sum
     */
    time add_seconds(s32 seconds) const;

    /**
     * Adds a certain amount of milliseconds to the current time. Negative values subtract
     * milliseconds
     *
     * @param minutes Number of milliseconds to add
     * @return Time containing sum
     */
    time add_milliseconds(s32 milliseconds) const;

    /**
     * Subtracts the given timespan from the current time
     *
     * @param dur A duration to subtract
     * @return Time containing result
     */
    time subtract(const time_span& dur) const;

    /**
     * Adds the given timespan to the current time
     *
     * @param dur A duration to add
     * @return Time containing sum
     */
    time add(const time_span& dur) const;

    /**
     * Calculates the timespan between the current time instance and given instance t
     *
     * @param t Time to calculate the duration between
     * @return Timespan containing the duration
     */
    time_span time_between(const time& t) const;

    /**
     * Converts the time to time_t (time.h representation)
     *
     * @return Time as time.h time_t
     */
    time_t mktime() const;

    /**
 * Calculates the time difference using ::difftime.
 * Calculates (this - t)
 *
 * @return Difference in seconds as double
     */
    double diff_time(const time& t) const;

    /**
     * Converts the time to MySQL time representation. Note that MySQL time does not support
     * milliseconds
     *
     * @return Time as MYSQL_TIME with no milliseconds
     */
    MYSQL_TIME mysql_time() const;

    /**
     * Converts the time to a string with the format hh:mm:ss[.nnn]
     *
     * @param with_millisecond Indicates if milliseconds should be printed or not.
     *
     * @return String representing time with optional milliseconds
     */
    const std::string str_time(bool with_millisecond = false) const;

    /**
     * Uses time.h to determine the current time in the local timezone.
     *
     * @return Time representing now
     */
    static time now();

    /**
     * Uses time.h to determine the current time in UTC timezone.
     *
     * @return Time representing now in UTC
     */
    static time now_utc();

   protected:
    u8 m_hour;
    u8 m_minute;
    u8 m_second;
    u16 m_millisecond;
};

std::ostream& operator<<(std::ostream& os, const time& t);
}

#endif
