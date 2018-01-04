//
//  M A R I A D B + +
//
//	Author   : Sylvain Rochette Langlois
//	License  : Boost Software License (http://www.boost.org/users/license.html)
//

#ifndef _MARIADB_BIND_HPP_
#define _MARIADB_BIND_HPP_

#include <mysql/mysql.h>
#include <mariadb++/types.hpp>
#include <mariadb++/data.hpp>

namespace mariadb {
class statement;
class result_set;
class bind {
    friend class statement;
    friend class result_set;

   public:
    /**
     * Construct a parameter bind
     */
    explicit bind(MYSQL_BIND *mysql_bind);

    /**
     * Construct a result bind for given field type
     */
    bind(MYSQL_BIND *mysql_bind, MYSQL_FIELD *mysql_field);

    char* buffer() const;

    long unsigned int length() const;

    bool is_null() const;

    void set(enum_field_types type, const char* buffer = nullptr, unsigned long length = 0, bool us = false);

   private:
    MYSQL_BIND* m_bind;
    MYSQL_TIME m_time;

    my_bool m_is_null;
    my_bool m_error;

    data_ref m_data;

    union {
        u64 m_unsigned64;
        s64 m_signed64;
        s32 m_signed32[2];
        f64 m_double64;
        f32 m_float32[2];
    };
};
}

#endif
