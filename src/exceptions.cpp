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
#include <iomanip>
#include <mariadb++/exceptions.hpp>

using namespace mariadb;

//
// Constructors
//
exception::date_time::date_time(u16 year, u8 month, u8 day, u8 hour, u8 minute, u8 second,
                                u16 millisecond) throw() : base() {
    std::ostringstream oss;
    oss << "Invalid date time: " << std::setfill('0') << std::setw(4) << year
        << "-" << std::setw(2) << int(month)
        << "-" << std::setw(2) << int(day)
        << "T" << std::setw(2) << int(hour)
        << ":" << std::setw(2) << int(minute)
        << ":" << std::setw(2) << int(second)
        << "." << std::setw(3) << millisecond;

    m_error = oss.str();
}

exception::time::time(u8 hour, u8 minute, u8 second, u16 millisecond) throw() : base() {
    std::ostringstream oss;
    oss << "Invalid time: " << std::setfill('0') << std::setw(2) << int(hour)
            << ":" << std::setw(2) << int(minute)
            << ":" << std::setw(2) << int(second)
            << "." << std::setw(3) << millisecond;

    m_error = oss.str();
}
