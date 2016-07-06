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
	class bind;
	class connection;
	class statement;

	/**
	 * Class used to store query and statement results
	 */
	class result_set : public last_error
	{
		friend class connection;
		friend class statement;

		typedef std::map<std::string, u32> map_indexes_t;

	public:
		/**
		 * Destructs the result_set and frees all result data
		 */
		virtual ~result_set();

		/**
		 * Gets a binary blob result by column name
		 *
		 * @param name Column name (case-sensitive) to get
		 * @return Reference to the binary data stream
		 */
		stream_ref get_blob(const std::string &name) const;

        /**
		 * Gets a binary blob result by column name
		 *
		 * @param name Column name (case-sensitive) to get
		 * @return Reference to the binary data blob
		 */
		data_ref get_data(const std::string &name) const;

        /**
		 * Gets a date result by column name
		 *
		 * @param name Column name (case-sensitive) to get
		 * @return Reference to the date
		 */
		date_time get_date(const std::string &name) const;

        /**
		 * Gets a date_time result by column name
		 *
		 * @param name Column name (case-sensitive) to get
		 * @return Reference to the date_time
		 */
		date_time get_date_time(const std::string &name) const;

        /**
		 * Gets a time result by column name
		 *
		 * @param name Column name (case-sensitive) to get
		 * @return Reference to the time
		 */
		time get_time(const std::string &name) const;

        /**
		 * Gets a decimal result by column name
		 *
		 * @param name Column name (case-sensitive) to get
		 * @return Reference to the decimal
		 */
		decimal get_decimal(const std::string &name) const;

        /**
		 * Gets a string result by column name
		 *
		 * @param name Column name (case-sensitive) to get
		 * @return String result
		 */
		std::string get_string(const std::string &name) const;

        /**
		 * Gets a boolean result by column name
		 *
		 * @param name Column name (case-sensitive) to get
		 * @return Boolean result
		 */
		bool get_boolean(const std::string &name) const;

        /**
		 * Gets a unsigned 8-bit integer result by column name
		 *
		 * @param name Column name (case-sensitive) to get
		 * @return Unsigned 8-bit result
		 */
		u8 get_unsigned8(const std::string &name) const;

        /**
		 * Gets a signed 8-bit integer result by column name
		 *
		 * @param name Column name (case-sensitive) to get
		 * @return Signed 8-bit result
		 */
		s8 get_signed8(const std::string &name) const;

        /**
		 * Gets a unsigned 16-bit integer result by column name
		 *
		 * @param name Column name (case-sensitive) to get
		 * @return Unsigned 16-bit result
		 */
		u16 get_unsigned16(const std::string &name) const;

        /**
		 * Gets a Signed 16-bit integer result by column name
		 *
		 * @param name Column name (case-sensitive) to get
		 * @return Signed 16-bit result
		 */
		s16 get_signed16(const std::string &name) const;

        /**
		 * Gets a unsigned 32-bit integer result by column name
		 *
		 * @param name Column name (case-sensitive) to get
		 * @return Unsigned 32-bit result
		 */
		u32 get_unsigned32(const std::string &name) const;

        /**
		 * Gets a signed 32-bit integer result by column name
		 *
		 * @param name Column name (case-sensitive) to get
		 * @return Signed 32-bit result
		 */
		s32 get_signed32(const std::string &name) const;

        /**
		 * Gets a unsigned 64-bit integer result by column name
		 *
		 * @param name Column name (case-sensitive) to get
		 * @return Unsigned 64-bit result
		 */
		u64 get_unsigned64(const std::string &name) const;

        /**
		 * Gets a signed 64-bit integer result by column name
		 *
		 * @param name Column name (case-sensitive) to get
		 * @return Signed 64-bit result
		 */
		s64 get_signed64(const std::string &name) const;

        /**
		 * Gets a float result by column name
		 *
		 * @param name Column name (case-sensitive) to get
		 * @return Float result
		 */
		f32 get_float(const std::string &name) const;

        /**
		 * Gets a double result by column name
		 *
		 * @param name Column name (case-sensitive) to get
		 * @return Double result
		 */
		f64 get_double(const std::string &name) const;

        /**
		 * Indicates whether a result is null by column name
		 *
		 * @param name Column name (case-sensitive) to check
		 * @return True on null
		 */
		bool is_null(const std::string &name) const;

        // get by index

        /**
         * Gets a binary blob result by column index
         *
         * @param name Column index to get
         * @return Reference to the binary data stream
         */
		stream_ref get_blob(u32 index) const;

        /**
		 * Gets a binary blob result by column index
		 *
		 * @param name Column index to get
		 * @return Reference to the binary data blob
		 */
		data_ref get_data(u32 index) const;

        /**
		 * Gets a date result by column index
		 *
		 * @param name Column index to get
		 * @return Reference to the date
		 */
		date_time get_date(u32 index) const;

        /**
		 * Gets a date_time result by column index
		 *
		 * @param name Column index to get
		 * @return Reference to the date_time
		 */
		date_time get_date_time(u32 index) const;

        /**
		 * Gets a time result by column index
		 *
		 * @param name Column index to get
		 * @return Reference to the time
		 */
		time get_time(u32 index) const;

        /**
		 * Gets a decimal result by column index
		 *
		 * @param name Column index to get
		 * @return Reference to the decimal
		 */
		decimal get_decimal(u32 index) const;

        /**
		 * Gets a string result by column index
		 *
		 * @param name Column index to get
		 * @return String result
		 */
		std::string get_string(u32 index) const;

        /**
		 * Gets a boolean result by column index
		 *
		 * @param name Column index to get
		 * @return Boolean result
		 */
		bool get_boolean(u32 index) const;

        /**
		 * Gets a unsigned 8-bit integer result by column index
		 *
		 * @param name Column index to get
		 * @return Unsigned 8-bit result
		 */
		u8 get_unsigned8(u32 index) const;

        /**
		 * Gets a signed 8-bit integer result by column index
		 *
		 * @param name Column index to get
		 * @return Signed 8-bit result
		 */
		s8 get_signed8(u32 index) const;

        /**
		 * Gets a unsigned 16-bit integer result by column index
		 *
		 * @param name Column index to get
		 * @return Unsigned 16-bit result
		 */
		u16 get_unsigned16(u32 index) const;

        /**
        * Gets a Signed 16-bit integer result by column index
        *
        * @param name Column index to get
        * @return Signed 16-bit result
        */
		s16 get_signed16(u32 index) const;

        /**
		 * Gets a unsigned 32-bit integer result by column index
		 *
		 * @param name Column index to get
		 * @return Unsigned 32-bit result
		 */
		u32 get_unsigned32(u32 index) const;

        /**
		 * Gets a signed 32-bit integer result by column index
		 *
		 * @param name Column index to get
		 * @return Signed 32-bit result
		 */
		s32 get_signed32(u32 index) const;

        /**
		 * Gets a unsigned 64-bit integer result by column index
		 *
		 * @param name Column index to get
		 * @return Unsigned 64-bit result
		 */
		u64 get_unsigned64(u32 index) const;

        /**
		 * Gets a signed 64-bit integer result by column index
		 *
		 * @param name Column index to get
		 * @return Signed 64-bit result
		 */
		s64 get_signed64(u32 index) const;

        /**
		 * Gets a float result by column index
		 *
		 * @param name Column index to get
		 * @return Float result
		 */
		f32 get_float(u32 index) const;

        /**
        * Gets a double result by column index
        *
        * @param name Column index to get
        * @return Double result
        */
		f64 get_double(u32 index) const;

        /**
		 * Indicates whether a result is null by column index
		 *
		 * @param name Column index to check
		 * @return True on null
		 */
		bool is_null(u32 index) const;

		/**
		 * Get the count of columns contained in this result_set
		 *
		 * @return Count of columns
		 */
		u64 column_count() const;

        /**
         * Get the index of a column by column-name (case sensitive)
         *
         * @param name Name of column to look up
         * @return Index of column if found, maximum uint32 if not found
         */
		u32 column_index(const std::string &name) const;

        /**
         * Gets the type of a column by index
         *
         * @param index Index of the column to examine
         * @return Type enum indicating column type
         */
		value::type column_type(u32 index);

        /**
         * Gets the name of a column by index
         *
         * @param index Index of the column to get the name for
         * @return String representing column name
         */
		const std::string column_name(u32 index);

        /**
         * Gets the size of the data contained in the column at index
         *
         * @param index Index of the column to get the size for
         * @return Size of the column contents for the currently active row
         */
        unsigned long column_size(u32 index) const;

		/**
		 * Gets the row index of the currently selected row
		 *
		 * @return Index of current row
		 */
		u64 row_index() const;

        /**
         * Gets the number of rows in this result
         *
         * @return Number of rows in result_set
         */
		u64 row_count() const;

		/**
		 * Fetch next row from result_set
		 *
		 * @return True if next row exists
		 */
		bool next();

        /**
         * Throws if the result set was created, but no row was ever fetched (using next())
         */
		void check_result_exists() const;

        /**
         * Set the current row index in result_set (seek to result).
         * Also immediately fetches the selected row.
         *
         * @param index Index of row to select
         * @return True if row could be seeked to and fetched.
         */
		bool set_row_index(u64 index);

	private:
		/**
		 * Create result_set from connection
		 */
		result_set(connection* conn);

        /**
         * Create result_set from statement
         */
		result_set(statement* stmt);

	private:
        // pointer to result set
        MYSQL_RES*         m_result_set;
        // pointer to array of fields
        MYSQL_FIELD*       m_fields;
        // pointer to current row
        MYSQL_ROW          m_row;
        // pointer to binds if any
        MYSQL_BIND*        m_my_binds;

        // pointer to each bind manager
        bind*              m_binds;
        // optional pointer to statement
        statement*         m_statement;
        // map caching column name by index
        map_indexes_t      m_indexes;
        // array of content lengths for the columns of current row
        long unsigned int* m_lengths;

        // count of fields per row
        u32                m_field_count;
        // indicates if a row was fetched using next()
        bool 			   m_has_result;
    };

	typedef std::shared_ptr<result_set> result_set_ref;
}

#endif
