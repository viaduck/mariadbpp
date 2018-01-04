//
//  M A R I A D B + +
//
//	Author   : Piotr Likus (https://github.com/vpiotr/decimal_for_cpp)
//	Adapted  : Sylvain Rochette Langlois
//	License  : Boost Software License (http://www.boost.org/users/license.html)
//

#ifndef _MARIADB_DECIMAL_HPP_
#define _MARIADB_DECIMAL_HPP_

#include <string>
#include <mariadb++/types.hpp>
#include <mariadb++/conversion_helper.hpp>

namespace mariadb {
class decimal {
public:
    explicit decimal(std::string str = "") : mStr(std::move(str)) { }

    std::string str() const {
        return mStr;
    }

    f32 float32() const {
        return string_cast<f32>(mStr);
    }

    f64 double64() const {
        return string_cast<f64>(mStr);
    }

private:
        std::string mStr;
};
}

#endif
