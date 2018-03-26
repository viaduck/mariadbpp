//
//  M A R I A D B + +
//
//          Copyright Sylvain Rochette Langlois 2013,
//                    Frantisek Boranek 2015,
//                    The ViaDuck Project 2016 - 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef _MARIADB_DATA_HPP_
#define _MARIADB_DATA_HPP_

#include <string.h>
#include <mariadb++/types.hpp>
#include <iostream>

namespace mariadb {
template <typename Type>
class data {
   public:
    typedef Type char_type;

    //
    // Constructor
    //
    data() : m_count(0), m_size(0), m_data(0) {}

    data(u32 count) : m_count(0), m_size(0), m_data(0) { create(count); }

    data(const Type* data, size_t count) : m_count(0), m_size(0), m_data(0) {
        create(data, static_cast<u32>(count));
    }

    //
    // Destructor
    //
    virtual ~data() { destroy(); }

    //
    // Create the data
    //
    bool create(u32 count) {
        if (m_data) destroy();

        m_data = new Type[count];

        if (m_data == 0) return false;

        m_count = count;
        m_size = sizeof(Type) * count;
        m_position = 0;

        return true;
    }

    bool create(const Type* data, u32 count) {
        if (create(count)) {
            memcpy(m_data, data, m_size);
            return true;
        }

        return false;
    }

    bool resize(u32 count) {
        if (count > m_count) {
            Type* data = new Type[count];

            if (data == 0) return false;

            memcpy(data, m_data, m_size);
            delete[] m_data;
            m_data = data;
        }

        m_count = count;
        m_size = sizeof(Type) * count;

        return true;
    }

    //
    // Destroy
    //
    void destroy() {
        if (m_data) delete[] m_data;

        m_data = NULL;
        m_size = 0;
        m_count = 0;
        m_position = 0;
    }

    //
    // Convert data to a valid string
    //
    std::string string() const {
        std::string str;
        if (m_size) str.append((const char*)m_data, m_size);
        return str;
    }

    //
    // Get data / size
    //
    inline u32 size() const { return m_size; }
    inline Type* get() const { return m_data; }

    //
    // Operator to access the base directly
    //
    inline operator Type*() { return m_data; }
    inline operator Type*() const { return m_data; }

    //
    // Stream methods
    //
    std::streamsize read(char_type* buffer, std::streamsize size) {
        const std::streamsize amount = static_cast<std::streamsize>(m_size - m_position);
        const std::streamsize result = std::min(size, amount);

        if (result != 0) {
            std::copy(m_data + m_position, m_data + m_position + result, buffer);
            m_position += result;
            return result;
        }

        return size ? -1 : 0;
    }

    std::streamsize write(char_type* buffer, std::streamsize size) {
        const std::streamsize amount = static_cast<std::streamsize>(m_size - m_position);
        const std::streamsize result = std::min(size, amount);

        if (result != 0) {
            std::copy(buffer, buffer + result, m_data + m_position);
            m_position += result;
            return result;
        }

        return size ? -1 : 0;
    }

    std::streampos seek(std::streampos offset, std::ios_base::seekdir seekdir) {
        std::streampos pos;

        if (seekdir == std::ios_base::beg)
            pos = offset;
        else if (seekdir == std::ios_base::cur)
            pos = m_position + offset;
        else if (seekdir == std::ios_base::end)
            pos = m_size + offset;
        else
            throw std::ios_base::failure("Bad seek direction");

        if (pos < 0 || pos > m_size) throw std::ios_base::failure("Bad seek offset");

        m_position = static_cast<u32>(pos);
        return pos;
    }

   protected:
    u32 m_count;
    u32 m_size;
    u32 m_position;
    Type* m_data;
};

typedef std::shared_ptr< ::mariadb::data<char> > data_ref;
typedef std::basic_iostream< ::mariadb::data<char> > data_stream;
}

#endif
