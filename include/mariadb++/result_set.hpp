//
//  M A R I A D B + +
//
//	Author   : Sylvain Rochette Langlois
//	License  : Boost Software License (http://www.boost.org/users/license.html)
//

#ifndef _MARIADB_RESULT_SET_HPP_
#define _MARIADB_RESULT_SET_HPP_

#include <mysql.h>
#include <memory>
#include <map>
#include "data.hpp"
#include "date_time.hpp"
#include "decimal.hpp"
#include "last_error.hpp"

namespace mariadb
{
	//
	// Result set only work with connection for a short period of time, it's
	// why the connection pointer is not a reference to the shared object.
	//
	// If a Result set exist while a connection as been closed, your code need to be changed.
	//
	class bind;
	class connection;
	class statement;
	class result_set : public last_error
	{
		friend class connection;
		friend class statement;

	public:
		//
		// Destructor
		//
		virtual ~result_set();

		//
		// Get from name
		//
		stream_ref get_blob(const char* name) const;
		data_ref get_data(const char* name) const;
		date_time get_date(const char* name) const;
		date_time get_date_time(const char* name) const;
		time get_time(const char* name) const;
		decimal get_decimal(const char* name) const;
		const char* get_string(const char* name) const;
		bool get_boolean(const char* name) const;
		u8 get_unsigned8(const char* name) const;
		s8 get_signed8(const char* name) const;
		u16 get_unsigned16(const char* name) const;
		s16 get_signed16(const char* name) const;
		u32 get_unsigned32(const char* name) const;
		s32 get_signed32(const char* name) const;
		u64 get_unsigned64(const char* name) const;
		s64 get_signed64(const char* name) const;
		f32 get_float(const char* name) const;
		f64 get_double(const char* name) const;
		bool is_null(const char* name) const;

		//
		// Get from index
		//
		stream_ref get_blob(u32 index) const;
		data_ref get_data(u32 index) const;
		date_time get_date(u32 index) const;
		date_time get_date_time(u32 index) const;
		time get_time(u32 index) const;
		decimal get_decimal(u32 index) const;
		const char* get_string(u32 index) const;
		bool get_boolean(u32 index) const;
		u8 get_unsigned8(u32 index) const;
		s8 get_signed8(u32 index) const;
		u16 get_unsigned16(u32 index) const;
		s16 get_signed16(u32 index) const;
		u32 get_unsigned32(u32 index) const;
		s32 get_signed32(u32 index) const;
		u64 get_unsigned64(u32 index) const;
		s64 get_signed64(u32 index) const;
		f32 get_float(u32 index) const;
		f64 get_double(u32 index) const;
		bool is_null(u32 index) const;

		//
		// Get column count / type / name
		//
		u64 column_count() const;
		u32 column_index(const char* name) const;
		value::type column_type(u32 index);
		const char* column_name(u32 index);

		//
		// Get row count & index
		//
		u64 row_index() const;
		u64 row_count() const;

		//
		// Go to next row
		//
		bool next();

		//
		// Set row index
		//
		bool set_row_index(u64 index);

	private:
		//
		// Constructor
		//
		result_set(connection* connection);
		result_set(statement* statement);

	private:
		u32                m_field_count;
		long unsigned int* m_lengths;
		MYSQL_RES*         m_result_set;
		MYSQL_FIELD*       m_fields;
		MYSQL_ROW          m_row;
		MYSQL_BIND*        m_my_binds;
		bind*              m_binds;
		statement*         m_statement;

		std::map<std::string, u32> m_indexes;
	};

	typedef std::shared_ptr<result_set> result_set_ref;
}

#endif
