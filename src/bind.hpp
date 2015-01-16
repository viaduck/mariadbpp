//
//  M A R I A D B + +
//
//	Author   : Sylvain Rochette Langlois
//	License  : Boost Software License (http://www.boost.org/users/license.html)
//

#ifndef _MARIADB_BIND_HPP_
#define _MARIADB_BIND_HPP_

#include <mariadb++/types.hpp>
#include <mariadb++/data.hpp>

namespace mariadb
{
	class statement;
	class result_set;
	class bind
	{
		friend class statement;
		friend class result_set;

	public:
		//
		// Constructor
		//
		bind();

		//
		// Get buffer
		//
		char* buffer() const;

		//
		// Get buffer length
		//
		long unsigned int length() const;

		//
		// Tell if the bind is null
		//
		bool is_null() const;

		//
		// Set mysql bind for input / output
		//
		void set_output(const MYSQL_FIELD& field, MYSQL_BIND* bind);
		void set_input(enum_field_types mysql_type, MYSQL_BIND* bind, const char* buffer = NULL, long unsigned int length = 0);

	private:
		my_bool     m_is_null;
		my_bool     m_error;
		data_ref    m_data;
		MYSQL_BIND* m_bind;
		MYSQL_TIME  m_time;

		union
		{
			u64 m_unsigned64;
			s64 m_signed64;
			s32 m_signed32[2];
			f64 m_double64;
			f32 m_float32[2];
		};
	};
}

#endif
