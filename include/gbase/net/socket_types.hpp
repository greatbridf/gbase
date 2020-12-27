#pragma once

#ifndef GB_DEFAULT_SOCKET_TIMEOUT
#define GB_DEFAULT_SOCKET_TIMEOUT 30
#endif

#ifndef GB_DEFAULT_SERVER_SOCKET_QUEUE_LENGTH
#define GB_DEFAULT_SERVER_SOCKET_QUEUE_LENGTH 16
#endif

#ifndef GB_DEFAULT_SOCKET_BUFFER_SIZE
#define GB_DEFAULT_SOCKET_BUFFER_SIZE (8 * 1024)
#endif

namespace gb
{
    typedef uint16_t port_t;

    enum socket_states {
        GOOD_BIT      = 0,
        BAD_BIT       = 1,
        READY         = (1 << 1)    ,
        LISTENING     = (1 << 2)    ,
        CONNECTED     = (1 << 3)    ,
        CLOSED        = (1 << 4) + 1,
        ERROR         = (1 << 5) + 1,
    };

    constexpr int SOCKET_ERROR_CLOSED = 9;

    constexpr const char* SOCKET_ADDR_ANY = "0.0.0.0";
    constexpr time_t DEFAULT_SOCKET_TIMEOUT = GB_DEFAULT_SOCKET_TIMEOUT;
    constexpr size_t DEFAULT_SERVER_SOCKET_QUEUE_LENGTH = GB_DEFAULT_SERVER_SOCKET_QUEUE_LENGTH;
    constexpr size_t DEFAULT_SOCKET_BUFFER_SIZE = GB_DEFAULT_SOCKET_BUFFER_SIZE;
}
