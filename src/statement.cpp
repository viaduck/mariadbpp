//
//  M A R I A D B + +
//
//	Author   : Sylvain Rochette Langlois
//	License  : Boost Software License (http://www.boost.org/users/license.html)
//

#include <mysql/mysql.h>
#include <memory.h>
#include <mariadb++/connection.hpp>
#include <mariadb++/result_set.hpp>
#include <mariadb++/statement.hpp>
#include "bind.hpp"
#include "private.hpp"

using namespace mariadb;

#define STMT_ERROR_RETURN_RS(statement) \
    {                                   \
        STMT_ERROR_NO_BRAKET(statement) \
        return rs;                      \
    }

statement::statement(connection* conn, const std::string& query)
    : m_data(statement_data_ref(new statement_data(mysql_stmt_init(conn->m_mysql)))) {
    if (!m_data->m_statement)
        MYSQL_ERROR(conn->m_mysql)
    else if (mysql_stmt_prepare(m_data->m_statement, query.c_str(), query.size()))
        STMT_ERROR(m_data->m_statement)
    else {
        m_data->m_bind_count = mysql_stmt_param_count(m_data->m_statement);

        if (m_data->m_bind_count > 0) {
            m_data->m_binds = new bind[m_data->m_bind_count];
            m_data->m_my_binds = new MYSQL_BIND[m_data->m_bind_count];

            if (m_data->m_my_binds)
                memset(m_data->m_my_binds, '\0', sizeof(MYSQL_BIND) * m_data->m_bind_count);
        }
    }
}

void statement::set_connection(connection_ref& connection) { m_connection = connection; }

u64 statement::execute() {
    if (m_data->m_my_binds && mysql_stmt_bind_param(m_data->m_statement, m_data->m_my_binds))
        STMT_ERROR_RETURN_FALSE(m_data->m_statement);

    if (mysql_stmt_execute(m_data->m_statement)) STMT_ERROR_RETURN_FALSE(m_data->m_statement);

    return mysql_stmt_affected_rows(m_data->m_statement);
}

u64 statement::insert() {
    if (m_data->m_my_binds && mysql_stmt_bind_param(m_data->m_statement, m_data->m_my_binds))
        STMT_ERROR_RETURN_FALSE(m_data->m_statement);

    if (mysql_stmt_execute(m_data->m_statement)) STMT_ERROR_RETURN_FALSE(m_data->m_statement);

    return mysql_stmt_insert_id(m_data->m_statement);
}

result_set_ref statement::query() {
    result_set_ref rs;

    if (m_data->m_my_binds && mysql_stmt_bind_param(m_data->m_statement, m_data->m_my_binds))
        STMT_ERROR_RETURN_RS(m_data->m_statement);

    if (mysql_stmt_execute(m_data->m_statement)) STMT_ERROR_RETURN_RS(m_data->m_statement);

    rs.reset(new result_set(m_data));
    return rs;
}

MAKE_SETTER(blob, stream_ref)
    if (!value) return;

    value->seekg(0, std::ios_base::end);
    u64 size = value->tellg();
    value->seekg(0);

    bind.set_input(MYSQL_TYPE_BLOB, &mybind, nullptr, static_cast<long unsigned int>(size));

    value->read(bind.buffer(), bind.length());
}

MAKE_SETTER_2(data, const data_ref&, MYSQL_TYPE_BLOB)
    if (!value) return;

    bind.m_data = value;
    mybind.buffer = (void*)value->get();
    mybind.buffer_length = value->size();
}

MAKE_SETTER_2(date_time, const date_time&, MYSQL_TYPE_DATETIME)
    bind.m_time = value.mysql_time();
    mybind.buffer = (void*)&bind.m_time;
    mybind.buffer_length = sizeof(MYSQL_TIME);
}

MAKE_SETTER_2(date, const date_time&, MYSQL_TYPE_DATE)
    bind.m_time = value.date().mysql_time();
    mybind.buffer = (void*)&bind.m_time;
    mybind.buffer_length = sizeof(MYSQL_TIME);
}

MAKE_SETTER_2(time, const mariadb::time&, MYSQL_TYPE_TIME)
    bind.m_time = value.mysql_time();
    mybind.buffer = (void*)&bind.m_time;
    mybind.buffer_length = sizeof(MYSQL_TIME);
}

MAKE_SETTER(decimal, const decimal&)
    std::string str = value.str();

    bind.set_input(MYSQL_TYPE_STRING, &mybind, str.c_str(), str.size());
}

MAKE_SETTER(string, const std::string&)
    bind.set_input(MYSQL_TYPE_STRING, &mybind, value.c_str(), value.size());
}

MAKE_SETTER_2(boolean, bool, MYSQL_TYPE_TINY)
    bind.m_unsigned64 = value;
}

MAKE_SETTER_2(unsigned8, u8, MYSQL_TYPE_TINY)
    bind.m_unsigned64 = value;
    mybind.is_unsigned = true;
}

MAKE_SETTER_2(signed8, s8, MYSQL_TYPE_TINY)
    bind.m_signed64 = value;
}

MAKE_SETTER_2(unsigned16, u16, MYSQL_TYPE_SHORT)
    bind.m_unsigned64 = value;
    mybind.is_unsigned = true;
}

MAKE_SETTER_2(signed16, s16, MYSQL_TYPE_SHORT)
    bind.m_signed64 = value;
}

MAKE_SETTER_2(unsigned32, u32, MYSQL_TYPE_LONG)
    bind.m_unsigned64 = value;
    mybind.is_unsigned = true;
}

MAKE_SETTER_2(signed32, s32, MYSQL_TYPE_LONG)
    bind.m_signed64 = value;
}

MAKE_SETTER_2(unsigned64, u64, MYSQL_TYPE_LONGLONG)
    bind.m_unsigned64 = value;
    mybind.is_unsigned = true;
}

MAKE_SETTER_2(signed64, s64, MYSQL_TYPE_LONGLONG)
    bind.m_signed64 = value;
}

MAKE_SETTER_2(float, f32, MYSQL_TYPE_FLOAT)
    bind.m_float32[0] = value;
}

MAKE_SETTER_2(double, f64, MYSQL_TYPE_DOUBLE)
    bind.m_double64 = value;
}

void statement::set_null(u32 index) {
    MAKE_SETTER_BODY

    bind.set_input(MYSQL_TYPE_NULL, &mybind);
}
