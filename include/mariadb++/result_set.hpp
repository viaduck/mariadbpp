//
//  M A R I A D B + +
//
//          Copyright Sylvain Rochette Langlois 2013,
//                    Frantisek Boranek 2015,
//                    The ViaDuck Project 2016 - 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef _MARIADB_RESULT_SET_HPP_
#define _MARIADB_RESULT_SET_HPP_

#include <map>
#include <vector>
#include <mariadb++/bind.hpp>
#include <mariadb++/data.hpp>
#include <mariadb++/date_time.hpp>
#include <mariadb++/decimal.hpp>
#include <mariadb++/last_error.hpp>

#define MAKE_GETTER_SIG_STR(nm, rtype, fq) rtype fq get_##nm(const std::string& name) const
#define MAKE_GETTER_SIG_INT(nm, rtype, fq) rtype fq get_##nm(u32 index) const

#define MAKE_GETTER_DECL(nm, rtype)   \
    MAKE_GETTER_SIG_STR(nm, rtype, ); \
    MAKE_GETTER_SIG_INT(nm, rtype, )

#define MAKE_GETTER(nm, rtype, vtype)                                                     \
    MAKE_GETTER_SIG_STR(nm, rtype, result_set::) { return get_##nm(column_index(name)); } \
    MAKE_GETTER_SIG_INT(nm, rtype, result_set::) {                                        \
        check_row_fetched();                                                              \
        check_type(index, vtype);                                                         \
                                                                                          \
        if (index >= m_field_count) throw std::out_of_range("Column index out of range");

namespace mariadb {
class connection;
class statement;

/*
 * This data is shared between a statement and its result_set,
 * therefore it needs to be destroyed only when
 * - the statement is freed
 * - all of the result_sets are freed
 *
 * A shared_ptr is used to keep the data alive in the statement and the result_set.
 */
struct statement_data {
    explicit statement_data(MYSQL_STMT* stmt) : m_statement(stmt) {}

    ~statement_data();

    // number of binds in this query
    unsigned long m_bind_count = 0;
    // pointer to underlying statement
    MYSQL_STMT* m_statement;
    // pointer to raw binds
    MYSQL_BIND* m_raw_binds = nullptr;
    // pointer to managed binds
    std::vector<bind_ref> m_binds;
};

typedef std::shared_ptr<statement_data> statement_data_ref;

/**
 * Class used to store query and statement results
 */
class result_set : public last_error {
    friend class connection;
    friend class statement;

    typedef std::map<std::string, u32> map_indexes_t;

   public:
    /**
     * Destructs the result_set and frees all result data
     */
    virtual ~result_set();

    /**
     * Get the count of columns contained in this result_set
     *
     * @return Count of columns
     */
    u32 column_count() const;

    /**
     * Get the index of a column by column-name (case sensitive)
     *
     * @param name Name of column to look up
     * @return Index of column if found, maximum uint32 if not found
     */
    u32 column_index(const std::string& name) const;

    /**
     * Gets the type of a column by index
     *
     * @param index Index of the column to examine
     * @return Type enum indicating column type
     */
    value::type column_type(u32 index) const;

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
     * Set the current row index in result_set (seek to result).
     * Also immediately fetches the selected row.
     *
     * @param index Index of row to select
     * @return True if row could be seeked to and fetched.
     */
    bool set_row_index(u64 index);

    // declare all getters
    MAKE_GETTER_DECL(blob, stream_ref);
    MAKE_GETTER_DECL(data, data_ref);
    MAKE_GETTER_DECL(date, date_time);
    MAKE_GETTER_DECL(date_time, date_time);
    MAKE_GETTER_DECL(time, time);
    MAKE_GETTER_DECL(decimal, decimal);
    MAKE_GETTER_DECL(string, std::string);
    MAKE_GETTER_DECL(boolean, bool);
    MAKE_GETTER_DECL(unsigned8, u8);
    MAKE_GETTER_DECL(signed8, s8);
    MAKE_GETTER_DECL(unsigned16, u16);
    MAKE_GETTER_DECL(signed16, s16);
    MAKE_GETTER_DECL(unsigned32, u32);
    MAKE_GETTER_DECL(signed32, s32);
    MAKE_GETTER_DECL(unsigned64, u64);
    MAKE_GETTER_DECL(signed64, s64);
    MAKE_GETTER_DECL(float, f32);
    MAKE_GETTER_DECL(double, f64);
    MAKE_GETTER_DECL(is_null, bool);

   private:
    /**
     * Create result_set from connection
     */
    explicit result_set(connection* conn);

    /**
     * Create result_set from statement
     */
    explicit result_set(const statement_data_ref& stmt);

    /**
     * Throws if the result set was created, but no row was ever fetched (using next())
     */
    void check_row_fetched() const;

    /**
     * Throws if the actual type of column at index cannot be converted to the requested type
     *
     * @param index Index of column to check
     * @param requested Requested type
     */
    void check_type(u32 index, value::type requested) const;

    // pointer to result set
    MYSQL_RES* m_result_set;
    // pointer to array of fields
    MYSQL_FIELD* m_fields;
    // pointer to current row
    MYSQL_ROW m_row;
    // pointer to raw binds
    MYSQL_BIND* m_raw_binds;

    // vector of managed binds
    std::vector<bind_ref> m_binds;
    // optional pointer to statement
    statement_data_ref m_stmt_data;
    // map caching column name by index
    map_indexes_t m_indexes;
    // array of content lengths for the columns of current row
    long unsigned int* m_lengths;

    // count of fields per row
    u32 m_field_count;
    // indicates if a row was fetched using next()
    bool m_was_fetched;
};

typedef std::shared_ptr<result_set> result_set_ref;
}

#endif
