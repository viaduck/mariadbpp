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

#define STMT_ERROR_RETURN_RS(statement)\
{\
	STMT_ERROR_NO_BRAKET(statement)\
	return rs;\
}

//
// Constructor
//
statement::statement(connection* connection, const std::string &query) :
	m_statement(mysql_stmt_init(connection->m_mysql)),
	m_my_binds(NULL),
	m_binds(NULL),
	m_bind_count(0)
{
	if (!m_statement)
		MYSQL_ERROR(connection->m_mysql)
	else if (mysql_stmt_prepare(m_statement, query.c_str(), query.size()))
		STMT_ERROR(m_statement)
	else
	{
		m_bind_count = mysql_stmt_param_count(m_statement);

		if (m_bind_count > 0)
		{
			m_binds = new bind[m_bind_count];
			m_my_binds = new MYSQL_BIND[m_bind_count];

			if (m_my_binds)
				memset(m_my_binds, '\0', sizeof(MYSQL_BIND) * m_bind_count);
		}
	}
}

//
// Destructor
//
statement::~statement()
{
	if (m_my_binds)
		delete [] m_my_binds;

	if (m_binds)
		delete [] m_binds;

	if (m_statement)
		mysql_stmt_close(m_statement);
}

//
// Set connection reference, used by concurrency namespace functions
//
void statement::set_connection(connection_ref& connection)
{
	m_connection = connection;
}

//
// Execute the query
//
u64 statement::execute()
{
	if (m_my_binds && mysql_stmt_bind_param(m_statement, m_my_binds))
		STMT_ERROR_RETURN_FALSE(m_statement);

	if (mysql_stmt_execute(m_statement))
		STMT_ERROR_RETURN_FALSE(m_statement);

	return mysql_stmt_affected_rows(m_statement);
}

u64 statement::insert()
{
	if (m_my_binds && mysql_stmt_bind_param(m_statement, m_my_binds))
		STMT_ERROR_RETURN_FALSE(m_statement);

	if (mysql_stmt_execute(m_statement))
		STMT_ERROR_RETURN_FALSE(m_statement);

	return mysql_stmt_insert_id(m_statement);
}

result_set_ref statement::query()
{
	result_set_ref rs;

	if (m_my_binds && mysql_stmt_bind_param(m_statement, m_my_binds))
		STMT_ERROR_RETURN_RS(m_statement);

	if (mysql_stmt_execute(m_statement))
		STMT_ERROR_RETURN_RS(m_statement);

	rs.reset(new result_set(this));
	return rs;
}

//
// Set a value to the sql command
//
void statement::set_blob(u32 index, stream_ref stream)
{
	if(!stream)
		return;

	if(index >= m_bind_count)
        throw std::out_of_range("Field index out of range");
    
	bind& bind = m_binds[index];
	MYSQL_BIND& mybind = m_my_binds[index];

	stream->seekg(0, std::ios_base::end);
	u64 size = stream->tellg();
	stream->seekg(0);

	bind.set_input(MYSQL_TYPE_BLOB, &mybind, NULL, static_cast<long unsigned int>(size));

	stream->read(bind.buffer(), bind.length());
}

void statement::set_data(u32 index, const data_ref& data)
{
	if(!data)
		return;

	if(index >= m_bind_count)
        throw std::out_of_range("Field index out of range");

	bind& bind = m_binds[index];
	MYSQL_BIND& mybind = m_my_binds[index];

	bind.set_input(MYSQL_TYPE_BLOB, &mybind);
	bind.m_data = data;
	mybind.buffer = (void*)data->get();
	mybind.buffer_length = data->size();
}

void statement::set_date_time(u32 index, const date_time& date_time)
{
	if(index >= m_bind_count)
        throw std::out_of_range("Field index out of range");

	bind& bind = m_binds[index];
	MYSQL_BIND& mybind = m_my_binds[index];

	bind.set_input(MYSQL_TYPE_DATETIME, &mybind);
	bind.m_time = date_time.mysql_time();
	mybind.buffer = (void*)&bind.m_time;
	mybind.buffer_length = sizeof(MYSQL_TIME);
}

void statement::set_date(u32 index, const date_time& date_time)
{
	if(index >= m_bind_count)
        throw std::out_of_range("Field index out of range");

	bind& bind = m_binds[index];
	MYSQL_BIND& mybind = m_my_binds[index];

	bind.set_input(MYSQL_TYPE_DATE, &mybind);
	bind.m_time = date_time.date().mysql_time();
	mybind.buffer = (void*)&bind.m_time;
	mybind.buffer_length = sizeof(MYSQL_TIME);
}

void statement::set_time(u32 index, const mariadb::time& time)
{
	if(index >= m_bind_count)
        throw std::out_of_range("Field index out of range");

	bind& bind = m_binds[index];
	MYSQL_BIND& mybind = m_my_binds[index];

	bind.set_input(MYSQL_TYPE_TIME, &mybind);
	bind.m_time = time.mysql_time();
	mybind.buffer = (void*)&bind.m_time;
	mybind.buffer_length = sizeof(MYSQL_TIME);
}

