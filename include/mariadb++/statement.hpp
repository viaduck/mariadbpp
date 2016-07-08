//
//  M A R I A D B + +
//
//	Author   : Sylvain Rochette Langlois
//	License  : Boost Software License (http://www.boost.org/users/license.html)
//

#ifndef _MARIADB_STATEMENT_HPP_
#define _MARIADB_STATEMENT_HPP_

#include <mariadb++/last_error.hpp>
#include <mariadb++/result_set.hpp>

typedef struct st_mysql_stmt MYSQL_STMT;

namespace mariadb
{
	class connection;
	class worker;
	class result_set;
	typedef std::shared_ptr<connection> connection_ref;

	/**
	 * Class representing a prepared statement with binding functionality
	 */
	class statement : public last_error
	{
		friend class connection;
		friend class result_set;
		friend class worker;

	public:
		/**
		 * Destructor
		 */
		virtual ~statement();

        /**
         * Binds a binary blob to a parameter
         *
         * @param index Index of parameter to bind to
         * @param Binary blob to bind
         */
		void set_blob(u32 index, stream_ref value);

        /**
        * Binds a date_time to a parameter
        *
        * @param index Index of parameter to bind to
        * @param dt Date_time to bind
        */
		void set_date_time(u32 index, const date_time& dt);

        /**
        * Binds a date to a parameter
        *
        * @param index Index of parameter to bind to
        * @param dt Date to bind
        */
		void set_date(u32 index, const date_time& dt);

        /**
        * Binds a time to a parameter
        *
        * @param index Index of parameter to bind to
        * @param tm Time to bind
        */
		void set_time(u32 index, const time& tm);

        /**
        * Binds a binary data blob to a parameter
        *
        * @param index Index of parameter to bind to
        * @param data Binary data blob to bind
        */
		void set_data(u32 index, const data_ref& data);

        /**
        * Binds a decimal to a parameter
        *
        * @param index Index of parameter to bind to
        * @param dec Decimal to bind
        */
		void set_decimal(u32 index, const decimal& dec);

        /**
        * Binds a string to a parameter
        *
        * @param index Index of parameter to bind to
        * @param value String to bind
        */
		void set_string(u32 index, const std::string &value);

        /**
        * Binds a enum to a parameter
        *
        * @param index Index of parameter to bind to
        * @param value Enum to bind
        */
		void set_enum(u32 index, const char* value);

        /**
        * Binds a boolean to a parameter
        *
        * @param index Index of parameter to bind to
        * @param value Boolean to bind
        */
		void set_boolean(u32 index, bool value);

        /**
        * Binds a unsigned 8-bit integer to a parameter
        *
        * @param index Index of parameter to bind to
        * @param value Unsigned 8-bit integer to bind
        */
		void set_unsigned8(u32 index, u8 value);

        /**
        * Binds a signed 8-bit integer to a parameter
        *
        * @param index Index of parameter to bind to
        * @param value Signed 8-bit integer to bind
        */
		void set_signed8(u32 index, s8 value);

        /**
        * Binds a unsigned 16-bit integer to a parameter
        *
        * @param index Index of parameter to bind to
        * @param value Unsigned 16-bit integer to bind
        */
		void set_unsigned16(u32 index, u16 value);

        /**
        * Binds a signed 16-bit integer to a parameter
        *
        * @param index Index of parameter to bind to
        * @param value Signed 16-bit integer to bind
        */
		void set_signed16(u32 index, s16 value);

        /**
        * Binds a unsigned 32-bit integer to a parameter
        *
        * @param index Index of parameter to bind to
        * @param value Unsigned 32-bit integer to bind
        */
		void set_unsigned32(u32 index, u32 value);

        /**
        * Binds a signed 32-bit integer to a parameter
        *
        * @param index Index of parameter to bind to
        * @param value Signed 32-bit integer to bind
        */
		void set_signed32(u32 index, s32 value);

        /**
        * Binds a unsigned 64-bit integer to a parameter
        *
        * @param index Index of parameter to bind to
        * @param value Unsigned 64-bit integer to bind
        */
		void set_unsigned64(u32 index, u64 value);

        /**
        * Binds a signed 64-bit integer to a parameter
        *
        * @param index Index of parameter to bind to
        * @param value Signed 64-bit integer to bind
        */
		void set_signed64(u32 index, s64 value);

        /**
        * Binds a float to a parameter
        *
        * @param index Index of parameter to bind to
        * @param value Float to bind
        */
		void set_float(u32 index, f32 value);

        /**
        * Binds a double to a parameter
        *
        * @param index Index of parameter to bind to
        * @param value Double to bind
        */
		void set_double(u32 index, f64 value);

        /**
        * Binds a null to a parameter
        *
        * @param index Index of parameter to bind to
        */
		void set_null(u32 index);

		/**
		 * Execute the query and return the number of rows affected
		 *
		 * @return Number of rows affected or zero on error
		 */
		u64 execute();

        /**
         * Execute the query and return the last insert id
         *
         * @return Last insert ID or zero on error
         */
		u64 insert();

		/**
		 * Execute the query and return a result set
		 *
		 * @return Result set containing a result or an empty set on error
		 */
		result_set_ref query();

		/**
		 * Set connection ref, used by concurrency
		 */
		void set_connection(connection_ref& connection);

	private:
		/**
		 * Private constructor used by connection
		 */
		statement(connection* conn, const std::string &query);

	private:
        // number of binds in this query
		unsigned long  m_bind_count;
        // reference to parent connection
		connection_ref m_connection;
        // pointer to underlying statement
		MYSQL_STMT*    m_statement;
        // pointer to underlying bindings
		MYSQL_BIND*    m_my_binds;
        // pointer to bind managers
		bind*          m_binds;
	};

	typedef std::shared_ptr<statement> statement_ref;
}

#endif
