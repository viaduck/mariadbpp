//
//  M A R I A D B + +
//
//          Copyright Sylvain Rochette Langlois 2013,
//                    Frantisek Boranek 2015,
//                    The ViaDuck Project 2016 - 2021.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef _MARIADB_PRIVATE_HPP_
#define _MARIADB_PRIVATE_HPP_

#include <mariadb++/exceptions.hpp>
#include <ctime>

namespace mariadb {
#if _WIN32
inline int localtime_safe(struct tm *_tm, const time_t *_time) {
    return localtime_s(_tm, _time);
}

inline int gmtime_safe(struct tm *_tm, const time_t *_time) {
    return gmtime_s(_tm, _time);
}
#else
inline int localtime_safe(struct tm *_tm, const time_t *_time) {
    return localtime_r(_time, _tm) ? 0 : -1;
}

inline int gmtime_safe(struct tm *_tm, const time_t *_time) {
    return gmtime_r(_time, _tm) ? 0 : -1;
}
#endif
}  // namespace mariadb
#if _WIN32

#if __MINGW32__
// this is needed because on mingw sprintf_s is broken (due to msvcrt not following c99 standard
// with sprintf...)
#define snprintf(buffer, szbuffer, format, ...) __mingw_sprintf(buffer, format, ##__VA_ARGS__)
#else
#define snprintf sprintf_s
#endif

#endif

inline int mysql_option_safe(MYSQL *mysql, const mysql_option option, const char *value) {
#if MARIADB_HAS_OPTIONS_V
    return mysql_optionsv(mysql, option, value);
#else
    return mysql_options(mysql, option, value);
#endif
}

#define MARIADB_THROW(error, ...) throw error(__VA_ARGS__)
#define MARIADB_THROW_IF(x, error, ...)        \
    do {                                       \
        if ((x)) {                             \
            MARIADB_THROW(error, __VA_ARGS__); \
        }                                      \
    } while (0)

#define MARIADB_ERROR_QUIET(error, error_id, error_desc) MARIADB_THROW(error, error_id, error_desc)
#define MARIADB_ERROR_VERBOSE(error, error_id, error_desc)                                                        \
    do {                                                                                                          \
        std::cerr << "MariaDB Error(" << (error_id) << "): " << (error_desc) << "\nIn function: " << __FUNCTION__ \
                  << "\nIn file " << __FILE__ << "\nOn line " << __LINE__ << '\n';                                \
        MARIADB_ERROR_QUIET(error, error_id, error_desc);                                                         \
    } while (0)

#if MARIADB_QUIET
#define MARIADB_ERROR MARIADB_ERROR_QUIET
#else
#define MARIADB_ERROR MARIADB_ERROR_VERBOSE
#endif

#define MARIADB_CONN_ERROR(conn)                                             \
    do {                                                                     \
        m_last_error_no = mysql_errno(conn);                                 \
        m_last_error = mysql_error(conn);                                    \
        MARIADB_ERROR(exception::connection, m_last_error_no, m_last_error); \
    } while (0)
#define MARIADB_CONN_CLOSE_ERROR(conn)                                       \
    do {                                                                     \
        m_last_error_no = mysql_errno(conn);                                 \
        m_last_error = mysql_error(conn);                                    \
        disconnect();                                                        \
        MARIADB_ERROR(exception::connection, m_last_error_no, m_last_error); \
    } while (0)

#define MARIADB_STMT_ERROR(stmt)                                            \
    do {                                                                    \
        m_last_error_no = mysql_stmt_errno(stmt);                           \
        m_last_error = mysql_stmt_error(stmt);                              \
        MARIADB_ERROR(exception::statement, m_last_error_no, m_last_error); \
    } while (0)

#endif
