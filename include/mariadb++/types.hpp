//
//  M A R I A D B + +
//
//          Copyright Sylvain Rochette Langlois 2013,
//                    The ViaDuck Project 2016 - 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef _MARIADB_TYPES_HPP_
#define _MARIADB_TYPES_HPP_

#include <mysql.h>
#include <memory>

namespace mariadb {
    //
    // Default types
    //
    typedef unsigned char u8;
    typedef unsigned short u16;
    typedef unsigned int u32;
    typedef signed char s8;
    typedef signed short s16;
    typedef signed int s32;
    typedef float f32;
    typedef double f64;
    typedef long double f128;

    #if defined(_MSC_VER) || defined(__BORLANDC__)
    typedef unsigned __int64 u64;
    typedef signed __int64 s64;
    #else
    typedef unsigned long long u64;
    typedef signed long long s64;
    #endif

    typedef u64 handle;

    //
    // Value type
    //
    namespace value {
        enum type {
            null = 0,
            blob,
            data,
            date,
            date_time,
            time,
            string,
            boolean,
            decimal,
            unsigned8,
            signed8,
            unsigned16,
            signed16,
            unsigned32,
            signed32,
            unsigned64,
            signed64,
            float32,
            double64,
            enumeration
        };
    }

    //
    // Isolation level
    //
    namespace isolation {
        enum level { repeatable_read = 0, read_committed, read_uncommitted, serializable };
    }

    //
    // Stream
    //
    typedef std::shared_ptr<std::istream> stream_ref;
}

#if !defined(MARIADB_BASE_VERSION) && MYSQL_VERSION_ID >  80000
// Assume MySQL Community 8.0+
typedef bool my_bool;
#endif

#endif