void statement::set_decimal(u32 index, const decimal& dec)
{
	if(index >= m_bind_count)
        throw std::out_of_range("Field index out of range");

	bind& bind = m_binds[index];
	MYSQL_BIND& mybind = m_my_binds[index];
	std::string str = dec.str();

	bind.set_input(MYSQL_TYPE_STRING, &mybind, str.c_str(), static_cast<unsigned long>((str.size())));
}

void statement::set_string(u32 index, const char* value)
{
	if(index >= m_bind_count)
        throw std::out_of_range("Field index out of range");

	bind& bind = m_binds[index];
	MYSQL_BIND& mybind = m_my_binds[index];

	bind.set_input(MYSQL_TYPE_STRING, &mybind, value, static_cast<unsigned long>(strlen(value)));
}

void statement::set_boolean(u32 index, bool value)
{
	if(index >= m_bind_count)
        throw std::out_of_range("Field index out of range");

	bind& bind = m_binds[index];
	MYSQL_BIND& mybind = m_my_binds[index];

	bind.set_input(MYSQL_TYPE_TINY, &mybind);
	bind.m_unsigned64 = value;
}

void statement::set_unsigned8(u32 index, u8 value)
{
	if(index >= m_bind_count)
        throw std::out_of_range("Field index out of range");

	bind& bind = m_binds[index];
	MYSQL_BIND& mybind = m_my_binds[index];

	bind.set_input(MYSQL_TYPE_TINY, &mybind);
	bind.m_unsigned64 = value;
	mybind.is_unsigned = true;
}

void statement::set_signed8(u32 index, s8 value)
{
	if(index >= m_bind_count)
        throw std::out_of_range("Field index out of range");

	bind& bind = m_binds[index];
	MYSQL_BIND& mybind = m_my_binds[index];

	bind.set_input(MYSQL_TYPE_TINY, &mybind);
	bind.m_signed64 = value;
}

void statement::set_unsigned16(u32 index, u16 value)
{
	if(index >= m_bind_count)
        throw std::out_of_range("Field index out of range");

	bind& bind = m_binds[index];
	MYSQL_BIND& mybind = m_my_binds[index];

	bind.set_input(MYSQL_TYPE_SHORT, &mybind);
	bind.m_unsigned64 = value;
	mybind.is_unsigned = true;
}

void statement::set_signed16(u32 index, s16 value)
{
	if(index >= m_bind_count)
        throw std::out_of_range("Field index out of range");

	bind& bind = m_binds[index];
	MYSQL_BIND& mybind = m_my_binds[index];

	bind.set_input(MYSQL_TYPE_SHORT, &mybind);
	bind.m_signed64 = value;
}

void statement::set_unsigned32(u32 index, u32 value)
{
	if(index >= m_bind_count)
		throw std::out_of_range("Field index out of range");

	bind& bind = m_binds[index];
	MYSQL_BIND& mybind = m_my_binds[index];

	bind.set_input(MYSQL_TYPE_LONG, &mybind);
	bind.m_unsigned64 = value;
	mybind.is_unsigned = true;
}

void statement::set_signed32(u32 index, s32 value)
{
	if(index >= m_bind_count)
        throw std::out_of_range("Field index out of range");

	bind& bind = m_binds[index];
	MYSQL_BIND& mybind = m_my_binds[index];

	bind.set_input(MYSQL_TYPE_LONG, &mybind);
	bind.m_signed64 = value;
}

void statement::set_unsigned64(u32 index, u64 value)
{
	if(index >= m_bind_count)
        throw std::out_of_range("Field index out of range");

	bind& bind = m_binds[index];
	MYSQL_BIND& mybind = m_my_binds[index];

	bind.set_input(MYSQL_TYPE_LONGLONG, &mybind);
	bind.m_unsigned64 = value;
	mybind.is_unsigned = true;
}

void statement::set_signed64(u32 index, s64 value)
{
	if(index >= m_bind_count)
        throw std::out_of_range("Field index out of range");

	bind& bind = m_binds[index];
	MYSQL_BIND& mybind = m_my_binds[index];

	bind.set_input(MYSQL_TYPE_LONGLONG, &mybind);
	bind.m_signed64 = value;
}

void statement::set_float(u32 index, f32 value)
{
	if(index >= m_bind_count)
        throw std::out_of_range("Field index out of range");

	bind& bind = m_binds[index];
	MYSQL_BIND& mybind = m_my_binds[index];

	bind.set_input(MYSQL_TYPE_FLOAT, &mybind);
	bind.m_float32[0] = value;
}

void statement::set_double(u32 index, f64 value)
{
	if(index >= m_bind_count)
        throw std::out_of_range("Field index out of range");

	bind& bind = m_binds[index];
	MYSQL_BIND& mybind = m_my_binds[index];

	bind.set_input(MYSQL_TYPE_DOUBLE, &mybind);
	bind.m_double64 = value;
}

void statement::set_null(u32 index)
{
	if(index >= m_bind_count)
        throw std::out_of_range("Field index out of range");

	bind& bind = m_binds[index];
	MYSQL_BIND& mybind = m_my_binds[index];

	bind.set_input(MYSQL_TYPE_NULL, &mybind);
}
