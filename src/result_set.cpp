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
#include <mariadb++/connection.hpp>
#include <mariadb++/result_set.hpp>
#include <mariadb++/conversion_helper.hpp>
#include <mariadb++/bind.hpp>
#include "private.hpp"

using namespace mariadb;

result_set::result_set(connection *conn)
    : m_result_set((conn->account()->store_result() ? mysql_store_result : mysql_use_result)(conn->m_mysql)),
      m_fields(nullptr),
      m_row(nullptr),
      m_raw_binds(nullptr),
      m_stmt_data(nullptr),
      m_lengths(nullptr),
      m_field_count(0),
      m_was_fetched(false) {

    if (m_result_set) {
        m_field_count = mysql_num_fields(m_result_set);
        m_fields = mysql_fetch_fields(m_result_set);

        for (u32 i = 0; i < m_field_count; ++i) m_indexes[m_fields[i].name] = i;
    }
}

result_set::result_set(connection* conn, const statement_data_ref &stmt_data)
    : m_result_set(nullptr),
      m_fields(nullptr),
      m_row(nullptr),
      m_raw_binds(nullptr),
      m_stmt_data(stmt_data),
      m_lengths(nullptr),
      m_field_count(0),
      m_was_fetched(false) {

    int max_length = 1;
    mysql_stmt_attr_set(stmt_data->m_statement, STMT_ATTR_UPDATE_MAX_LENGTH, &max_length);

    if (conn->account()->store_result() && mysql_stmt_store_result(stmt_data->m_statement))
        STMT_ERROR(stmt_data->m_statement)
    else {
        m_field_count = mysql_stmt_field_count(stmt_data->m_statement);
        m_result_set = mysql_stmt_result_metadata(stmt_data->m_statement);

        if (m_field_count > 0) {
            m_fields = mysql_fetch_fields(m_result_set);
            m_raw_binds = new MYSQL_BIND[m_field_count];
            m_row = new char *[m_field_count];

            for (u32 i = 0; i < m_field_count; ++i) {
                m_indexes[m_fields[i].name] = i;
                m_binds.emplace_back(new bind(&m_raw_binds[i], &m_fields[i]));
                m_row[i] = m_binds[i]->buffer();
            }

            mysql_stmt_bind_result(stmt_data->m_statement, m_raw_binds);
        }
    }
}

statement_data::~statement_data() {
    delete[] m_raw_binds;

    if (m_statement) mysql_stmt_close(m_statement);
}

result_set::~result_set() {
    if (m_result_set) mysql_free_result(m_result_set);

    delete[] m_raw_binds;

    if (m_stmt_data) {
        delete[] m_row;

        mysql_stmt_free_result(m_stmt_data->m_statement);
    }
}

u32 result_set::column_count() const { return m_field_count; }

value::type result_set::column_type(u32 index) const {
    if (index >= m_field_count) throw std::out_of_range("Column index out of range");

    bool is_unsigned = (m_fields[index].flags & UNSIGNED_FLAG) == UNSIGNED_FLAG;

    switch (m_fields[index].type) {
        case MYSQL_TYPE_NULL:
            return value::null;

        case MYSQL_TYPE_BIT:
            return value::boolean;

        case MYSQL_TYPE_FLOAT:
            return value::float32;

        case MYSQL_TYPE_DECIMAL:
        case MYSQL_TYPE_NEWDECIMAL:
            return value::decimal;

        case MYSQL_TYPE_DOUBLE:
            return value::double64;

        case MYSQL_TYPE_NEWDATE:
        case MYSQL_TYPE_DATE:
            return value::date;

        case MYSQL_TYPE_TIME:
            return value::time;

        case MYSQL_TYPE_TIMESTAMP:
        case MYSQL_TYPE_DATETIME:
            return value::date_time;

        case MYSQL_TYPE_TINY:
            return (is_unsigned ? value::unsigned8 : value::signed8);

        case MYSQL_TYPE_YEAR:
        case MYSQL_TYPE_SHORT:
            return (is_unsigned ? value::unsigned16 : value::signed16);

        case MYSQL_TYPE_INT24:
        case MYSQL_TYPE_LONG:
            return (is_unsigned ? value::unsigned32 : value::signed32);

        case MYSQL_TYPE_LONGLONG:
            return (is_unsigned ? value::unsigned64 : value::signed64);

        case MYSQL_TYPE_TINY_BLOB:
        case MYSQL_TYPE_MEDIUM_BLOB:
        case MYSQL_TYPE_LONG_BLOB:
        case MYSQL_TYPE_BLOB:
            return value::blob;

        case MYSQL_TYPE_ENUM:
            return value::enumeration;

        default:
        case MYSQL_TYPE_VARCHAR:
        case MYSQL_TYPE_VAR_STRING:
        case MYSQL_TYPE_STRING:
            return value::string;
    }
}

