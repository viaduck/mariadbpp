//
//  M A R I A D B + +
//
//          Copyright Sylvain Rochette Langlois 2013,
//                    The ViaDuck Project 2016 - 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef _MARIADB_DATE_TIME_HPP_
#define _MARIADB_DATE_TIME_HPP_

#include <iostream>
#include <mariadb++/time.hpp>

namespace mariadb {
/**
 * Class used to represent SQL date_time
 */
class date_time : public time {
   public:
    /**
     * Construct date_time from given data. An exception will be thrown on invalid data.
     *
     * @param year The year to set.
     * @param month The month to set. [1-12]
     * @param day The day to set. Make sure it exists in the month. [1-28/39/30/31]
     * @param hour The hour to set. [0-23}
     * @param minute The minute to set. [0-59]
     * @param second The second to set. [0-60]
     * @param millisecond The millisecond to set. [0-999]
     */
    date_time(u16 year = 1970, u8 month = 1, u8 day = 1, u8 hour = 0, u8 minute = 0, u8 second = 0,
              u16 millisecond = 0);

    /**
     * Copy constructor
     *
     * @param dt Datetime to copy from
     */
    date_time(const date_time& dt);

    /**
     * Construct date_time with given time. Sets the date to Jan, 1st of 1900
     *
     * @param t Time to copy from
     */
    date_time(const time& t);

    /**
     * Construct date_time from tm struct. Since tm cannot represent milliseconds, none will be set.
     *
     * @param time_struct Struct to copy from
     */
    date_time(const tm& time_struct);

    /**
     * Construct date_time from given time_t. Converts the time to local timezone before setting it.
     * Note that no milliseconds will be set.
     *
     * @param time Timetype to set.
     */
    date_time(const time_t& time);

    /**
     * Construct date_time from MYSQL_TIME. No milliseconds will be set.
     *
     * @param time MYSQL_TIME to copy from.
     */
    date_time(const MYSQL_TIME& time);

    /**
     * Construct date_time from ISO yyyy-mm-dd hh:mm:ss.nnn date format. Throws an exception on
     * invalid input
     *
     * @param dt String containing ISO date
     */
    date_time(const std::string& dt);

    //
    // Operators
    //
    int compare(const date_time& dt) const;
    date_time& operator=(const date_time& dt);
    bool operator==(const date_time& dt) const;
    bool operator!=(const date_time& dt) const;
    bool operator<(const date_time& dt) const;
    bool operator<=(const date_time& dt) const;
    bool operator>(const date_time& dt) const;
    bool operator>=(const date_time& dt) const;

    /**
     * Get currently set year
     *
     * @return Current year
     */
    u16 year() const;

    /**
     * Set current year. If date becomes invalid, month and day will be reset to 1-1
     *
     * @param year Year to set
     * @return Newly set year
     */
    u16 year(u16 year);

    /**
     * Get currently set month
             *
             * @return Current month
     */
    u8 month() const;

    /**
     * Set current month. If date becomes invalid, day will be reset to 1
     *
     * @param month Month to set
     * @return Newly set month
     */
    u8 month(u8 month);

    /**
     * Get currently set day of month
             *
             * @return Current day
     */
    u8 day() const;

    /**
     * Set current day. If date becomes invalid, an exception will be thrown
     *
     * @param day Day to set
     * @return Newly set day
     */
    u8 day(u8 day);

    /**
     * Calculates the day of year from current date.
     *
     * @return Day of year
     */
    u16 day_of_year() const;

    /**
     * Sets the date by calculating which date the day_of_year corresponds to.
     *
     * @return Day of year that was set
     */
    u16 day_of_year(u16 day_of_year);

    /**
     * Set only date part. Invalid dates will throw an exception
     *
     * @param year Year to set.
     * @param month Month to set. [1-12]
     * @param day Day to set. [1-28/29/30/31]
     * @return True on success
     */
    bool set(u16 year, u8 month, u8 day);

    /**
             * Set date and time part. Invalid dates will throw an exception
             *
             * @param year Year to set.
             * @param month Month to set. [1-12]
             * @param day Day to set. [1-28/29/30/31]
     * @param hour The hour to set. [0-23}
             * @param minute The minute to set. [0-59]
             * @param second The second to set. [0-60]
             * @param millisecond The millisecond to set. [0-999]
             * @return True on success
             */
    bool set(u16 year, u8 month, u8 day, u8 hour, u8 minute, u8 second, u16 millisecond);

    /**
     * Set date and time to ISO yyyy-mm-dd hh:mm:ss.nnn date format.
     * Throws an exception on invalid input
     *
     * @param dt String containing ISO date
     * @return True on success
     */
    bool set(const std::string& dt) override;

    /**
     * Add years to current date.
     *
     * @param years Number of years to add
     * @return Newly created date_time containing result
     */
    date_time add_years(s32 years) const;

    /**
             * Add months to current date with year wrapping.
             *
             * @param months Number of months to add
             * @return Newly created date_time containing result
             */
    date_time add_months(s32 months) const;

