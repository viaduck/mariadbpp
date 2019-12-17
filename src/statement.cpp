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
#include <mariadb++/statement.hpp>
#include <mariadb++/bind.hpp>
#include "private.hpp"

using namespace mariadb;

#define STMT_ERROR_RETURN_RS(statement) \
    {                                   \
        STMT_ERROR_NO_BRAKET(statement) \
    }

statement::statement(connection* conn, const std::string& query)
    : m_parent(conn), m_data(statement_data_ref(new statement_data(mysql_stmt_init(conn->m_mysql)))) {
    if (!m_data->m_statement)
        MYSQL_ERROR(conn->m_mysql)
    else if (mysql_stmt_prepare(m_data->m_statement, query.c_str(), query.size()))
        STMT_ERROR(m_data->m_statement)
    else {
        m_data->m_bind_count = mysql_stmt_param_count(m_data->m_statement);

        if (m_data->m_bind_count > 0) {
            m_data->m_raw_binds = new MYSQL_BIND[m_data->m_bind_count];

            for (uint32_t i = 0; i < m_data->m_bind_count; i++)
                m_data->m_binds.emplace_back(new bind(&m_data->m_raw_binds[i]));
        }
    }
}

void statement::set_connection(connection_ref& connection) { m_connection = connection; }

u64 statement::execute() {
    if (m_data->m_raw_binds && mysql_stmt_bind_param(m_data->m_statement, m_data->m_raw_binds))
        STMT_ERROR(m_data->m_statement);

    if (mysql_stmt_execute(m_data->m_statement)) STMT_ERROR(m_data->m_statement);

    return mysql_stmt_affected_rows(m_data->m_statement);
}

u64 statement::insert() {
    if (m_data->m_raw_binds && mysql_stmt_bind_param(m_data->m_statement, m_data->m_raw_binds))
        STMT_ERROR(m_data->m_statement);

    if (mysql_stmt_execute(m_data->m_statement)) STMT_ERROR(m_data->m_statement);

    return mysql_stmt_insert_id(m_data->m_statement);
}

result_set_ref statement::query() {
    result_set_ref rs;

    if (m_data->m_raw_binds && mysql_stmt_bind_param(m_data->m_statement, m_data->m_raw_binds))
        STMT_ERROR_RETURN_RS(m_data->m_statement);

    if (mysql_stmt_execute(m_data->m_statement)) STMT_ERROR_RETURN_RS(m_data->m_statement);

    rs.reset(new result_set(m_parent, m_data));
    return rs;
}

MAKE_SETTER(blob, stream_ref)
    if (!value) return;

    value->seekg(0, std::ios_base::end);
    u64 size = value->tellg();
    value->seekg(0);

    // allocate empty buffer
    bind.set(MYSQL_TYPE_BLOB, nullptr, static_cast<long unsigned int>(size));
    // copy stream over
    value->read(bind.buffer(), bind.length());
}

MAKE_SETTER(data, const data_ref&)
    if (!value) return;
    bind.set(MYSQL_TYPE_BLOB, value->get(), value->size());
}

MAKE_SETTER(date_time, const date_time&)
    bind.m_time = value.mysql_time();
    bind.set(MYSQL_TYPE_DATETIME);
}

MAKE_SETTER(date, const date_time&)
    bind.m_time = value.date().mysql_time();
    bind.set(MYSQL_TYPE_DATE);
}

MAKE_SETTER(time, const mariadb::time&)
    bind.m_time = value.mysql_time();
    bind.set(MYSQL_TYPE_TIME);
}

MAKE_SETTER(decimal, const decimal&)
    std::string str = value.str();
    bind.set(MYSQL_TYPE_STRING, str.c_str(), str.size());
}

MAKE_SETTER(string, const std::string&)
    bind.set(MYSQL_TYPE_STRING, value.c_str(), value.size());
}

MAKE_SETTER(boolean, bool)
    bind.m_unsigned64 = value;
    bind.set(MYSQL_TYPE_TINY);
}

MAKE_SETTER(unsigned8, u8)
    bind.m_unsigned64 = value;
    bind.set(MYSQL_TYPE_TINY, nullptr, 0, true);
}

MAKE_SETTER(signed8, s8)
    bind.m_signed64 = value;
    bind.set(MYSQL_TYPE_TINY);
}

MAKE_SETTER(unsigned16, u16)
    bind.m_unsigned64 = value;
    bind.set(MYSQL_TYPE_SHORT, nullptr, 0, true);
}

MAKE_SETTER(signed16, s16)
    bind.m_signed64 = value;
    bind.set(MYSQL_TYPE_SHORT);
}

MAKE_SETTER(unsigned32, u32)
    bind.m_unsigned64 = value;
    bind.set(MYSQL_TYPE_LONG, nullptr, 0, true);
}

MAKE_SETTER(signed32, s32)
    bind.m_signed64 = value;
    bind.set(MYSQL_TYPE_LONG);
}

MAKE_SETTER(unsigned64, u64)
    bind.m_unsigned64 = value;
    bind.set(MYSQL_TYPE_LONGLONG, nullptr, 0, true);
}

MAKE_SETTER(signed64, s64)
    bind.m_signed64 = value;
    bind.set(MYSQL_TYPE_LONGLONG);
}

MAKE_SETTER(float, f32)
    bind.m_float32[0] = value;
    bind.set(MYSQL_TYPE_FLOAT);
}

MAKE_SETTER(double, f64)
    bind.m_double64 = value;
    bind.set(MYSQL_TYPE_DOUBLE);
}

void statement::set_null(u32 index) {
    MAKE_SETTER_BODY
    bind.set(MYSQL_TYPE_NULL);
}
