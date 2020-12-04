#include <gbase/error.hpp>

gb::exception::exception(const std::string& msg)
    : msg(msg)
{}

const char* gb::exception::what() const noexcept
{
    return this->msg.c_str();
}