const std::string result_set::column_name(u32 index) {
    if (index >= m_field_count) throw std::out_of_range("Column index out of range");

    return m_fields[index].name;
}

u32 result_set::column_index(const std::string &name) const {
    const map_indexes_t::const_iterator i = m_indexes.find(name);

    if (i == m_indexes.end()) return 0xffffffff;

    return i->second;
}

unsigned long result_set::column_size(u32 index) const {
    if (index >= m_field_count) throw std::out_of_range("Column index out of range");

    return m_stmt_data ? m_binds.at(index)->length() : m_lengths[index];
}

bool result_set::set_row_index(u64 index) {
    if (m_stmt_data)
        mysql_stmt_data_seek(m_stmt_data->m_statement, index);
    else
        mysql_data_seek(m_result_set, index);
    return next();
}

bool result_set::next() {
    if (!m_result_set) return (m_was_fetched = false);

    if (m_stmt_data) return (m_was_fetched = !mysql_stmt_fetch(m_stmt_data->m_statement));

    m_row = mysql_fetch_row(m_result_set);
    m_lengths = mysql_fetch_lengths(m_result_set);

    // make sure no access to results is possible until a result is successfully fetched
    return (m_was_fetched = m_row != nullptr);
}

u64 result_set::row_index() const {
    if (m_stmt_data) return reinterpret_cast<u64>(mysql_stmt_row_tell(m_stmt_data->m_statement));

    return reinterpret_cast<u64>(mysql_row_tell(m_result_set));
}

u64 result_set::row_count() const {
    if (m_stmt_data) return mysql_stmt_num_rows(m_stmt_data->m_statement);

    return mysql_num_rows(m_result_set);
}

void result_set::check_row_fetched() const {
    if (!m_was_fetched) throw std::out_of_range("No row was fetched");
}

void result_set::check_type(u32 index, value::type requested) const {
    value::type actual = column_type(index);
    bool type_error;

    // check requested type vs actual type.
    switch (requested) {
        // these types require exact type match
        case value::type::float32:
        case value::type::double64:
        case value::type::decimal:

        case value::type::time:
        case value::type::date_time:
        case value::type::date:

        case value::type::enumeration:
            type_error = actual != requested;
            break;

        // these types require size match
        case value::type::unsigned8:
        case value::type::signed8:
            type_error = actual != value::type::signed8 && actual != value::type::unsigned8;
            break;
        case value::type::unsigned16:
        case value::type::signed16:
            type_error = actual != value::type::signed16 && actual != value::type::unsigned16;
            break;
        case value::type::unsigned32:
        case value::type::signed32:
            type_error = actual != value::type::signed32 && actual != value::type::unsigned32;
            break;
        case value::type::unsigned64:
        case value::type::signed64:
            type_error = actual != value::type::signed64 && actual != value::type::unsigned64;
            break;

        // bool can also be a signed8
        case value::type::boolean:
            type_error = actual != requested && actual != value::type::signed8;
            break;

        // only string, blob, data and null are interchangeable
        case value::type::string:
        case value::type::blob:
        case value::type::data:
            type_error = actual != value::type::string &&
                         actual != value::type::blob &&
                         actual != value::type::data &&
                         actual != value::type::null;
            break;

        default:
            type_error = false;
    }

    if (type_error) {
        printf("requested type %d does not match actual type %d", +requested, +actual);
        MARIADB_ERROR_THROW_CONNECTION(12, "type error");
    }
}

