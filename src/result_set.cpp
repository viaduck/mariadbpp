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
#include <mariadb++/conversion_helper.hpp>
#include "bind.hpp"
#include "private.hpp"

using namespace mariadb;

//
// Constructor
//
result_set::result_set(connection* connection) :
	m_field_count(0),
	m_lengths(NULL),
	m_result_set(mysql_store_result(connection->m_mysql)),
	m_fields(NULL),
	m_my_binds(NULL),
	m_binds(NULL),
	m_statement(NULL)
{
	if (m_result_set)
	{
		m_field_count = mysql_num_fields(m_result_set);
		m_fields = mysql_fetch_fields(m_result_set);

		for (u32 i = 0; i < m_field_count; ++i)
			m_indexes[m_fields[i].name] = i;
	}
}

result_set::result_set(statement* statement) :
	m_field_count(0),
	m_lengths(NULL),
	m_result_set(NULL),
	m_fields(NULL),
	m_my_binds(NULL),
	m_binds(NULL),
	m_statement(statement)
{
	int max_length = 1;
	mysql_stmt_attr_set(m_statement->m_statement, STMT_ATTR_UPDATE_MAX_LENGTH, &max_length);

	if (mysql_stmt_store_result(m_statement->m_statement))
		STMT_ERROR(m_statement->m_statement)
	else
	{
		m_field_count = mysql_stmt_field_count(m_statement->m_statement);
		m_result_set = mysql_stmt_result_metadata(m_statement->m_statement);
		m_fields = mysql_fetch_fields(m_result_set);

		if (m_field_count > 0)
		{
			m_binds = new bind[m_field_count];
			m_my_binds = new MYSQL_BIND[m_field_count];
			m_row = new char*[m_field_count];

			memset(m_my_binds, 0, sizeof(MYSQL_BIND) * m_field_count);
		}

		for (u32 i = 0; i < m_field_count; ++i)
		{
			m_indexes[m_fields[i].name] = i;
			m_binds[i].set_output(m_fields[i], &m_my_binds[i]);
			m_row[i] = m_binds[i].buffer();
		}

		if (m_field_count > 0)
			mysql_stmt_bind_result(m_statement->m_statement, m_my_binds);
	}
}

//
// Destructor
//
result_set::~result_set()
{
	if (m_result_set)
		mysql_free_result(m_result_set);

	if (m_my_binds)
		delete [] m_my_binds;

	if (m_statement)
	{
		if (m_row)
			delete [] m_row;

		mysql_stmt_free_result(m_statement->m_statement);
	}

	if (m_binds)
		delete [] m_binds;
}

//
// Get from name
//
stream_ref result_set::get_blob(const char* name) const
{
	return get_blob(column_index(name));
}

data_ref result_set::get_data(const char* name) const
{
	return get_data(column_index(name));
}

const char* result_set::get_string(const char* name) const
{
	return get_string(column_index(name));
}

date_time result_set::get_date(const char* name) const
{
	return get_date(column_index(name));
}

date_time result_set::get_date_time(const char* name) const
{
	return get_date_time(column_index(name));
}

mariadb::time result_set::get_time(const char* name) const
{
	return get_time(column_index(name));
}

decimal result_set::get_decimal(const char* name) const
{
	return get_decimal(column_index(name));
}

bool result_set::get_boolean(const char* name) const
{
	return get_boolean(column_index(name));
}

u8 result_set::get_unsigned8(const char* name) const
{
	return get_unsigned8(column_index(name));
}

s8 result_set::get_signed8(const char* name) const
{
	return get_signed8(column_index(name));
}

u16 result_set::get_unsigned16(const char* name) const
{
	return get_unsigned16(column_index(name));
}

s16 result_set::get_signed16(const char* name) const
{
	return get_signed16(column_index(name));
}

u32 result_set::get_unsigned32(const char* name) const
{
	return get_unsigned32(column_index(name));
}

s32 result_set::get_signed32(const char* name) const
{
	return get_signed32(column_index(name));
}

s64 result_set::get_signed64(const char* name) const
{
	return get_signed64(column_index(name));
}

u64 result_set::get_unsigned64(const char* name) const
{
	return get_unsigned64(column_index(name));
}

f32 result_set::get_float(const char* name) const
{
	return get_float(column_index(name));
}

f64 result_set::get_double(const char* name) const
{
	return get_double(column_index(name));
}

bool result_set::is_null(const char* name) const
{
	return is_null(column_index(name));
}

//
// Get from index
//
stream_ref result_set::get_blob(u32 index) const
{
	size_t len = m_statement ? m_binds[index].length() : m_lengths[index];

	if (!len)
		return stream_ref();

	std::istringstream* ss = new std::istringstream();
	ss->rdbuf()->pubsetbuf(m_row[index], len);
	return stream_ref(ss);
}

data_ref result_set::get_data(u32 index) const
{
	size_t len = m_statement ? m_binds[index].length() : m_lengths[index];

	if (!len)
		return data_ref();

	return data_ref(new data<char>(m_row[index], len));
}

