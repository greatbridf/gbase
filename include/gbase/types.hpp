#pragma once

#include <ostream>

#include <gbase/net/socket_types.hpp>

namespace gb
{
    typedef unsigned char byte;

    class non_copyable {
    public:
        non_copyable() = default;
        non_copyable(const non_copyable&) = delete;
        non_copyable& operator=(const non_copyable&) = delete;
    };

    class interface : private non_copyable {
    public:
        virtual ~interface() = default;
    };

    class serializable {
    public:
        virtual std::string toString() const = 0;

        friend std::ostream& operator<<(std::ostream& os, const serializable& obj)
        {
            os << obj.toString();
            return os;
        }
    };
}
