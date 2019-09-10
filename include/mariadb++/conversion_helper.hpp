//
//  M A R I A D B + +
//
//          Copyright The ViaDuck Project 2016 - 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef MARIADBCLIENTPP_CONVERSION_HELPER_H
#define MARIADBCLIENTPP_CONVERSION_HELPER_H

#include <limits>
#include <stdexcept>

#ifdef WIN32
#undef max
#undef min
#endif

template <typename T, typename K>
inline T checked_cast(K value) {
    if (value < std::numeric_limits<T>::lowest() || value > std::numeric_limits<T>::max()) return T();

    return static_cast<T>(value);
}

template <typename T>
inline T string_cast(const std::string &str) {
    size_t endPos;
    int parsedNumber = std::stoi(str, &endPos);

    if (endPos != str.size()) return T();

    return checked_cast<T>(parsedNumber);
}

template <>
inline unsigned long string_cast(const std::string &str) {
    size_t endPos;
    unsigned long parsedNumber = std::stoul(str, &endPos);

    if (endPos != str.size()) return 0;

    return parsedNumber;
}

template <>
inline unsigned int string_cast(const std::string &str) {
    unsigned long parsedNumber = string_cast<unsigned long>(str);

    return checked_cast<unsigned int>(parsedNumber);
}

template <>
inline unsigned long long string_cast(const std::string &str) {
    size_t endPos;
    unsigned long long parsedNumber = std::stoull(str, &endPos);

    if (endPos != str.size()) return 0;

    return parsedNumber;
}

template <>
inline long long string_cast(const std::string &str) {
    size_t endPos;
    long long parsedNumber = std::stoll(str, &endPos);

    if (endPos != str.size()) return 0;

    return parsedNumber;
}

template <>
inline double string_cast(const std::string &str) {
    size_t endPos;
    try {
        double parsedNumber = std::stod(str, &endPos);

        if (endPos != str.size()) return 0;

        return parsedNumber;
    } catch (std::out_of_range &) {
        // Not a Number double
        return std::numeric_limits<double>::quiet_NaN();
    }
}

template <>
inline float string_cast(const std::string &str) {
    size_t endPos;
    try {
        float parsedNumber = std::stof(str, &endPos);

        if (endPos != str.size()) return 0;

        return parsedNumber;
    } catch (std::out_of_range &) {
        // Not a Number float
        return std::numeric_limits<float>::quiet_NaN();
    }
}

#endif  // MARIADBCLIENTPP_CONVERSION_HELPER_H