MAKE_GETTER(blob, stream_ref, value::type::blob)
    size_t len = column_size(index);

    if (len == 0) return stream_ref();

    auto *ss = new std::istringstream();
    ss->rdbuf()->pubsetbuf(m_row[index], len);
    return stream_ref(ss);
}

MAKE_GETTER(data, data_ref, value::type::data)
    size_t len = column_size(index);

    return len == 0 ? data_ref() : data_ref(new data<char>(m_row[index], len));
}

MAKE_GETTER(string, std::string, value::type::string)
    return std::string(m_row[index], column_size(index));
}

MAKE_GETTER(date, date_time, value::type::date)
    if (m_stmt_data) return mariadb::date_time(m_binds[index]->m_time);

    return date_time(std::string(m_row[index], column_size(index))).date();
}

MAKE_GETTER(date_time, date_time, value::type::date_time)
    if (m_stmt_data) return mariadb::date_time(m_binds[index]->m_time);

    return date_time(std::string(m_row[index], column_size(index)));
}

MAKE_GETTER(time, mariadb::time, value::type::time)
    if (m_stmt_data) return mariadb::time(m_binds[index]->m_time);

    return mariadb::time(std::string(m_row[index], column_size(index)));
}

MAKE_GETTER(decimal, decimal, value::type::decimal)
    return decimal(std::string(m_row[index], column_size(index)));
}

MAKE_GETTER(boolean, bool, value::type::boolean)
    if (m_stmt_data) return (m_binds[index]->m_uchar8[0] != 0);

    return string_cast<bool>(std::string(m_row[index], column_size(index)));
}

MAKE_GETTER(unsigned8, u8, value::type::unsigned8)
    if (m_stmt_data) return checked_cast<u8>(0x00000000000000ff & m_binds[index]->m_unsigned64);

    return string_cast<u8>(std::string(m_row[index], column_size(index)));
}

MAKE_GETTER(signed8, s8, value::type::signed8)
    if (m_stmt_data) return checked_cast<s8>(0x00000000000000ff & m_binds[index]->m_signed64);

    return string_cast<s8>(std::string(m_row[index], column_size(index)));
}

MAKE_GETTER(unsigned16, u16, value::type::unsigned16)
    if (m_stmt_data) return checked_cast<u16>(0x000000000000ffff & m_binds[index]->m_unsigned64);

    return string_cast<u16>(std::string(m_row[index], column_size(index)));
}

MAKE_GETTER(signed16, s16, value::type::signed16)
    if (m_stmt_data) return checked_cast<s16>(0x000000000000ffff & m_binds[index]->m_signed64);

    return string_cast<s16>(std::string(m_row[index], column_size(index)));
}

MAKE_GETTER(unsigned32, u32, value::type::unsigned32)
    if (m_stmt_data) return checked_cast<u32>(0x00000000ffffffff & m_binds[index]->m_unsigned64);

    return string_cast<u32>(std::string(m_row[index], column_size(index)));
}

MAKE_GETTER(signed32, s32, value::type::signed32)
    if (m_stmt_data) return m_binds[index]->m_signed32[0];

    return string_cast<s32>(std::string(m_row[index], column_size(index)));
}

MAKE_GETTER(unsigned64, u64, value::type::unsigned64)
    if (m_stmt_data) return m_binds[index]->m_unsigned64;

    return string_cast<u64>(std::string(m_row[index], column_size(index)));
}

MAKE_GETTER(signed64, s64, value::type::signed64)
    if (m_stmt_data) return m_binds[index]->m_signed64;

    return string_cast<s64>(std::string(m_row[index], column_size(index)));
}

MAKE_GETTER(float, f32, value::type::float32)
    if (m_stmt_data) return m_binds[index]->m_float32[0];

    return string_cast<f32>(std::string(m_row[index], column_size(index)));
}

MAKE_GETTER(double, f64, value::type::double64)
    if (m_stmt_data) return checked_cast<f64>(m_binds[index]->m_double64);

    return string_cast<f64>(std::string(m_row[index], column_size(index)));
}

MAKE_GETTER(is_null, bool, value::type::null)
    if (m_stmt_data) return m_binds[index]->is_null();

    return !m_row[index];
}
