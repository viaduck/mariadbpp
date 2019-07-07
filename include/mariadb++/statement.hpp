//
//  M A R I A D B + +
//
//          Copyright Sylvain Rochette Langlois 2013,
//                    Frantisek Boranek 2015,
//                    The ViaDuck Project 2016 - 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef _MARIADB_STATEMENT_HPP_
#define _MARIADB_STATEMENT_HPP_

#include <mariadb++/last_error.hpp>
#include <mariadb++/result_set.hpp>

#define MAKE_SETTER_SIG(nm, type, fq) void fq set_##nm(u32 index, type value)

#define MAKE_SETTER_DECL(nm, type) MAKE_SETTER_SIG(nm, type, )

#define MAKE_SETTER_BODY                                                                    \
    if (index >= m_data->m_bind_count) throw std::out_of_range("Field index out of range"); \
                                                                                            \
    bind& bind = *m_data->m_binds.at(index);

#define MAKE_SETTER(nm, type)                \
    MAKE_SETTER_SIG(nm, type, statement::) { \
        MAKE_SETTER_BODY

namespace mariadb {
class connection;
class worker;
class result_set;
typedef std::shared_ptr<connection> connection_ref;

/**
 * Class representing a prepared statement with binding functionality
 */
class statement : public last_error {
    friend class connection;
    friend class result_set;
    friend class worker;

   public:
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

    // declare all setters
    MAKE_SETTER_DECL(blob, stream_ref);
    MAKE_SETTER_DECL(date_time, const date_time&);
    MAKE_SETTER_DECL(date, const date_time&);
    MAKE_SETTER_DECL(time, const time&);
    MAKE_SETTER_DECL(data, const data_ref&);
    MAKE_SETTER_DECL(decimal, const decimal&);
    MAKE_SETTER_DECL(string, const std::string&);
    MAKE_SETTER_DECL(boolean, bool);
    MAKE_SETTER_DECL(unsigned8, u8);
    MAKE_SETTER_DECL(signed8, s8);
    MAKE_SETTER_DECL(unsigned16, u16);
    MAKE_SETTER_DECL(signed16, s16);
    MAKE_SETTER_DECL(unsigned32, u32);
    MAKE_SETTER_DECL(signed32, s32);
    MAKE_SETTER_DECL(unsigned64, u64);
    MAKE_SETTER_DECL(signed64, s64);
    MAKE_SETTER_DECL(float, f32);
    MAKE_SETTER_DECL(double, f64);
    void set_null(u32 index);

   private:
    /**
     * Private constructor used by connection
     */
    statement(connection* conn, const std::string& query);

   private:
    // reference to parent connection
    connection_ref m_connection;
    // reference to internal data, shared with all results
    statement_data_ref m_data;
};

typedef std::shared_ptr<statement> statement_ref;
}

#endif
