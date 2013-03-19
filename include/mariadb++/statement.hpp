//
//  M A R I A D B + +
//
//	Author   : Sylvain Rochette Langlois
//	License  : Boost Software License (http://www.boost.org/users/license.html)
//

#ifndef _MARIADB_STATEMENT_HPP_
#define _MARIADB_STATEMENT_HPP_

#include "last_error.hpp"
#include "result_set.hpp"

namespace mariadb
{
	class connection;
	typedef MARIADB_STD::shared_ptr<connection> connection_ref;

	//
	// Statement
	//
	class worker;
	class result_set;
	class statement : public last_error
	{
		friend class connection;
		friend class result_set;
		friend class worker;

	public:
		//
		// Destructor
		//
		virtual ~statement();

		//
		// Set a value
		//
		void set_blob(u32 index, stream_ref value);
		void set_date_time(u32 index, const date_time& date_time);
		void set_date(u32 index, const date_time& date_time);
		void set_time(u32 index, const time& time);
		void set_data(u32 index, const data_ref& data);
		void set_decimal(u32 index, const decimal& dec);
		void set_string(u32 index, const char* value);
		void set_enum(u32 index, const char* value);
		void set_boolean(u32 index, bool value);
		void set_unsigned8(u32 index, u8 value);
		void set_signed8(u32 index, s8 value);
		void set_unsigned16(u32 index, u16 value);
		void set_signed16(u32 index, s16 value);
		void set_unsigned32(u32 index, u32 value);
		void set_signed32(u32 index, s32 value);
		void set_unsigned64(u32 index, u64 value);
		void set_signed64(u32 index, s64 value);
		void set_float(u32 index, f32 value);
		void set_double(u32 index, f64 value);
		void set_null(u32 index);

		//
		// Execute the query
		//
		s32 execute();
		u32 insert();

		//
		// This is not the best alternative, use the result_set from a connection instead if you can.
		// This can be a little overkill compage to a standard result_set from a connection...
		//
		result_set_ref query();

		//
		// Set connection reference, used by concurrency namespace functions
		//
		void set_connection(connection_ref& connection);

	private:
		//
		// Constructor
		//
		statement(connection* connection, const char* query);

	private:
		connection_ref m_connection;
		MYSQL_STMT*    m_statement;
		MYSQL_BIND*    m_my_binds;
		bind*          m_binds;
	};

	typedef MARIADB_STD::shared_ptr<statement> statement_ref;
}

#endif
