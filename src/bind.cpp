//
//  M A R I A D B + +
//
//	Author   : Sylvain Rochette Langlois
//	License  : Boost Software License (http://www.boost.org/users/license.html)
//

#include <mysql/mysql.h>
#include <memory.h>
#include "bind.hpp"
#include "private.hpp"

using namespace mariadb;

//
// Constructor
//
bind::bind() :
	m_is_null(0),
	m_error(0),
	m_bind(NULL)
{
}

//
// Get buffer length
//
long unsigned int bind::length() const
{
	return m_bind->buffer_length;
}

//
// Get buffer
//
char* bind::buffer() const
{
	if (m_data)
		return m_data->get();

	return (char*)&m_unsigned64;
}

//
// Tell if the bind is null
//
bool bind::is_null() const
{
	return (m_is_null != 0);
}

//
// Set mysql bind
//
void bind::set_output(const MYSQL_FIELD& field, MYSQL_BIND* bind)
{
	set_input(field.type, bind, NULL, field.max_length);
}

void bind::set_input(enum_field_types mysql_type, MYSQL_BIND* bind, const char* buffer, long unsigned int length)
{
	m_bind = bind;
	m_bind->buffer = &m_unsigned64;
	m_bind->buffer_type = mysql_type;
	m_bind->buffer_length = 0;
	m_bind->is_null = &m_is_null;
	m_bind->error = &m_error;
	m_bind->length = &m_bind->buffer_length;

	switch (mysql_type)
	{
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
			if (length)
			{
				m_data = data_ref(new data<char>(length));
				m_bind->buffer = m_data->get();
				m_bind->buffer_length = m_data->size();

				if (buffer)
					memcpy(m_bind->buffer, buffer, length);
			}
			break;
	}
}
