//
//  M A R I A D B + +
//
//          Copyright Sylvain Rochette Langlois 2013,
//                    Frantisek Boranek 2015,
//                    The ViaDuck Project 2016 - 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <sstream>
#include <mariadb++/exceptions.hpp>

using namespace mariadb;

//
// Constructors
//
exception::date_time::date_time(u16 year, u8 month, u8 day, u8 hour, u8 minute, u8 second,
                                u16 millisecond) throw()
    : base() {
    std::ostringstream oss;
    oss << "Invalid date time: year - " << year << ", month - " << month << ", day - " << day
        << ", hour - " << hour << ", minute - " << minute << ", second - " << second
        << ", millisecond - " << millisecond;

    m_error = oss.str();
}

exception::time::time(u8 hour, u8 minute, u8 second, u16 millisecond) throw() : base() {
    std::ostringstream oss;
    oss << "Invalid time: hour - " << hour << ", minute - " << minute << ", second - " << second
        << ", millisecond - " << millisecond;

    m_error = oss.str();
}
