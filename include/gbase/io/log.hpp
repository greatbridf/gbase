#pragma once

#include <iostream>
#include <string>

namespace gb {
class logger {
public:
    enum level_t {
        ALL = 0,
        INFO = 1,
        WARN = 2,
        ERROR = 3,
    };

    static level_t current_level;
    static std::ostream& current_output;

    static void log(const std::string& msg, level_t level = ALL);
};
}
