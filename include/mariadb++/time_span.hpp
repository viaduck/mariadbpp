//
//  M A R I A D B + +
//
//          Copyright Sylvain Rochette Langlois 2013,
//                    The ViaDuck Project 2016 - 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef _MARIADB_TIME_SPAN_HPP_
#define _MARIADB_TIME_SPAN_HPP_

#include <sstream>
#include <mariadb++/types.hpp>

namespace mariadb {
class time_span {
   public:
    /**
     * Construct time_span from given data. Note that a time_span can span one day at most.
     *
     * @param hours Number of hours 0-23
     * @param minutes Number of minutes 0-59
     * @param seconds Number of seconds 0-59
     * @param milliseconds Number of milliseconds 0-999
     * @param negative Indicates negative timespan
     */
    time_span(u32 days = 0, u8 hours = 0, u8 minutes = 0, u8 seconds = 0, u16 milliseconds = 0,
              bool negative = false);

    /**
     * Copy constructor
     *
     * @param dur Timespan to copy from
     */
    time_span(const time_span& dur);

    /**
     * Compares this timespan with the given timespan.
     *
     * @param rhs Timespan to compare this to
     * @return 1 if this is greater, 0 if equal, -1 if this is smaller
     */
    int compare(const time_span& rhs) const;

    time_span& operator=(const time_span& rhs);
    bool operator==(const time_span& rhs) const;
    bool operator!=(const time_span& rhs) const;
    bool operator<(const time_span& rhs) const;
    bool operator<=(const time_span& rhs) const;
    bool operator>(const time_span& rhs) const;
    bool operator>=(const time_span& rhs) const;

    /**
     * Set the value of this time_span to given values
     *
     * @param days Any number of days
     * @param hours Number of hours 0-23
     * @param minutes Number of minutes 0-59
     * @param seconds Number of seconds 0-59
     * @param milliseconds Number of milliseconds 0-999
     * @param negative Indicates negative timespan
     */
    void set(u32 days = 0, u8 hours = 0, u8 minutes = 0, u8 seconds = 0, u16 milliseconds = 0,
             bool negative = false);

    /**
     * Indicates whether this time_span is zero. That is only true if all components are zero
     *
     * @return True if zero
     */
    bool zero() const;

    /**
     * Indicates whether this time_span is negative.
     *
     * @return True if negative
     */
    bool negative() const;

    /**
     * Sets negative flag on this time_span
     *
     * @param negative Value to set
     * @return Newly set value
     */
    bool negative(bool negative);

    /**
     * Get number of days
     *
     * @return Number of days
     */
    u32 days() const;

    /**
     * Sets the number of days
     *
     * @param hour Number of days to set
     * @return Newly set value
     */
    u32 days(u32 day);

    /**
     * Get number of hours
     *
     * @return Number of hours
     */
    u8 hours() const;

    /**
     * Sets the number of hours
     *
     * @param hour Number of hours to set
     * @return Newly set value
     */
    u8 hours(u8 hour);

    /**
     * Get number of hours
     *
     * @return Number of hours
     */
    u8 minutes() const;

    /**
     * Sets the number of minutes
     *
     * @param minute Number of minutes to set
     * @return Newly set value
     */
    u8 minutes(u8 minute);

    /**
     * Get number of seconds
     *
     * @return Number of seconds
     */
    u8 seconds() const;

    /**
     * Sets the number of seconds
     *
     * @param second Number of seconds to set
     * @return Newly set value
     */
    u8 seconds(u8 second);

    /**
     * Get number of milliseconds
     *
     * @return Number of milliseconds
     */
    u16 milliseconds() const;

    /**
     * Sets the number of milliseconds
     *
     * @param millisecond Number of milliseconds to set
     * @return Newly set value
     */
    u16 milliseconds(u16 millisecond);

    /**
     * Converts the time_span to minutes
     *
     * @return Total number of minutes in this time_span
     */
    u64 total_hours() const;

    /**
     * Converts the time_span to minutes
     *
     * @return Total number of minutes in this time_span
     */
    u64 total_minutes() const;

    /**
     * Converts the time_span to seconds
     *
     * @return Total number of seconds in this time_span
     */
    u64 total_seconds() const;

    /**
     * Converts the time_span to milliseconds
     *
     * @return Total number of milliseconds in this time_span
     */
    u64 total_milliseconds() const;

   private:
    bool m_negative = false;
    u32 m_days = 0;
    u8 m_hours = 0;
    u8 m_minutes = 0;
    u8 m_seconds = 0;
    u16 m_milliseconds = 0;
};

std::ostream& operator<<(std::ostream& os, const time_span& ts);
}

#endif
