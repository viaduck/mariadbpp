//
//  M A R I A D B + +
//
//          Copyright Sylvain Rochette Langlois 2013,
//                    The ViaDuck Project 2016 - 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef _MARIADB_LAST_ERROR_HPP_
#define _MARIADB_LAST_ERROR_HPP_

#include <string>
#include "types.hpp"

namespace mariadb {
class last_error {
   public:
    //
    // Constructor
    //
    last_error();

    //
    // Get last error
    //
    u32 error_no() const;
    const std::string& error() const;

   protected:
    u32 m_last_error_no;
    std::string m_last_error;
};
}

#endif
