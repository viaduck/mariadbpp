//
//  M A R I A D B + +
//
//          Copyright Sylvain Rochette Langlois 2013,
//                    Frantisek Boranek 2015,
//                    The ViaDuck Project 2016 - 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <mysql.h>
#include <memory.h>
#include <mariadb++/bind.hpp>

using namespace mariadb;

bind::bind(MYSQL_BIND* b) : m_bind(b), m_is_null(0), m_error(0) {
    // clear bind
    memset(b, 0, sizeof(MYSQL_BIND));

    // set default data
    m_bind->buffer = &m_unsigned64;
    m_bind->buffer_length = 0;
    m_bind->buffer_type = MYSQL_TYPE_NULL;
    m_bind->is_null = &m_is_null;
    m_bind->error = &m_error;
    m_bind->length = &m_bind->buffer_length;
}

bind::bind(MYSQL_BIND* b, MYSQL_FIELD* f) : bind(b) {
    set(f->type, nullptr, f->max_length, (f->flags & UNSIGNED_FLAG) == UNSIGNED_FLAG);
}

char* bind::buffer() const {
    if (m_data) return m_data->get();

    return const_cast<char*>(reinterpret_cast<const char*>(&m_unsigned64));
}

unsigned long bind::length() const { return m_bind->buffer_length; }

bool bind::is_null() const { return (m_is_null != 0); }

void bind::set(enum_field_types type, const char* buffer, unsigned long length, bool us) {
    m_bind->buffer_type = type;
    m_bind->is_unsigned = us ? 1 : 0;

    switch (type) {
        case MYSQL_TYPE_NULL:
            m_bind->buffer_length = 1;
            break;

        case MYSQL_TYPE_TINY:
        case MYSQL_TYPE_BIT:
            m_bind->buffer_length = 1;
            break;

        case MYSQL_TYPE_YEAR:
        case MYSQL_TYPE_SHORT:
            m_bind->buffer_length = sizeof(s16);
            break;

        case MYSQL_TYPE_INT24:
        case MYSQL_TYPE_LONG:
            m_bind->buffer_length = sizeof(s32);
            break;

        case MYSQL_TYPE_LONGLONG:
            m_bind->buffer_length = sizeof(s64);
            break;

        case MYSQL_TYPE_FLOAT:
            m_bind->buffer_length = sizeof(f32);
            break;

        case MYSQL_TYPE_DOUBLE:
            m_bind->buffer_length = sizeof(f64);
            break;

        case MYSQL_TYPE_NEWDATE:
        case MYSQL_TYPE_DATE:
        case MYSQL_TYPE_TIME:
        case MYSQL_TYPE_TIMESTAMP:
        case MYSQL_TYPE_DATETIME:
            m_bind->buffer = &m_time;
            m_bind->buffer_length = sizeof(MYSQL_TIME);
            break;

        default:
        case MYSQL_TYPE_DECIMAL:
        case MYSQL_TYPE_NEWDECIMAL:
        case MYSQL_TYPE_TINY_BLOB:
        case MYSQL_TYPE_MEDIUM_BLOB:
        case MYSQL_TYPE_LONG_BLOB:
        case MYSQL_TYPE_BLOB:
        case MYSQL_TYPE_ENUM:
        case MYSQL_TYPE_VARCHAR:
        case MYSQL_TYPE_VAR_STRING:
        case MYSQL_TYPE_STRING:
            m_data = data_ref(new data<char>(length));
            m_bind->buffer = m_data->get();
            m_bind->buffer_length = m_data->size();

            if (buffer) memcpy(m_bind->buffer, buffer, length);
            break;
    }
}