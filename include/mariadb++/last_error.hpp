//
//  M A R I A D B + +
//
//	Author   : Sylvain Rochette Langlois
//	License  : Boost Software License (http://www.boost.org/users/license.html)
//

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
