#pragma once

#include <exception>
#include <string>

namespace gb {
class exception : public std::exception {
public:
    exception(const std::string& msg);

    const char* what() const noexcept override;

protected:
    std::string msg;
};
}
