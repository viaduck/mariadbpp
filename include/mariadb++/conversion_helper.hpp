#ifndef MARIADBCLIENTPP_CONVERSION_HELPER_H
#define MARIADBCLIENTPP_CONVERSION_HELPER_H

#include <limits>

template <typename T, typename K>
inline T checked_cast(K value) {
    if(value < std::numeric_limits<T>::min() || value > std::numeric_limits<T>::max())
        return T();

    return static_cast<T>(value);
};

template <typename T>
inline T string_cast(std::string str) {
    size_t endPos;
    int parsedNumber = std::stoi(str, &endPos);

    if(endPos != str.size())
        return T();

    return checked_cast<T>(parsedNumber);
}

template <>
inline unsigned int string_cast(std::string str) {
    unsigned long parsedNumber = string_cast<unsigned long>(str);

    return checked_cast<unsigned int>(parsedNumber);
}

template <>
inline unsigned long string_cast(std::string str) {
    size_t endPos;
    unsigned long parsedNumber = std::stoul(str, &endPos);

    if(endPos != str.size())
        return 0;

    return parsedNumber;
}

template <>
inline unsigned long long string_cast(std::string str) {
    size_t endPos;
    unsigned long long parsedNumber = std::stoull(str, &endPos);

    if(endPos != str.size())
        return 0;

    return parsedNumber;
}

template <>
inline long long string_cast(std::string str) {
    size_t endPos;
    long long parsedNumber = std::stoll(str, &endPos);

    if(endPos != str.size())
        return 0;

    return parsedNumber;
}

template <>
inline double string_cast(std::string str) {
    size_t endPos;
    double parsedNumber = std::stod(str, &endPos);

    if(endPos != str.size())
        return 0;

    return parsedNumber;
}

template <>
inline float string_cast(std::string str) {
    size_t endPos;
    float parsedNumber = std::stof(str, &endPos);

    if(endPos != str.size())
        return 0;

    return parsedNumber;
}

#endif //MARIADBCLIENTPP_CONVERSION_HELPER_H