    /**
             * Add days to current date with month wrapping.
             *
             * @param days Number of days to add
             * @return Newly created date_time containing result
             */
    date_time add_days(s32 days) const;

    /**
             * Add hours to current date with day wrapping.
             *
             * @param hours Number of hours to add
             * @return Newly created date_time containing result
             */
    date_time add_hours(s32 hours) const;

    /**
     * Add minutes to current date with hour wrapping.
             *
             * @param minutes Number of minutes to add
             * @return Newly created date_time containing result
     */
    date_time add_minutes(s32 minutes) const;

    /**
     * Add seconds to current date with minute wrapping.
             *
             * @param seconds Number of seconds to add
             * @return Newly created date_time containing result
     */
    date_time add_seconds(s32 seconds) const;

    /**
     * Add milliseconds to current date with second wrapping.
             *
             * @param milliseconds Number of milliseconds to add
             * @return Newly created date_time containing result
     */
    date_time add_milliseconds(s32 milliseconds) const;

    /**
     * Adds a timespan to the date_time.
     *
     * @param dur The duration to add
     * @return Newly created date_time containing result
     */
    date_time add(const time_span& dur) const;

    /**
     * Subtracts a timespan from the date_time.
     *
     * @param dur The duration to subtract
     * @return Newly created date_time containing result
     */
    date_time subtract(const time_span& dur) const;

    /**
     * Adds a mariadb::time to the date_time.
     *
     * @param t Time to add
     * @return Newly created date_time containing result
     */
    date_time add(const time& t) const;

    /**
     * Substract a mariadb::time from the date_time.
     *
     * @param t Time to subtract
     * @return Newly created date_time containing result
     */
    date_time substract(const time& t) const;

    /**
     * Calculates the time_span between this date_time and dt. If dt > this the time_span will be
     * negative
     *
     * @param dt Datetime to calculate the timespan to
     * @return Timespan representing the time between this and dt
     */
    time_span time_between(const date_time& dt) const;

    /**
     * Indicates whether a given year is leap according to gregorian calendar
     *
     * @param year The year to check
     * @return True if year is leap
     */
    static bool is_leap_year(u16 year);

    /**
     * Indicates whether a given date is valid in terms of existing month and day in month. Accounts
     * for leap years
     *
     * @param year Year to check
     * @param month Month to check
     * @param day Day to check
     * @return True if date is valid
     */
    static bool valid_date(u16 year, u8 month, u8 day);

    /**
     * Gets the number of days in a given year. Accounts for leap years
     *
     * @param year Year to check
     * @return Number of days in year
     */
    static u16 days_in_year(u16 year);

    /**
     * Gets the number of days in a given month of a given year. Accounts for leap years
     *
     * @param year Year to check
     * @param month Month to check
     * @return Number of days in month of year
     */
    static u8 days_in_month(u16 year, u8 month);

    /**
     * Gets the day of year for given date. Accounts for leap years
     *
     * @param year Year of date
     * @param month Month of date
     * @param day Day of date
     * @return Day of year of given date
     */
    static u16 day_of_year(u16 year, u8 month, u8 day);

    /**
     * Gets the date from given year and day of year
     *
     * @param year Year in which to position day_of_year
     * @param day_of_year Position of day within the year
     * @return Datetime representing the exact date
     */
    static date_time reverse_day_of_year(u16 year, u16 day_of_year);

    /**
     * Convert the date_time to a time_t. Precision is limited to seconds
     *
     * @return Timetype representing the date_time
     */
    time_t mktime() const;

    /**
     * Uses ::difftime to calculate number of seconds between two dates
     *
     * @param dt Datetime to calculate difference to
     * @return Number of seconds with fractions between the two dates
     */
    double diff_time(const date_time& dt) const;

    /**
     * Gets only the date part of this date_time
     *
     * @return Datetime representing only the date part
     */
    date_time date() const;

    /**
     * Converts the date_time to a MYSQL_TIME. Precision is limited to seconds
     *
     * @return MYSQL_TIME representing this date_time
     */
    MYSQL_TIME mysql_time() const;

    /**
     * Gets the current date and time as date_time. Does not set milliseconds
     *
     * @return Current date and time in local timezone
     */
    static date_time now();

    /**
     * Gets the current date and time as date_time. Does not set milliseconds
     *
     * @return Current date and time in UTC
     */
    static date_time now_utc();

    /**
     * Converts the date and time to ISO 8601 string yyyy-mm-dd hh:mm:ss[.nnn]
     *
     * @param with_millisecond Controls whether or not to print optional .nnn part
     * @return String containing ISO date and time
     */
    const std::string str(bool with_millisecond = false) const;

    /**
     * Converts only the date part of this date_time to ISO 8601 date string yyyy-mm-dd
     *
     * @return String containing ISO date
     */
    const std::string str_date() const;

   private:
    u16 m_year;
    u8 m_month;
    u8 m_day;
};

std::ostream& operator<<(std::ostream& os, const date_time& ddt);
}

#endif
