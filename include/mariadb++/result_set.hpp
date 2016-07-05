//
//  M A R I A D B + +
//
//	Author   : Sylvain Rochette Langlois
//	License  : Boost Software License (http://www.boost.org/users/license.html)
//

#ifndef _MARIADB_RESULT_SET_HPP_
#define _MARIADB_RESULT_SET_HPP_

#include <map>
#include <mariadb++/data.hpp>
#include <mariadb++/date_time.hpp>
#include <mariadb++/decimal.hpp>
#include <mariadb++/last_error.hpp>

struct st_mysql_res;
typedef struct st_mysql_res MYSQL_RES;

struct st_mysql_field;
typedef struct st_mysql_field MYSQL_FIELD;

struct st_mysql_bind;
typedef struct st_mysql_bind MYSQL_BIND;

typedef char **MYSQL_ROW;

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

		typedef std::map<std::string, u32> map_indexes_t;

	public:
		//
		// Destructor
		//
		virtual ~result_set();

		//
		// Get from name
		//
		stream_ref get_blob(const std::string &name) const;
		data_ref get_data(const std::string &name) const;
		date_time get_date(const std::string &name) const;
		date_time get_date_time(const std::string &name) const;
		time get_time(const std::string &name) const;
		decimal get_decimal(const std::string &name) const;
		std::string get_string(const std::string &name) const;
		bool get_boolean(const std::string &name) const;
		u8 get_unsigned8(const std::string &name) const;
		s8 get_signed8(const std::string &name) const;
		u16 get_unsigned16(const std::string &name) const;
		s16 get_signed16(const std::string &name) const;
		u32 get_unsigned32(const std::string &name) const;
		s32 get_signed32(const std::string &name) const;
		u64 get_unsigned64(const std::string &name) const;
		s64 get_signed64(const std::string &name) const;
		f32 get_float(const std::string &name) const;
		f64 get_double(const std::string &name) const;
		bool is_null(const std::string &name) const;

		//
		// Get from index
		//
		stream_ref get_blob(u32 index) const;
		data_ref get_data(u32 index) const;
		date_time get_date(u32 index) const;
		date_time get_date_time(u32 index) const;
		time get_time(u32 index) const;
		decimal get_decimal(u32 index) const;
		std::string get_string(u32 index) const;
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
		u32 column_index(const std::string &name) const;
		value::type column_type(u32 index);
		const char* column_name(u32 index);
        unsigned long column_size(u32 index) const;

		//
		// Get row count & index
		//
		u64 row_index() const;
		u64 row_count() const;

		//
		// Go to next row
		//
		bool next();

		void check_result_exists() const;

		//
		// Set row index
		//
		bool set_row_index(u64 index);

	private:
		//
		// Constructor
		//
		result_set(connection* conn);
		result_set(statement* stmt);

	private:
		bool 			   m_has_result;
		u32                m_field_count;
		long unsigned int* m_lengths;
		MYSQL_RES*         m_result_set;
		MYSQL_FIELD*       m_fields;
		MYSQL_ROW          m_row;
		MYSQL_BIND*        m_my_binds;
		bind*              m_binds;
		statement*         m_statement;
		map_indexes_t      m_indexes;
	};

	typedef std::shared_ptr<result_set> result_set_ref;
}

#endif