decimal result_set::get_decimal(u32 index) const
{
	return decimal(m_row[index]);
}

date_time result_set::get_date(u32 index) const
{
	if (m_statement)
		return mariadb::date_time(m_binds[index].m_time);

	return date_time(m_row[index]).date();
}

date_time result_set::get_date_time(u32 index) const
{
	if (m_statement)
		return mariadb::date_time(m_binds[index].m_time);

	return date_time(m_row[index]);
}

mariadb::time result_set::get_time(u32 index) const
{
	if (m_statement)
		return mariadb::time(m_binds[index].m_time);

	return mariadb::time(m_row[index]);
}

const char* result_set::get_string(u32 index) const
{
	return m_row[index];
}

bool result_set::get_boolean(u32 index) const
{
	if (m_statement)
		return (m_binds[index].m_unsigned64 != 0);

	return string_cast<bool>(m_row[index]);
}

u8 result_set::get_unsigned8(u32 index) const
{
	if (m_statement)
		return checked_cast<u8>(0x00000000000000ff & m_binds[index].m_unsigned64);

	return string_cast<u8>(m_row[index]);
}

s8 result_set::get_signed8(u32 index) const
{
	if (m_statement)
		return checked_cast<s8>(0x00000000000000ff & m_binds[index].m_signed64);

	return string_cast<s8>(m_row[index]);
}

u16 result_set::get_unsigned16(u32 index) const
{
	if (m_statement)
		return checked_cast<u16>(0x000000000000ffff & m_binds[index].m_unsigned64);

	return string_cast<u16>(m_row[index]);
}

s16 result_set::get_signed16(u32 index) const
{
	if (m_statement)
		return checked_cast<s16>(0x000000000000ffff & m_binds[index].m_signed64);

	return string_cast<s16>(m_row[index]);
}

u32 result_set::get_unsigned32(u32 index) const
{
	if (m_statement)
		return checked_cast<u32>(0x00000000ffffffff & m_binds[index].m_unsigned64);

	return string_cast<u32>(m_row[index]);
}

s32 result_set::get_signed32(u32 index) const
{
	if (m_statement)
		return m_binds[index].m_signed32[0];

	return string_cast<s32>(m_row[index]);
}

u64 result_set::get_unsigned64(u32 index) const
{
	if (m_statement)
		return m_binds[index].m_unsigned64;

	return string_cast<u64>(m_row[index]);
}

s64 result_set::get_signed64(u32 index) const
{
	if (m_statement)
		return m_binds[index].m_signed64;

	return string_cast<s64>(m_row[index]);
}

f32 result_set::get_float(u32 index) const
{
	if (m_statement)
		return m_binds[index].m_float32[0];

	return string_cast<f32>(m_row[index]);
}

f64 result_set::get_double(u32 index) const
{
	if (m_statement)
		return checked_cast<f64>(m_binds[index].m_double64);

	return string_cast<f64>(m_row[index]);
}

bool result_set::is_null(u32 index) const
{
	if (m_statement)
		return m_binds[index].is_null();

	return !m_row[index];
}

//
// Get column count / type / name
//
u64 result_set::column_count() const
{
	return m_field_count;
}

value::type result_set::column_type(u32 index)
{
	switch (m_fields[index].type)
	{
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
			return value::signed8;

		case MYSQL_TYPE_YEAR:
		case MYSQL_TYPE_SHORT:
			return value::signed16;

		case MYSQL_TYPE_INT24:
		case MYSQL_TYPE_LONG:
			return value::signed32;

		case MYSQL_TYPE_LONGLONG:
			return value::signed64;

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

const char* result_set::column_name(u32 index)
{
	return m_fields[index].name;
}

u32 result_set::column_index(const char* name) const
{
	const map_indexes_t::const_iterator i = m_indexes.find(name);

	if (i == m_indexes.end())
		return 0xffffffff;

	return i->second;
}

//
// Set row index
//
bool result_set::set_row_index(u64 index)
{
	if (m_statement)
		mysql_stmt_data_seek(m_statement->m_statement, index);
	else
		mysql_data_seek(m_result_set, index);
	return next();
}

//
// Go to next row
//
bool result_set::next()
{
	if (!m_result_set)
		return false;

	if (m_statement)
		return !mysql_stmt_fetch(m_statement->m_statement);

	m_row = mysql_fetch_row(m_result_set);
	m_lengths = mysql_fetch_lengths(m_result_set);
	return m_row != NULL;
}

//
// Get row count & index
//
u64 result_set::row_index() const
{
	if (m_statement)
		return (u64)mysql_stmt_row_tell(m_statement->m_statement);

	return (u64)mysql_row_tell(m_result_set);
}

u64 result_set::row_count() const
{
	if (m_statement)
		return (u64)mysql_stmt_num_rows(m_statement->m_statement);

	return (u64)mysql_num_rows(m_result_set);
}
