#include <gbase/io/string.hpp>

#include <string>

inline unsigned char higher_byte(unsigned char c) { return c >> 4; }

inline unsigned char lower_byte(unsigned char c) { return c & 0x0f; }

inline unsigned char uchar_to_hex(unsigned char c)
{
    if (c > 9) {
        return ('a' + c - 10);
    } else {
        return ('0' + c);
    }
}

std::string gb::io::uchar_arr_to_string(const unsigned char* arr, size_t n)
{
    std::string str;
    for (size_t i = 0; i < n; ++i) {
        str.push_back(uchar_to_hex(higher_byte(arr[i])));
        str.push_back(uchar_to_hex(lower_byte(arr[i])));
    }
    return str;
}
