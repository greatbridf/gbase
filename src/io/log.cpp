#include <bits/types/struct_tm.h>
#include <gbase/io/log.hpp>

#include <ctime>

gb::logger::level_t gb::logger::current_level = ALL;
std::ostream& gb::logger::current_output = std::cerr;

static const char* level_strings[] = {
    "[ALL]",
    "[INFO]",
    "[WARN]",
    "[ERROR]",
};

static inline const char* level_to_string(gb::logger::level_t level)
{
    return level_strings[level];
}

static inline std::string local_time_string(void)
{
    std::string str;
    str.resize(64, 0x00);
    time_t now = time(0);
    tm* lnow = localtime(&now);
    snprintf(str.data(), 64, "[%04d-%02d-%02d %02d:%02d:%02d]"
            , 1900 + lnow->tm_year, 1 + lnow->tm_mon, lnow->tm_mday
            , lnow->tm_hour, lnow->tm_min, lnow->tm_sec
            );
    return str;
}

void gb::logger::log(const std::string& msg, level_t level)
{
    if (level < current_level || level > 3 /* level_t item count */) {
        return;
    }
    current_output << local_time_string() << ' '
                   << level_to_string(level) << ' '
                   << msg << std::endl;
}
