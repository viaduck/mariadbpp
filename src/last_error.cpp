//
//  M A R I A D B + +
//
//          Copyright Sylvain Rochette Langlois 2013,
//                    The ViaDuck Project 2016 - 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <mariadb++/last_error.hpp>

using namespace mariadb;

//
// Constructor
//
last_error::last_error() : m_last_error_no(0) {}

//
// Get last error
//
u32 last_error::error_no() const { return m_last_error_no; }

const std::string& last_error::error() const { return m_last_error; }
